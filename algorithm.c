#include "jcb.h"
#include "queue.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "algorithm.h"

/*extern*/ p_queue_t ready = NULL;
/*extern*/ p_queue_t run = NULL;
/*extern*/ p_queue_t finish = NULL;

static void queue_init();
static void jcb_print(void *jcb);
static void priority_print();
static int compare_priority(void *data1, void *data2);
static void eat_line();	/* 清空缓冲区 */

static void fcfs_print(void *jcb);
static void begin_end_time();
static void turnaround();
static void average_turnaround_print();

static int compare_sjf(void *data1, void *data2);
static void sjf_print();

static void queue_init()
{
	ready = queue_create();
	finish = queue_create();
	if (NULL == ready || NULL == finish)
		err_exit("queue create error");
	run = ready;
}

/* 初始化JCB信息 */
void priority_create(int num_process)
{
	p_jcb_t p = NULL;
	char name_buffer[10];
	float time;
	int i;

	queue_init();
	for (i = 0; i < num_process; i++)
	{
		p = jcb_create();
		
		eat_line();	
                printf("请输入第%d作业名(不超过10个字符):", i + 1);
                gets(name_buffer);
                printf("请输入作业执行时间:");
                scanf("%f", &time);

                strcpy(p->name, name_buffer);
                p->status = TASK_STATUS_READY;  /* 就绪状态 */
                p->priority = 50 - time;        /* 得到优先数,这里自定义的,你也可以自定义 */
                p->time.time_run = 0;           /* 才开始执行了的时间为0 */
                p->time.time_needtime = time;   /* 才开始的needtime为time,因为还没有执行 */

                queue_insert_maxsize(ready, p, compare_priority);/* 按照优先级大小插入就绪队列 */
                eat_line();
	}
	priority_print();
}

static void priority_print()
{
	printf("\t\toutput of priority:\n");
        printf("**********************************************************\n");
        printf("名字  执行了的时间  还剩时间  优先数     状态\n");
        queue_print(ready, jcb_print);
        queue_print(finish, jcb_print);
        getchar();      /* 按回车键继续 */
}

static void jcb_print(void *jcb)
{
	p_jcb_t temp = (p_jcb_t)jcb;
	printf("%-10s%-10f%-10f%-10d", temp->name, temp->time.time_run,
                        temp->time.time_needtime, temp->priority);

        if (temp->status == TASK_STATUS_BLOCK)
                printf("%s\n", "TASK_STATUS_BLOCK");
        else if (temp->status == TASK_STATUS_READY)
                printf("%s\n", "TASK_STATUS_READY");
        else if (temp->status == TASK_STATUS_RUNNING)
                printf("%s\n", "TASK_STATUS_RUNNING");
        else
                printf("%s\n", "TASK_STATUS_FINISH");
}

static int compare_priority(void *data1, void *data2)
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

static void eat_line()
{
	while (getchar() != '\n')
		continue;
}

void priority()
{
	run = ready;
        p_q_node_t head = run->head;
        while (head != NULL)
        {
                ((p_jcb_t)head->data)->time.time_run += 1;      /* 每运行一次, 执行了的时间增加1单位    */
                ((p_jcb_t)head->data)->time.time_needtime -= 1; /* 每运行一次, 还剩的时间减1个单位      */
                ((p_jcb_t)head->data)->priority -= 3;           /* 每运行一次, 优先数较低3个单位        */
                if ( 0 == ((p_jcb_t)head->data)->time.time_needtime)
                {
                        ((p_jcb_t)head->data)->status = TASK_STATUS_FINISH;     /* 状态改为完成态 */
                        queue_put(finish, head->data);                          /* 插入完成队列   */
                        ready->head = head->next;                             
			free(head);
                        ready->size--;
                        head = run->head;
                }
                else
                {
                        ((p_jcb_t)head->data)->status = TASK_STATUS_RUNNING;    /* 状态改为运行态 */
                        if (ready->size != 1)
                        {
                                if (((p_jcb_t)head->data)->priority < ((p_jcb_t)head->next->data)->priority)
                                {
                                        ready->head = head->next;
                                        queue_insert_maxsize(ready, head->data, compare_priority);
                                        ready->size--;
                                        head = run->head;
                                }
                        }
                }
                priority_print();
        }

	queue_destroy(ready);
	queue_destroy(finish);
}


void fcfs_create(int num_process)
{
	p_jcb_t p = NULL;
	char name_buffer[10];
	int i;

	queue_init();
	for (i = 0; i < num_process; i++)
	{
		p = jcb_create();

		eat_line();
		printf("请输入第%d作业名(不超过10个字符):", i + 1);
		gets(name_buffer);
		printf("请输入作业要运行的时间:");
		scanf("%f", &p->time.time_needtime);

		strcpy(p->name, name_buffer);
		p->time.time_submit = i;	/* 这里自定义作业的提交时间 */
		p->status = TASK_STATUS_READY;	/* 就绪状态 */
		
		queue_put(ready, p);	/* 插入就绪队列 */
		eat_line();
	}
	begin_end_time();
	turnaround();
}

void fcfs()
{
	run = ready;

	queue_print(ready, fcfs_print);	/* 打印运行基本信息*/
	average_turnaround_print();
	queue_destroy(ready);
}

