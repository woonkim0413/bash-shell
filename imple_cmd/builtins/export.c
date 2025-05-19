/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:33:32 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/18 20:57:02 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void printf_env(t_object *object);
static void insert_node(t_object *object, char **argv, int equals_flag);
static int update_node(t_object *object, char **argv_equals, int equals_flag);

// 환경 변수를 추가하는 함수이다
// 인자 없이 export만 사용하는 경우, declare -x : "~" 형식으로 출력된다
// a=t가 등록된 상태에서 export a=ttt를 사용하면 update되어야 한다
// export aaa=bbb kkk= ttt 이렇게 사용하는 경우,
// 3개의 인자가 모두 반영되어야 한다
// export aa+=add라면 aa에 add가 추가되어야 한다
int	execute_export(t_object *object, t_imp_stus *imp_stus)
{
	char 	**evecve_arg;
	char 	**argv_equals;
	int		equals_flag;

	// export 출력 실행
	if (!object->cmd_info->evecve_argv[1])
	{
		printf_env(object);
		return (1);
	}
	// env node 추가
	evecve_arg = object->cmd_info->evecve_argv;
	equals_flag = 0;
	imp_stus->i = 0;
	while (evecve_arg[++(imp_stus->i)])
	{
		// kkk=이런 경우엔 =으로 쪼개면 kkk와 같아지니 equals_flag를 두어 구분한다
		if (ft_strnstr(evecve_arg[imp_stus->i], "=", \
			ft_strlen(evecve_arg[imp_stus->i])))
			equals_flag = 1;
		argv_equals = ft_split(evecve_arg[imp_stus->i], '=');
		// argv_equals[0]과 동일한 key를 같는 노드가 있는지 확인해야 한다
		// 있다면 node추가가 아니라 update해야됨
		if (!update_node(object, argv_equals, equals_flag))
			insert_node(object, argv_equals, equals_flag);
		free_doublechar(argv_equals);
	}
	// 아래 free가 꼭 필요한가? object에서 꺼낸 값이니 implemnet 밖에서 해제해줄 것 같음
	// free_doublechar(evecve_arg);
	// object->cmd_info->evecve_argv = NULL;
	return (1);
}

// 만약 export로 추가한 path key를 갖고 있는 기존 node가 있다면
// env node를 추가하는 것 대신 해당 node의 값을 update한다
static int update_node(t_object *object, char **argv_equals, int equals_flag)
{
	t_env	*temp;
	int		strlen;

	temp = object->env;
	while (temp)
	{
		strlen = ft_strlen(temp->key);
		if (!ft_strncmp(temp->key, argv_equals[0], strlen))
		{
			if (temp->value)
				free(temp->value);
			if (argv_equals[1])
				temp->value = ft_strdup(argv_equals[1]);
			// 환경변수 value는 없지만 =는 있는 경우 NULL char*할당 
			else if (equals_flag)
			{
				temp->value = (char *)malloc(sizeof(char));
				temp->value[0] = '\0';
			}
			else
				temp->value = NULL;
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}

// env node를 새로 생성하여 기존 env linked list 끝에 붙인다 
static void insert_node(t_object *object, char **argv, int equals_flag)
{
	t_env	*temp;
	t_env 	*env;

	env = (t_env *)malloc(sizeof(t_env));
	temp = object->env;
	while (temp->next)
		temp = temp->next;
	env->key = ft_strdup(argv[0]);
	if (argv[1])
		env->value = ft_strdup(argv[1]);
	// 환경변수 value는 없지만 =는 있는 경우 NULL char*할당 
	else if (equals_flag)
	{
		temp->value = (char *)malloc(sizeof(char));
		temp->value[0] = '\0';
	}
	else
		env->value = NULL;
	env->next = NULL;
	temp->next = env;
}

static void printf_env(t_object *object)
{
	t_env *temp;

	temp = object->env;
	while (temp)
	{
		printf("declare -x %s", temp->key);
		if (temp->value)
			printf("=\"%s\"", temp->value);
		printf("\n");
		temp = temp->next;
	}
	return ;
}
