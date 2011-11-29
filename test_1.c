#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include "queue.h"
int compare(void *data1, void *data2);
void print(void *data);
int main(int argc, char *argv[])
{
	int *p = (int *)malloc(sizeof(int) * 10);

	p_queue_t test = NULL;
	int i;
	test = queue_create();
	
	for (i = 0; i < 10; i++)
	{
		p[i] = i % 3;
	}
	printf("test->size = %d\n", test->size);

	for (i = 0; i < 10; i++)
	{
		queue_insert_maxsize(test, p+i, compare);
	}
	printf("\n%4d\n", *((int *)test->head->data));
//	printf("%4d\n", *((int *)test->head->next->data));
	queue_print(test, print);
	putchar('\n');
	printf("test->size = %d\n", test->size);
		
	for (i = 0; i < 10; i++)
	{
		queue_insert_maxsize(test, p+i, compare);
	}
	printf("test->size = %d\n", test->size);
	for (i = 0; i < 10; i++)
	{
		queue_insert_maxsize(test, p+i, compare);
	}
	printf("test->size = %d\n", test->size);
	queue_print(test, print);
	putchar('\n');
	int a = 100;
	queue_insert_maxsize(test, &a, compare);
	queue_print(test, print);
	putchar('\n');

	int b = 3;
	queue_insert_maxsize(test, &b, compare);
	queue_print(test, print);
	putchar('\n');

	p_queue_t queue_test = NULL;
	queue_test = queue_create();
	
	for (i = 0; i < 10; i++)
	{
		queue_insert_minsize(queue_test, p+i, compare);
	}
	queue_print(queue_test, print);
	putchar('\n');

	for (i = 0; i < 10; i++)
	{
		queue_insert_minsize(queue_test, p+i, compare);
	}
	queue_print(queue_test, print);
	putchar('\n');
	return 0;
}

int compare(void *data1, void *data2)
{
	int *p = (int *)data1;
	int *q = (int *)data2;

	if ((*p - *q) > 0)
		return 1;
	else if ((*p - *q) == 0)
		return 0;
	else
		return -1;
}

void print(void *data)
{
	printf("%4d", *((int *)data));
}
