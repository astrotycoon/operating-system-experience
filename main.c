#include "jcb.h"
#include "queue.h"
#include "algorithm.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

//p_queue_t ready = NULL;	/* 就绪队列 */
//p_queue_t run = NULL;	/* 运行队列 */
//p_queue_t finish = NULL;/* 完成队列 */

void menu_show();
void choice(int num_process);

int main(int argc, char *argv[])	/* atoi(argv[1]) 为作业数量 */
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: [%s] + num_process\n", argv[1]);
	}

	menu_show();
	choice(atoi(argv[1]));
	return EXIT_SUCCESS;
}

void menu_show()
{
	printf("1. 优先级调度算法\n");
	printf("0. 推出程序\n");
}

void choice(int num_process)
{
	int choice;
	printf("请选择:");
	scanf("%d", &choice);
	switch(choice)
	{
		case 1:
			priority_create(num_process);
			priority();
			break;		

		default:
			break;
	}

}
