/***************************************************
*	文件名: algorithm.h
*
*	文件名: 作业调度算法头文件
*
*	创建人: Astrol 2011-11-30 0:03:11
*
*	版本号: 1.0
***************************************************/
#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#include "jcb.h"
#include "queue.h"

/* 优先级调度算法 */
void priority_create(int num_process);
void priority();

/* 先来先服务算法 */
void fcfs_create(int num_process);
void fcfs();

/* 最短作业优先算法*/
void sjf_create(int num_process);
void sjf();

/* 响应比最高者优先算法 */
void hrrf_create(int num_process);
void hrrf();

/* 错误处理函数 */
void err_exit(char *str_err);

#endif 	/* __ALGORITHM_H__ */
