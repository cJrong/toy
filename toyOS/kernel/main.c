#include <kernel/pm.h>
#include <kernel/sched.h>
#include <kernel/i8259a.h>
#include <kernel/io.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>



void testa(void);
void testb(void);
void init_task(void);
void delay(int time);
extern void restart(void);

int main (void)
{
	clear();
	init_gdt();
	init_idt();
	init_pit();
	init_keybord();
	init_tty();
	
	init_task();	
	
	
    while (1)io_hlt();
    return 0;
}

#include <kernel/sys.h>
void testa(void)
{	
	int i = 0; 
	char buf[9] = {0};
	
	/*for (; i < 10000; i++)	{
		printk("%d", '\n');
		delay(1);
	}*/
	
	while(1);
}

void testb(void)
{	
	int i = 0; 
/*
	for (; i < 10000; i++)	{
			write("B", 1);
		delay(1);
	}*/
	
	while(1);
}

void testc(void)
{	
	int i = 0; 
	/*
	for (; i < 10000; i++)	{
		write("C", 1);
	
		delay(1);
	}*/
	
	while(1);
}

void delay(int time)
{
	int i, j, k;
	for (k = 0; k < time; k++)
		for (i = 0; i < 10; i++)
			for (j = 0; j < 10000; j++);
}