#ifndef _TTY_H
#define _TTY_H

#define INPUT_BUFFER_MAX

typedef struct {
	unsigned int front;
	unsigned int rear;
	unsigned int count;
	char buf[INPUT_BUFFER_MAX];
}INPUT_QUEUE;

typedef struct {
	unsigned int start_addr;	/* TTY对应的显存基址 */
	unsigned int current_addr;	/* 当前TTY显存开始地址 */
	unsigned int end_addr;		/* 当前TTY显存结束地址 */
	unsigned int pointer_addr;	/* 当前显存偏移 */
	
	unsigned int char_pos;
	unsigned int cursor_pos;
	unsigned int activity;		
	INPUT_QUEUE input;
}TTY_STRUCT;

extern TTY_STRUCT tty_list[];
void tty_write(TTY_STRUCT *tty, char *buf, int n);


#endif 