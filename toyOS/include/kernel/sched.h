#ifndef _SCHED_H
#define _SCHED_H
#include <kernel/pm.h>

#define	NR_TASKS	4

#define TASK_RUNING				0
#define TASK_INTERRUPTIBLE		1
#define TASK_UNINTERRUPTIBLE	2
#define TASK_ZOMBLE				3
#define TASK_STOPPED			4


#define LDT_SIZE	3
#define MAX_NAME	24

typedef struct {
	unsigned int back_link;
	unsigned int esp0;
	unsigned int ss0;
	unsigned int esp1;
	unsigned int ss1;
	unsigned int esp2;
	unsigned int ss2;
	unsigned int cr3;
	unsigned int eip;
	unsigned int eflags;
	unsigned int eax;
	unsigned int ecx;
	unsigned int edx;
	unsigned int ebx;
	unsigned int esp;
	unsigned int ebp;
	unsigned int esi;
	unsigned int edi;
	unsigned int es;
	unsigned int cs;
	unsigned int ss;
	unsigned int ds;
	unsigned int fs;
	unsigned int gs;
	unsigned int ldt;
	unsigned int trace_bitmap;
}TSS_STRUCT;


typedef struct {
	unsigned int gs;
	unsigned int fs;
	unsigned int es;
	unsigned int ds;
	unsigned int edi;
	unsigned int esi;
	unsigned int ebp;
	unsigned int kernel_esp;
	unsigned int ebx;
	unsigned int edx;
	unsigned int ecx;
	unsigned int eax;
	unsigned int retaddr;
	unsigned int eip;
	unsigned int cs;
	unsigned int eflags;
	unsigned int esp;
	unsigned int ss;
}REGISTER_STRUCT;

#define SENDING 0x01
#define RECVING 0x02
#define IDLE	0x03

typedef struct {
	REGISTER_STRUCT reg;
	unsigned short ldt_selector;
	SEGMENT_DESCRIPTOR ldts[LDT_SIZE];
	unsigned int pid;
	unsigned int tty_id;
	int flags;				/* 内核任务还是用户任务 */
	int status;				/* 状态，SENDING, RECVING, IDLE */
	unsigned int elapse;
	unsigned int prior;
}TASK_STRUCT;

#define TASK_STACK_SIZE	0x8000
#define KERNEL_TASK	0x00
#define USER_TASK	0X01
typedef struct {
	void  *eip;
	void  *esp;
	int flags;
}TABLE_STRUCT;



extern TABLE_STRUCT global_task_table[NR_TASKS];
extern unsigned int global_jiffies;
TASK_STRUCT global_task_list[NR_TASKS];
TASK_STRUCT *global_current;
TSS_STRUCT	global_tss;
int global_reenter;


void sechedule(void);


#endif 



















