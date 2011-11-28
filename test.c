#include "jcb.h"
#include "queue.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

p_queue_t ready = NULL;	  /* 就绪队列 */
p_queue_t run = NULL;	  /* 运行队列 */
p_queue_t finish = NULL;  /* 完成队列 */

void create(int process_num);
void priority(void);
void queue_init();
void err_exit(char *str_err);
static int compare(void *data1, void *data2);
static void jcb_print();
p_jcb_t jcb_create();
void info_print();

int main(int argc, char *argv[])
{
	if (argc != 2)	//argv[1]进程数 
	{
		fprintf(stderr, "Usage: [%s] + num_process\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	create(atoi(argv[1]));
	priority();

	return 0;
}

void create(int process_num)//初始化JCB信息
{
	p_jcb_t p = NULL;
	char name_buffer[10];
	float time;
	int i;

	queue_init();
	for (i = 0; i < process_num; i++)
	{
		p = jcb_create();

		printf("请输入第%d作业名(不超过10个字符):", i + 1);
		gets(name_buffer);
		printf("请输入作业执行时间:");
		scanf("%f", &time);

		strcpy(p->name, name_buffer);
		p->status = TASK_STATUS_READY;	/* 就绪状态 */
		p->priority = 50 - time;	/* 得到优先数,这里自定义的,你也可以自定义 */
		p->time.time_run = 0;		/* 才开始执行了的时间为0 */
		p->time.time_needtime = time;	/* 才开始的needtime为time,因为还没有执行 */
		
		queue_insert_maxsize(ready, p, compare);/* 按照优先级大小插入就绪队列 */
		while (getchar() != '\n')
			continue;		
	}
	info_print();
}

void queue_init()
{
	ready = queue_create();
	finish = queue_create();
	if (ready == NULL || finish == NULL)
		err_exit("create error");

	run = NULL;
}

void err_exit(char *str_err)
{
	perror(str_err);
	exit(EXIT_FAILURE);	
}

static int compare(void *data1, void *data2)
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

static void jcb_print(void *jcb)
{
	printf("%-10s%-10f%-10f%-10d", ((p_jcb_t)jcb)->name, ((p_jcb_t)jcb)->time.time_run,
			((p_jcb_t)jcb)->time.time_needtime, ((p_jcb_t)jcb)->priority);

	if (((p_jcb_t)jcb)->status == TASK_STATUS_BLOCK)
		printf("%s\n", "TASK_STATUS_BLOCK");
	else if (((p_jcb_t)jcb)->status == TASK_STATUS_READY)
		printf("%s\n", "TASK_STATUS_READY");
	else if (((p_jcb_t)jcb)->status == TASK_STATUS_RUNNING)
		printf("%s\n", "TASK_STATUS_RUNNING");
	else
		printf("%s\n", "TASK_STATUS_FINISH");
}

void info_print()
{
	
	printf("\t\toutput of priority:\n");
	printf("**********************************************************\n");
	printf("名字  执行了的时间  还剩时间  优先数     状态\n");
	queue_print(ready, jcb_print);
	queue_print(finish, jcb_print);
	getchar();	/* 按回车键继续 */
}

void priority()
{
	run = ready;
	p_q_node_t head = run->head;
	while (head != NULL)
	{
		((p_jcb_t)head->data)->time.time_run += 1;	/* 每运行一次, 执行了的时间增加1单位 	*/
		((p_jcb_t)head->data)->time.time_needtime -= 1; /* 每运行一次, 还剩的时间减1个单位     	*/
		((p_jcb_t)head->data)->priority -= 3;		/* 每运行一次, 优先数较低3个单位  	*/
		if ( 0 == ((p_jcb_t)head->data)->time.time_needtime)
		{
			((p_jcb_t)head->data)->status = TASK_STATUS_FINISH;	/* 状态改为完成态 */
			queue_put(finish, head->data);				/* 插入完成队列   */
			ready->head = head->next;				/* head在后面要删除 */
			ready->size--;
			head = run->head;
		}
		else
		{
			((p_jcb_t)head->data)->status = TASK_STATUS_RUNNING;	/* 状态改为运行态 */
			if (ready->size != 1)
			{
				if (((p_jcb_t)head->data)->priority < ((p_jcb_t)head->next->data)->priority)
				{
					ready->head = head->next;
					queue_insert_maxsize(ready, head->data, compare);
					ready->size--;
					head = run->head;
				}
			}
		}	
		info_print();
	}
}
