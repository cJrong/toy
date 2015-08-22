#ifndef _SYS_H
#define _SYS_H
#include <kernel/sched.h>

typedef void* fn_ptr;
typedef struct {
	int type;
}MESSAGE_STRUCT;

int sys_getticks(void);
void sys_write(TASK_STRUCT *this, char *buf, int n);
int sys_sendrec(TASK_STRUCT *task, int func, int dest, MESSAGE_STRUCT *m);
int msg_send(TASK_STRUCT *task, int dest, MESSAGE_STRUCT *m);
int msg_recv(TASK_STRUCT *task, int src, MESSAGE_STRUCT *m);

extern fn_ptr global_sys_call_table[];
extern unsigned int global_jiffies;

#endif 