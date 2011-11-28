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
