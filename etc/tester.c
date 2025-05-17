#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define BUFFER_SIZE 8192
#define MAX_LINE_LENGTH 1024
#define MAX_CMDS_PER_BLOCK 1000

void trim_output(char *buf) {
    char *start = buf + strspn(buf, " \t\r\n");
    char *end = start + strlen(start);
    while (end > start && (end[-1] == '\n' || end[-1] == '\r' ||
                           end[-1] == ' ' || end[-1] == '\t'))
        end--;
    *end = '\0';
    memmove(buf, start, end - start + 1);
}

void clean_minishell_prompt(char *buf) {
    const char *prompt = "minishell $ : ";
    char *line_start = buf;
    char cleaned[BUFFER_SIZE] = {0};
    char *write_ptr = cleaned;

    while (*line_start) {
        char *next_line = strchr(line_start, '\n');
        if (next_line) *next_line = '\0';

        if (strncmp(line_start, prompt, strlen(prompt)) != 0) {
            write_ptr += sprintf(write_ptr, "%s\n", line_start);
        }

        if (!next_line) break;
        *next_line = '\n';
        line_start = next_line + 1;
    }

    strncpy(buf, cleaned, BUFFER_SIZE - 1);
    buf[BUFFER_SIZE - 1] = '\0';
}

void trim_whitespace(char *str) {
    char *start = str + strspn(str, " \t\r\n");
    char *end = start + strlen(start);
    while (end > start && strchr(" \t\r\n", end[-1])) end--;
    *end = '\0';
    memmove(str, start, end - start + 1);
}

int run_shell_command(const char *shell_path,
                      const char *input,
                      char *output_buf,
                      size_t buf_size) {
    char temp_file[] = "/tmp/test_output_XXXXXX";
    int fd = mkstemp(temp_file);
    if (fd == -1) return -1;
    close(fd);

    int pipefd[2];
    if (pipe(pipefd) == -1) return -1;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    } else if (pid == 0) {
        // Child: stdin ← pipe, stdout/stderr → temp_file
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        int outfd = open(temp_file, O_CREAT | O_TRUNC | O_WRONLY, 0666);
        if (outfd < 0) exit(1);
        dup2(outfd, STDOUT_FILENO);
        dup2(outfd, STDERR_FILENO);
        close(outfd);

        execl(shell_path, shell_path, (char *)NULL);
        perror("execl");
        exit(1);
    } else {
        // Parent: input → pipe, 대기, temp_file 읽기
        close(pipefd[0]);
        write(pipefd[1], input, strlen(input));
        write(pipefd[1], "\n", 1);
        close(pipefd[1]);

        waitpid(pid, NULL, 0);

        FILE *fp = fopen(temp_file, "r");
        if (!fp) return -1;
        size_t n = fread(output_buf, 1, buf_size - 1, fp);
        output_buf[n] = '\0';
        fclose(fp);
        unlink(temp_file);
        return 0;
    }
}

// 한 블록(명령어 묶음)을 처리
void process_block(char **cmds, int cmd_count, int test_num) {
    char bash_output[BUFFER_SIZE];
    char mini_output[BUFFER_SIZE];
    char dummy[BUFFER_SIZE];

    // 1) 블록 전체 설명 문자열(desc) 생성
    char desc[BUFFER_SIZE] = {0};
    for (int i = 0; i < cmd_count; i++) {
        strncat(desc, cmds[i], BUFFER_SIZE - 1 - strlen(desc));
        if (i < cmd_count - 1)
            strncat(desc, " --->> ", BUFFER_SIZE - 1 - strlen(desc));
    }

    // 2) 마지막 명령 전까지는 실행만 (출력 무시)
    for (int i = 0; i < cmd_count - 1; i++) {
        run_shell_command("/bin/bash",    cmds[i], dummy, BUFFER_SIZE);
        run_shell_command("./minishell", cmds[i], dummy, BUFFER_SIZE);
    }

    // 3) 마지막 명령어 출력 캡처 & 비교
    const char *last = cmds[cmd_count - 1];
    memset(bash_output, 0, BUFFER_SIZE);
    memset(mini_output, 0, BUFFER_SIZE);

    run_shell_command("/bin/bash",    last, bash_output, BUFFER_SIZE);
    run_shell_command("./minishell", last, mini_output, BUFFER_SIZE);

    clean_minishell_prompt(mini_output);
    trim_output(bash_output);
    trim_output(mini_output);

    int pass = strcmp(bash_output, mini_output) == 0;

    // 4) 전체 블록 설명(desc)을 헤더에 출력
    printf("========================================================\n");
    printf("[Test %d: %s - %s]\n",
           test_num, desc, pass ? "PASS" : "FAIL");

    if (!pass) {
        printf("----- bash output -----\n");
        printf("\x1b[31m%s\x1b[0m\n", bash_output);
        printf("----- minishell output -----\n");
        printf("\x1b[31m%s\x1b[0m\n", mini_output);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <test_file>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    char *cmds[MAX_CMDS_PER_BLOCK];
    int cmd_count = 0, test_num = 1;
    printf("========================================================\n");
    printf("비교 환경 : bash -c (프롬프트 없는 단순화 된 출력 bash)\n\n");
    printf("명령어 읽는 방식 : enter을 기준으로 명령어 블록 인식, 3개의 명령어로 되어 있는 명령어 블록인 경우 마지막 명령어의 출력만 비교하고 나머지는 실행만 한다.\n\n");
    printf("처리 하지 못하는 명령어 종류 : 커스텀 에러 메세지, <<, >> \n");
    while (fgets(line, sizeof(line), fp)) {
        trim_whitespace(line);
        // 빈 줄이면 현재 블록 처리
        if (line[0] == '\0') {
            if (cmd_count > 0) {
                process_block(cmds, cmd_count, test_num++);
                for (int i = 0; i < cmd_count; i++) free(cmds[i]);
                cmd_count = 0;
            }
        } else {
            // 명령어 저장
            if (cmd_count < MAX_CMDS_PER_BLOCK)
                cmds[cmd_count++] = strdup(line);
        }
    }
    // 마지막 블록이 남아 있으면 처리
    if (cmd_count > 0) {
        process_block(cmds, cmd_count, test_num++);
        for (int i = 0; i < cmd_count; i++) free(cmds[i]);
    }

    printf("========================================================\n");
    fclose(fp);
    return 0;
}
