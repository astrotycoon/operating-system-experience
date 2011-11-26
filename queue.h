/************************************************************
*	文件名: queue.h
*
*	文件描述: 链队列的头文件 
*
*	创建人: Astrol 2011-11-24 10:05:45
*
*	版本号: 1.0
*
*	修改记录:
************************************************************/

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdbool.h>	/* C99特性 */
#include <stddef.h>

/* 队列结点定义 */
typedef struct q_node
{
	void *data;
	struct q_node *next;
}q_node_t, *p_q_node_t;

/* 队列链表结构定义 */
typedef struct queue
{
	size_t size;		/* 队列大小 */
	p_q_node_t head;	/* 对头指针,若队列不空,指向队列头结点 */
	p_q_node_t tail;	/* 队尾指针,若不空,指向队尾元素,若为空,指向头结点 */
}queue_t, *p_queue_t;

/* 队列操作函数声明 */
p_queue_t queue_create(void);
	
bool queue_destroy(p_queue_t queue);

bool queue_empty(p_queue_t queue);

void *queue_gethead(p_queue_t queue);

bool queue_put(p_queue_t queue, void *data);
/* 按数据大小插入 */
bool queue_insert_maxsize(p_queue_t queue, void *data, 
	int (*compare)(void *data1, void *data2));
/*	队列元素按从大到小排列	
 *	0  相等
 *	1  大于
 *	-1 小于
 */
bool queue_insert_minsize(p_queue_t queue, void *data,
		int (*compare)(void *data1, void *data2));

bool queue_delete(p_queue_t queue);

void queue_print(p_queue_t queue, void (*fp)(void *data));


#endif	/* __QUEUE_H__ */
