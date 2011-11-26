#ifndef __JCB_H__
#define __JCB_H__

typedef struct time{
	float time_begin;		/* 作业开始时刻 */
	float time_end;			/* 作业完成时刻 */
	float time_submit;		/* 作业提交时间 */
	float time_run;			/* 作业执行了的时间 */
	float time_needtime;		/* 作业到完成还需要的时间 */
	float time_turnaround;		/* 作业周转时间 */
	float time_turnaround_weight; 	/* 带权周转时间 */
}worktime;
/*--------------------------------------------------------------|	
 |	作业周转时间 = 作业提交时刻 - 作业完成时刻		|
 |	time_turnaround = time_submit - time_end		|
 |--------------------------------------------------------------|
 | 	带权周转时间 = 作业周转时间 / 作业执行时间		|
 |	time_turnaround_weight = time_turnaround / time_run	| 								
 |--------------------------------------------------------------|
 |	响应比 = 作业周转时间 / 作业执行时间			|
 |	response_ratio = time_turnaround / time_run		|
 |--------------------------------------------------------------|*/

/* 定义jcb结构 */
typedef struct jcb_control_block{
	char name[10];			/* 进程名称 	*/
	int status;			/* 进程状态 	*/
	int priority;       		/* 进程优先级 	*/
	worktime time;			/* 时间集合	*/
	float response_ratio;		/* 响应比 	*/
	struct jcb_control_block *next;	/* 指针域	*/		
}jcb_t, *p_jcb_t;

/* 进程状态宏定义 */
#define TASK_STATUS_BLOCK 	0	/* 阻塞状态 */
#define TASK_STATUS_READY 	1	/* 就绪状态 */
#define TASK_STATUS_RUNNING 	2 	/* 运行状态 */
#define TASK_STATUS_FINISH	3	/* 完成状态 */

#endif 	/* __JCB_H__ */
