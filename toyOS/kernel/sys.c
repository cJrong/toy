#include <kernel/sys.h>
#include <kernel/tty.h>
#include <stdio.h>

fn_ptr global_sys_call_table [] = {
	sys_getticks, sys_write, sys_sendrec
	
};

int sys_getticks(void)
{
	return global_jiffies;
}

void sys_write(TASK_STRUCT *this, char *buf, int n)
{
	tty_write(&tty_list[this->tty_id], buf, n);
}


#define SEND_MESSAGE 0x01
#define RECV_MESSAGE 0x02

int sys_sendrec(TASK_STRUCT *task, int func, int dest, MESSAGE_STRUCT *m)
{

	switch (func)
	{
	case SEND_MESSAGE:
		msg_send(task, dest, m);
		break;
	case RECV_MESSAGE:
		msg_recv(task, dest, m);
		break;
	default:
		printf("message transfer error !\n\n");
		break;
	}
	return 0;
}


int msg_send(TASK_STRUCT  *task, int dest, MESSAGE_STRUCT *m)
{

	return 0;
}

int msg_recv(TASK_STRUCT *task, int dest, MESSAGE_STRUCT *m)
{

	return 0;	
}