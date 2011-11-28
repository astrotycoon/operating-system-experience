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
