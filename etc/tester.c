#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <ctype.h>

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
                      size_t buf_size,
                      int *exit_status) {
    char temp_file[] = "/tmp/test_output_XXXXXX";
    int fd = mkstemp(temp_file);
    if (fd == -1) return -1;
    close(fd);

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) return -1;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    } else if (pid == 0) {
        // Child: stdin ← pipe, stdout/stderr → temp_file
        close(pipe_fd[1]);
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);

        int outfd = open(temp_file, O_CREAT | O_TRUNC | O_WRONLY, 0666);
        if (outfd < 0) exit(1);
        dup2(outfd, STDOUT_FILENO);
        dup2(outfd, STDERR_FILENO);
        close(outfd);

        if (strcmp(shell_path, "./minishell") == 0)
        {
            execl("/usr/bin/valgrind",
                    "valgrind",
                    "--suppressions=/home/rakim/Desktop/minishell/etc/readline-leak.supp",
                    "./minishell",
                    (char*)NULL);
        }
        else
            execl(shell_path, shell_path, (char *)NULL);
        perror("execl");
        exit(1);
    } else {
        // Parent: input → pipe, 대기, temp_file 읽기
        close(pipe_fd[0]);
        write(pipe_fd[1], input, strlen(input));
        write(pipe_fd[1], "\n", 1);
        close(pipe_fd[1]);

        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            *exit_status = WEXITSTATUS(status);
        else
            *exit_status = -1;

        FILE *fp = fopen(temp_file, "r");
        if (!fp) return -1;
        size_t n = fread(output_buf, 1, buf_size - 1, fp);
        output_buf[n] = '\0';
        fclose(fp);
        unlink(temp_file);
        return 0;
    }
}

void process_block(char **cmds, int cmd_count, int test_num) {
    char bash_output[BUFFER_SIZE];
    char mini_output[BUFFER_SIZE];
    char raw_mini[BUFFER_SIZE];
    char dummy[BUFFER_SIZE];
    int bash_status, mini_status;

    // 1) desc 생성 (생략)
    char desc[BUFFER_SIZE] = {0};
    for (int i = 0; i < cmd_count; i++) {
        strncat(desc, cmds[i], BUFFER_SIZE - 1 - strlen(desc));
        if (i < cmd_count - 1) {
            strncat(desc, " --->> ", BUFFER_SIZE - 1 - strlen(desc));
        }
    }

    // 2) 마지막 전까지 실행만 (출력 무시)
    for (int i = 0; i < cmd_count - 1; i++) {
        run_shell_command("/bin/bash",    cmds[i], dummy, BUFFER_SIZE, &bash_status);
        run_shell_command("./minishell", cmds[i], dummy, BUFFER_SIZE, &mini_status);
    }

    // 3) 마지막 명령어 출력 + 상태 캡처
    const char *last = cmds[cmd_count - 1];
    memset(bash_output, 0, BUFFER_SIZE);
    memset(mini_output, 0, BUFFER_SIZE);
    run_shell_command("/bin/bash",    last, bash_output, BUFFER_SIZE, &bash_status);
    run_shell_command("./minishell", last, mini_output, BUFFER_SIZE, &mini_status);

    clean_minishell_prompt(mini_output);
    trim_output(bash_output);
    trim_output(mini_output);

    // 원본 복사 (Valgrind 로그 포함)
    strncpy(raw_mini, mini_output, BUFFER_SIZE - 1);
    raw_mini[BUFFER_SIZE - 1] = '\0';

    // 4) Valgrind 로그 파싱해서 누수 여부 확인
    int dl=0, il=0, pl=0, sr=0;
    char *p = raw_mini;
    bool hasLeak = false;
    if ((p = strstr(raw_mini, "definitely lost:")) &&
        sscanf(p, "definitely lost: %d bytes", &dl) == 1 &&
        (p = strstr(raw_mini, "indirectly lost:")) &&
        sscanf(p, "indirectly lost: %d bytes", &il) == 1 &&
        (p = strstr(raw_mini, "possibly lost:")) &&
        sscanf(p, "possibly lost: %d bytes", &pl) == 1 &&
        (p = strstr(raw_mini, "still reachable:")) &&
        sscanf(p, "still reachable: %d bytes", &sr) == 1) {
        hasLeak = (dl != 0 || il != 0 || pl != 0 || sr != 0);
    }

    // 5) mini_output 에서 "==" 로 시작하는 줄만 걸러내기
    {
        char filtered[BUFFER_SIZE] = {0};
        char *saveptr, *line = strtok_r(mini_output, "\n", &saveptr);
        while (line) {
            // 앞 공백 건너뛰고
            char *t = line + strspn(line, " \t");
            if (strncmp(t, "==", 2) != 0 && *t != '\0') {
                // 남은 여유 공간 계산
                size_t used = strlen(filtered);
                size_t remain = BUFFER_SIZE - used - 1;
                // 실제 출력 문자열 추가
                strncat(filtered, t, remain);
                // 개행 추가
                remain = BUFFER_SIZE - strlen(filtered) - 1;
                strncat(filtered, "\n", remain);
            }
            line = strtok_r(NULL, "\n", &saveptr);
        }
        // 다시 복사
        strncpy(mini_output, filtered, BUFFER_SIZE - 1);
        mini_output[BUFFER_SIZE - 1] = '\0';
        trim_output(mini_output);
    }

    // 6) PASS 판정: exit code 또는 출력 비교
    bool pass = false;

    if (bash_status != 0) {
        pass = (strstr(mini_output, "Error") != NULL);
    } else {
        pass = (strcmp(bash_output, mini_output) == 0);
    }
    

    // 7) 결과 출력
    printf("========================================================================\n");
    printf("[Test %d: %s - %s]\n", test_num, desc, pass ? "PASS" : "FAIL");

    if (!pass) {
        printf("----- bash output -----\n");
        printf("\x1b[31m%s\x1b[0m\n", bash_output);
        printf("----- minishell output -----\n");
        if (hasLeak) {
            // 누수 발생 시, 원본 로그(Valgrind 포함) 보여주기
            printf("\x1b[31m%s\x1b[0m\n", raw_mini);
        } else {
            // 누수 없으면, 필터된 실제 출력만
            printf("\x1b[31m%s\x1b[0m\n", mini_output);
        }
    }
    printf("========================================================================\n");
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
        // #이면 출력만
        if (line[0] == '#')
        {
            printf("\n");
            printf("\x1b[33m%s\x1b[0m\n", line);
            printf("\n");
        }
        // 빈 줄이면 현재 블록 처리
        else if (line[0] == '\0') {
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
