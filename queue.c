/***********************************************************
*	文件名: queue.c
*
*	文件描述: 链队列的函数实现
*
*	创建人: Astrol 2011-11-24 10:23:52
*
*	版本号: 1.0
*
*	修改记录:
***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "queue.h"

static p_q_node_t node_create(void *data)
{
	p_q_node_t new_node = NULL;
	new_node = (p_q_node_t)malloc(sizeof(q_node_t));
	if (NULL == new_node)
	{
		errno = ENOMEM;
		return NULL;
	}
	new_node->data = data;
	new_node->next = NULL;
	return new_node;
}
p_queue_t queue_create(void)
{
	p_queue_t queue = NULL;

	queue = (p_queue_t)malloc(sizeof(queue_t));
	if (NULL == queue)
	{
		errno = ENOMEM;
		return NULL;
	}	
	queue->head = (p_q_node_t)malloc(sizeof(q_node_t));
	if (NULL == queue->head)
	{
		free(queue);
		errno = ENOMEM;
		return NULL;
	}
	queue->size = 0;
	queue->head->data = NULL;
	queue->head->next = NULL;
	queue->tail = queue->head;
	
	return queue;
}

bool queue_destroy(p_queue_t queue)
{
	if (NULL == queue)
	{
		errno = EINVAL;
		return false;
	}	

	size_t count;

	for (count = queue->size; count >= 1; count--)
	{
		queue_delete(queue);
	}
	free(queue);	

	return true;
}

bool queue_empty(p_queue_t queue)
{
	if (NULL == queue)
	{
		errno = EINVAL;
		return false;
	}

	return (queue->size == 0);
}

void *queue_gethead(p_queue_t queue)
{
	if (NULL == queue || 0 == queue->size)
	{
		errno = EINVAL;
		return NULL;
	}

	return queue->head->data;
}

bool queue_put(p_queue_t queue, void *data)
{
	if (NULL == queue)
	{
		errno = EINVAL;
		return false;
	}
	if (queue->size == 0)
	{
		queue->head->data = data;
		queue->size++;
		return true;
	}

	p_q_node_t new_node = NULL;
	new_node = (p_q_node_t)malloc(sizeof(q_node_t));
	if (NULL == new_node)
	{
		errno = ENOMEM;
		return false;
	}
	new_node->data = data;
	new_node->next = NULL;

	queue->tail->next = new_node;
	queue->tail = new_node;
	queue->size++;
	
	return true;
}

bool queue_insert_maxsize(p_queue_t queue, void *data, 
	int (*compare)(void *data1, void *data2))
{
	if (NULL == queue)
	{
		errno = EINVAL;
		return false;
	}

	if (queue->size == 0)
	{
		queue->head->data = data;
		queue->size++;
		return true;

	}

	p_q_node_t new_node = NULL;
	new_node = node_create(data);
	if (NULL == new_node)
	{
		errno = ENOMEM;
		return false;
	}

	p_q_node_t head = queue->head;	/* head指向第一个结点 */
	p_q_node_t temp = NULL;
	while (head != NULL)
	{
		if (1 == compare(head->data, data))	/* 大于 */
		{
			temp = head;
			head = head->next;
		}
		else
		{
			if (head == queue->head)
			{
				new_node->next = queue->head;
				queue->head = new_node;
				queue->size++;
				break;				
			}
			else
			{
				temp->next = new_node;		
				new_node->next = head;
				queue->size++;
				break;
			}
		}
	}
	
	return true;
}

bool queue_insert_minsize(p_queue_t queue, void *data, 
		int (*compare)(void *data1, void *data2))
{
	if (NULL == queue)
	{
		errno = EINVAL;
		return false;
	}

	if (queue->size == 0)
	{
		queue->head->data = data;
		queue->size++;
		return true;
	}

	p_q_node_t new_node = NULL;
	new_node = node_create(data);
	if (NULL == new_node)
	{
		errno = ENOMEM;
		return false;
	}	
	
	p_q_node_t head = queue->head;
	p_q_node_t temp = NULL;
	while (head != NULL)
	{
		if (-1 == compare(head->data, data))
		{
			temp = head;
			head = head->next;
		}
		else
		{
			if (head == queue->head)
			{
				new_node->next = queue->head;
				queue->head = new_node;
				queue->size++;
				break;
			}
			else
			{
				temp->next = new_node;
				new_node->next = head;
				queue->size++;
				break;
			}
		}
	}
	return true;
}
bool queue_delete(p_queue_t queue)
{
	if (NULL == queue)
	{
		errno = EINVAL;
		return false;
	}

	p_q_node_t head = queue->head;	/* head指向队列头结点 */
	
	queue->head = queue->head->next;
	free(head->data);
	free(head);
	queue->size--;

	return true;
}

void queue_print(p_queue_t queue, void (*fp)(void *data))
{
	if (NULL == queue || 0 == queue->size)
	{
		errno = EINVAL;
		return;
	}

	p_q_node_t head = queue->head;
	while (head)
	{
		(*fp)(head->data);
		head = head->next;
	}
}
