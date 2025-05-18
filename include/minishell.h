/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:54:25 by rakim             #+#    #+#             */
/*   Updated: 2025/05/18 13:59:11 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <dirent.h>
# include <limits.h>

# define DOLLAR_ASCII 36
# define DOUBLE_QUOTE_ASCII 34
# define SINGLE_QUOTE_ASCII 39
# define MULTI_PIPLINE 0

typedef enum e_token_type
{
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_HEREDOC,
	TOKEN_APPEND
}	t_token_type;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_redirect
{
	t_token_type		type;
	char				*file_path;
	int					heredoc_fd;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_cmd_info
{	
	char				*cmd;
	char				**evecve_argv;
	char				*cmd_path;
	t_redirect			*redirect;
	struct s_cmd_info	*prev;
	struct s_cmd_info	*next;
}	t_cmd_info;

// parsing부에서 전달 받은 정보 저장
typedef struct s_object
{
	t_cmd_info	*cmd_info;
	t_env		*env;
	int			last_exit_status;
	int			heredoc_interrupted;
}	t_object;

/* parsing에서 구분자로 split 하기 위한 구조체 */
typedef struct s_result_info
{
	char	**result;
	int		result_idx;
	int		add_value;
	int		file_path_flag;
}	t_result_info;

typedef struct s_check_redir_arg
{
	char		**src;
	int			current_src;
	char		*cmd;
	int			file_path_flag;
	int			end_flag;
	t_redirect	*redirect;
	t_object	*object;
}	t_check_redir_arg;

// imple에서 사용할 정보 저장
// 구현부에서 사용할 정보 저장 (나중에 error handler로 free할 수 있게 코드 변경)
typedef struct s_imp_stus
{
	int		i;
	int		all_path; // 이후의 함수 실행 안 하고 끝내야 할 때 사용
	int		input_fd; // 리다이렉션 할 때 사용
	int		output_fd; // 리다이렉션 할 때 사용
	int		stdoutFd; // stdout buffer에 연결된 fd보존
	int		stdinFd;
	int		cur_c_n; // curent_command_number
	int		total_c_n; // total_cmmand_number
	int		stderr_pipe[2];
	pid_t	*chil_pid;
	int		**pipeFd; // 명령어 갯수에 따른 pipe용 int배열 저장
}	t_imp_stus;

/* init */
void		init(int length, char *input[], t_object *object, char **env);
void		init_signal(void);
void		init_child_signal(void);
/* init/utils */
int			is_all_space(const char *line);
/* error */
void		throw_error(char *message, t_object *object, \
	t_imp_stus *imp_stus, char **line);
/* free/free */
void		free_object(t_object *object);
void		free_stus_and_object(t_object *object, t_imp_stus *imp_stus);
void		free_string(char **line);
void		free_arg(t_check_redir_arg **arg);
/* free/free_helper */
void		free_string_arr(char ***string_arr);
void		free_stus(t_imp_stus *imp_stus);
void		free_env(t_env **env);
void		free_redirect(t_redirect **redirect);
void		free_cmd_info(t_cmd_info **cmd_info);
/* parsing/parsing_token.c */
void		parsing(char **line_splited_pipe, t_object *object);
/* parsing/quote/quote_handler.c */
void		check_quotes(char **line, t_object *object);
int			is_have_quotes(char *line);
/* parsing/quote/clean_up_quote.c */
void		clean_up_quote(t_cmd_info *cmd_info);
/* parsing/env/env_helper.c */
char		*get_env(char *key, t_env *env);
/* parsing/env/extend_env.c */
char		*extract_key_in(char *src);
void		extend_env(char **line, int *dolloar_idx, t_object *object);
/* parsing/redirect/redirect_parsing_handler.c */
void		check_redirect(t_check_redir_arg *arg);
/* parsing/heardoc/heardoc_handler.c */
void		handle_heardoc(t_object *object);
/* parsing/utils/parsing_utils.c */
void		check_pipe(char **line, t_object *object);
void		print_all_cmd(t_object *object);
void		whitespace_convert_to_space(char **line);
/* parsing/utils/split_by_redir_2.c */
void		split_by_redir(char **temp, t_result_info *result_info);
/* parsing/utils/split_by_redir.c */
char		**split_redir_with_quote(char **temp);
/* parsing/utils/split_with_quote.c */
char		**split_by_space_with_quote(char *s);
/* parsing/utils/seperate_helper.c */
char		**extend_env_and_split(char **line, t_object *object);
void		set_toggle(char c, int *in_single, int *in_double);

/*---------------------- 구현부 함수 ----------------------*/

/* /imple_cmd/builtin */
int			execute_echo(t_object *object, t_imp_stus *imp_stus);
int			execute_env(t_object *object, t_imp_stus *imp_stus);
int			execute_exit(t_object *object, t_imp_stus *imp_stus);
int			execute_export(t_object *object, t_imp_stus *imp_stus);
int			execute_pwd(t_object *object, \
	t_imp_stus *imp_stus, int prev_cwd_fd);
int			execute_unset(t_object *object, t_imp_stus *imp_stus);
int			execute_cd(t_object *object, t_imp_stus *imp_stus);

/* /imple_cmd/implement.c*/
void		implement(t_object *object);

/* /imple_cmd/cmd_path_find.c*/
int			find_path(t_cmd_info *t_cmd, t_env *env);

/* /imple_cmd/redirect_setting.c */
void		input_output_setting(t_object *object, t_imp_stus *imp_stus, \
		int one_builtin_flag);

/* /imple_cmd/imp_utils.c */
void		create_execve_args(t_cmd_info *cmd_info);
char		**env_to_char(t_env *env, int i);
void		free_doublechar(char **argv);
int			cmd_null_check(t_object *object, t_imp_stus *imp_stus);

/* /imple_cmd/parent_wait_to_child.c */
void		wait_childs_process(t_object *object, t_imp_stus *imp_stus);

/* /imple_cmd/builtins/builtin_utils1.c */
void		execute_one_builtin(t_object *object, t_imp_stus *imp_stus);
int			check_one_builtin(t_object *object, t_imp_stus *imp_stus);
int			execute_builtins(t_object *object, t_imp_stus *imp_stus);

/* /imple_cmd/imple_setting */
void		setting_pipline(t_cmd_info *t_cmd, t_imp_stus *imp_stus);
void		pipe_and_fork(t_imp_stus *imp_stus);
void		init_t_imp_stus(t_imp_stus *imp_stus);

/* /print_log.c */
void		print_log(int stdout_fd, t_object *object, const char *format, ...);

/* imple_cmd/help_except_case.c */
void		single_duuble_quates_check(t_object *object);

#endif
