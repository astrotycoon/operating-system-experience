/****************************************************
*	文件名: jcb.c
*	
*	文件描述: jcb实现
*
*	创建人: Astrol 2011-11-29 23:57:52
*
*	版本号: 1.0
***************************************************/
#include "jcb.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

p_jcb_t jcb_create()
{
	p_jcb_t new_jcb = NULL;
	new_jcb = (p_jcb_t)malloc(sizeof(jcb_t));
	if (NULL == new_jcb)
		err_exit("malloc error");
	return new_jcb;
}

void err_exit(char *str_err)
{
	perror(str_err);
	exit(EXIT_FAILURE);
}
