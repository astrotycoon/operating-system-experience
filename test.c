#include "jcb.h"
#include "queue.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

p_queue_t ready = NULL;	/* 就绪队列 */
p_queue_t run = NULL;	/* 运行队列 */
p_queue_t finish = NULL;  /* 完成队列 */

void err_exit(char *str_err);
int compare(void *data1, void *data2);
p_jcb_t jcb_create();
void jcb_print();

int main(int argc, char *argv[])
{
	if (argc != 2)	//argv[1]进程数 
	{
		fprintf(stderr, "Usage: filename num_process");
		exit(EXIT_FAILURE);
	}
	create(argv[1]);
	priority();

	return 0;
}

void create(int process_num)//初始化JCB信息
{
	p_jcb_t p = NULL;
	char name_buffer[10];
	int time;
	int i;

	queue_init();
	for (i = 0; i < process_num; i++)
	{
		p = jcb_create();

		printf("请输入作业名(不超过10个字符):");
		gets(name_buffer);
		printf("请输入作业执行时间:");
		scanf("%f", &time);

		strcpy(p->name, name_buffer);
		p->status = TASK_STATUS_READY;	/* 就绪状态 */
		p->priority = 50 - time;	/* 得到优先数,你可以自定义 */
		p->time.time_run = 0;		/* 才开始执行了的时间为0 */
		p->time.time_needtime = time;	/* 才开始的needtime为time,因为还没有执行 */
		
		queue_insert_maxsize(ready, &p, compare);/* 按照优先级大小插入就绪队列 */
		
	}
	printf("\t\t\t\toutput of priority:\n");
	printf("************************************************\n");
	jcb_print();
	
	

}

void queue_init()
{
	ready = queue_create(void);
	if (ready == NULL)
		err_exit("create error");
	run = finish = NULL;
}

void err_exit(char *str_err)
{
	perror(str_err);
	exit(EXIT_FAILURE);	
}

int compare(void *data1, void *data2)
{
	p_jcb_t p = (p_jcb_t)data1;
	p_jcb_t q = (p_jcb_t)data2;

	if ((p->priority - q->priority) > 0)
		return 1;
	else if ((p->priority - q->priority) == 0)
		return 0;
	else	
		return -1;
}

p_jcb_t jcb_create()
{
	p_jcb_t new_jcb = NULL;
	new_jcb = (p_jcb_t)malloc(sizeof(jcb_t));
	if (NULL == new_jcb)
		err_exit("malloc error");
	return new_jcb;
}

void jcb_print()
{
	p_q_nodt_t head = ready->head;
	
	while (head != NULL)
	{
		printf("%-10s%-10f%-10f%-10d%s\n", head->data->name,
		head->data->time.time_run, head->data->time.needtime,
		head->data->priority, TASK_STATUS_READY);
	}

	getchar();
}
