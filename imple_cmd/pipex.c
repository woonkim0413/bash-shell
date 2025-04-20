/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:38:46 by woonkim           #+#    #+#             */
/*   Updated: 2025/03/07 13:19:45 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void child_work(char **main_argv, char **main_envp, int *pipeFd);
void parent_work(char **main_argv, char **main_envp, int *pipeFd);

// argv[0] = 프로그램 실행 문자열, argv[1] = 파일1, argv[2] = cmd1
// argv[3] = cmd2, argv[4] = 파일2
int main(int argc, char *argv[], char *envp[])
{
    int pipeFd[2];
    int child_status;
    pid_t pid;

    if (argc != 5)
    {
        write(2, "please insert a valid args\n", 27);
        // exit은 arg로 0을 받으면 정상 종료, 그 외의 수는 비정상 종료를 의미함
        exit(1);
    }
    pipe(pipeFd);
    // 자식 process 생성
    pid = fork();
    if (pid == 0)
    {
        // 자식은 쓰기만 하기에 읽기 pipeFd[0]은 close
        close(pipeFd[0]);
        // 자식 process에서 명령어를 실행하여 결과 값을 pipe에 전달
        child_work(argv, envp, pipeFd);
    }
    else
    {
        // 부모는 읽기만 하기에 쓰기 fd는 close
        close(pipeFd[1]);
        // pipe에서 글자 읽어오기 -> 해당 data로 명령어 실행 -> 결과 값을 file2에 넣기
        parent_work(argv, envp, pipeFd);
        waitpid(pid, &child_status, 0);
    }
    return (0);
}

void error_check(char *str, char **argv)
{
    if (str == NULL)
    {
        memory_free(argv);
        exit(1);
    }
}

void free_and_exit(char *cmd_path, char **argv)
{
    // execve 실패 시에 memory free
    memory_free(argv);
    free(cmd_path);
    exit(1); // exit(0)은 정상적인 종료, 그 외의 숫자를 arg로 주면 비정상 종료를 의미한다
}

// execve를 사용하기 전에 표준 입력을 argv[1]으로, 포준 출력은
// pipe로 전환 해주어야 한다
void child_work(char **main_argv, char **main_envp, int *pipeFd)
{
    char **argv;
    char **envp;
    char *cmd_path;
    int fileFd;

    fileFd = open(main_argv[1], O_RDONLY);
    if (fileFd == -1)
    {
        perror(main_argv[1]);
        exit(1);
    }
    // 명령어 string을 명령어, 옵션 각각으로 쪼갬
    // spilit 반환 값은 malloc
    argv = ft_split(main_argv[2], ' ');
    argv = fuck_case_check(argv);
    envp = NULL;
    // find_path 반환 값은 malloc
    cmd_path = find_path(argv[0], main_envp);
    // cmd_path가 널이면 명령어를 못 찾은거니 모두 free
    error_check(cmd_path, argv);
    // filFd를 통해 접근할 수 있었던 file을 fd 0으로도(이전의 표준 입력) 접근할 수 있도록 redirection
    dup2(fileFd, 0);
    dup2(pipeFd[1], 1);
    close(pipeFd[1]);
    close(fileFd);
    // execve가 실패하는 경우에는 malloc을 모두 free해주어야 한다 (성공하면 알아서 해제됨)
    if (execve(cmd_path, argv, envp) == -1)
    {
        perror("execve 실패\n");
        free_and_exit(cmd_path, argv);
    }
}

void parent_work(char **main_argv, char **main_envp, int *pipeFd)
{
    char **argv;
    char **envp;
    char *cmd_path;
    int fileFd;

    // 파일이 존재하지 않다면 파일을 만든다 마지막 arg는 생성한 파일의 권한이다
    fileFd = open(main_argv[4], O_WRONLY | O_CREAT, 0644);
    if (fileFd == -1)
    {
        perror("file1 열기 실패\n");
        exit(1);
    }
    argv = ft_split(main_argv[3], ' ');
    envp = NULL;
    cmd_path = find_path(argv[0], main_envp);
    error_check(cmd_path, argv);
    dup2(pipeFd[0], 0);
    dup2(fileFd, 1);
    close(pipeFd[0]);
    close(fileFd);
    if (execve(cmd_path, argv, envp) == -1)
    {
        perror("execve 실패\n");
        free_and_exit(cmd_path, argv);
    }
}