static void fcfs_print(void *jcb)
{
	p_jcb_t temp = (p_jcb_t)jcb;

	printf("作业%s正在运行,估计其运行情况如下:\n", temp->name);
	printf("开始运行时刻: %f\n", temp->time.time_begin);
	printf("完成时刻: %f\n", temp->time.time_end);
	printf("周转时间: %f\n", temp->time.time_turnaround);
	printf("带权周转时间: %f\n", temp->time.time_turnaround_weight);
	printf("\n作业%s运行完毕.按回车键继续... ...\n", temp->name);
	getchar();
}
/* 初始化开始时刻与完成时刻 */
static void begin_end_time()
{
	p_q_node_t head = ready->head;
	float before = 0.0;
	while (NULL != head)
	{
		((p_jcb_t)head->data)->time.time_begin = before;	/* 作业的开始时刻 */
		((p_jcb_t)head->data)->time.time_end = ((p_jcb_t)head->data)->time.time_begin
			+ ((p_jcb_t)head->data)->time.time_needtime;	/* 作业的完成时刻 */
		before = ((p_jcb_t)head->data)->time.time_end;
		head = head->next;
	}
}

/* 初始化作业周转时间和带权周转时间 */
static void turnaround()
{
	p_q_node_t head = ready->head;
	while (NULL != head)
	{
		((p_jcb_t)head->data)->time.time_turnaround = ((p_jcb_t)head->data)->time.time_end
						- ((p_jcb_t)head->data)->time.time_submit;
		((p_jcb_t)head->data)->time.time_turnaround_weight = ((p_jcb_t)head->data)->time.time_turnaround
						/ ((p_jcb_t)head->data)->time.time_needtime;
		head = head->next;
	}
}

/* 打印平均周转时间和平均带权周转时间 */
static void average_turnaround_print()
{
	p_q_node_t head = ready->head;
	float turnaround = 0;
	float turnaround_weight = 0;
	float average_turnaround;
	float average_turnaround_weight;

	while (NULL != head)
	{
		turnaround += ((p_jcb_t)head->data)->time.time_turnaround;
		turnaround_weight += ((p_jcb_t)head->data)->time.time_turnaround_weight;
		head = head->next;
	}
	
	average_turnaround = turnaround / ready->size;
	average_turnaround_weight = turnaround_weight / ready->size;
	printf("%d个作业的平均周转时间为:%f\n",ready->size, average_turnaround);
	printf("%d个作业的平均带权周转时间为:%f\n", ready->size, average_turnaround_weight);
}

void sjf_create(int num_process)
{
	p_jcb_t p = NULL;
	char name_buffer[10];
	float time;
	int i;

	queue_init();
	for (i = 0; i < num_process; i++)
	{
		p = jcb_create();

		eat_line();
		printf("请输入第%d作业名(不超过10个字符):", i + 1);
		gets(name_buffer);
		printf("请输入作业执行时间:");
		scanf("%f", &time);

		strcpy(p->name, name_buffer);
		p->status = TASK_STATUS_READY;	/* 就绪状态 */
		p->time.time_run = 0;		/* 才开始执行了的时间为0 */
		p->time.time_needtime = time;	/* 才开始的needtime为time, 因为还没有执行 */
		p->priority = 50 - time;
	
		queue_insert_minsize(ready, p, compare_sjf);	/* 按运行时间从小到达的顺序插入就绪队列 */
		eat_line();
	}	
	sjf_print();
}

/* 浮点数大小比较 参考:http://wenku.baidu.com/view/833b736548d7c1c708a14566.html */
static int compare_sjf(void *data1, void *data2)
{
	float limit = 0.000001;	/* 比较阀值 */
	float temp1 = ((p_jcb_t)data1)->time.time_needtime;
	float temp2 = ((p_jcb_t)data2)->time.time_needtime;

	if ((temp1 - temp2) > limit)
		return 1;
	else if (((temp1 - temp2) < limit) && ((temp1 - temp2) > limit))
		return 0;
	else
		return -1;

}

static void sjf_print()
{
	printf("\t\toutput of sjf\n");
	printf("*********************************************************\n");
        printf("名字  执行了的时间  还剩时间  优先数     状态\n");
	queue_print(ready, jcb_print);
	queue_print(finish, jcb_print);
	getchar();	/* 按回车键继续 */
}

void sjf()
{
	run = ready;
	p_q_node_t head = run->head;
	while (NULL != head)
	{
		((p_jcb_t)head->data)->time.time_run += 1;	/* 每运行一次, 执行了的时间增加1个单位 */
		((p_jcb_t)head->data)->time.time_needtime -= 1;	/* 每运行一次, 还剩的时间减1个单位 */
		((p_jcb_t)head->data)->priority -= 3;

		if (0 == ((p_jcb_t)head->data)->time.time_needtime)
		{
			((p_jcb_t)head->data)->status = TASK_STATUS_FINISH;	/* 状态改为完成态 */
			queue_put(finish, head->data);				/* 插入完成队列 */
			ready->head = head->next;
			ready->size--;
			head = run->head;
		}
		else
		{
			((p_jcb_t)head->data)->status = TASK_STATUS_RUNNING;	/* 状态改为运行态 */
			if (ready->size != 1)
			{
				if (((p_jcb_t)head->data)->time.time_needtime 
				< ((p_jcb_t)head->data)->time.time_needtime)
				{
					ready->head = head->next;
					queue_insert_minsize(ready, head->data, compare_sjf);
					ready->size--;
					free(head);
					head = run->head;
				}
			}
		}
		sjf_print();
	}
	queue_destroy(ready);
	queue_destroy(finish);
}
