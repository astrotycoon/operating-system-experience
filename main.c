/*********************************************************
*	文件名: mian.c
*
*	文件描述: 调度算法
*
*	创建人: Astrol 2011-11-29 23:41:58
*
*	版本号: 1.0
********************************************************/
#include "jcb.h"
#include "queue.h"
#include "algorithm.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

void menu_show();

int main(int argc, char *argv[])
{
	int choice;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: [%s] + num_process\n", argv[0]);
	}

	while (true)
	{
		menu_show();
		scanf("%d", &choice);

		switch(choice)
		{
			case 1:
				priority_create(atoi(argv[1]));
				priority();
				break;
			case 2:
				fcfs_create(atoi(argv[1]));
				fcfs();
				break;
			case 3:
				sjf_create(atoi(argv[1]));
				sjf();
				break;
			case 0:
				printf("退出程序...\n");
				break;
			default:
				printf("选择错误，请重新选择!\n");
				break;
		}	
		if (choice == 0)
			break;
	}
	return EXIT_SUCCESS;
}

void menu_show()
{
	printf("1. 优先级调度算法\n");
	printf("2. 先来先服务算法\n");
	printf("3. 最短作业优先算法\n");
	printf("0. 退出程序\n");
	printf("请选择:");
}
