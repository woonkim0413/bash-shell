/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imp_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 20:07:27 by woonkim           #+#    #+#             */
/*   Updated: 2025/04/22 20:16:17 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// echo, pwd, unset, export, env, exit인 경우
int	is_builtins(t_object *object)
{
	// TODO
}

// 만약 pwd, echo, env인 경우 출력 값이 있으니 파이프라인을 통해서
// 다음 명령어의 input으로 사용될 수 있음
// 그러니 pipe를 이용하여 data를 buffer넣어주는 것이 좋을 것 같다
void execute_builtins(t_object *object, t_imp_stus *imp_stus)
{
	// TODO
	// 다음 명령어로 이동
	imp_stus->cur_c_n += 1;
}
