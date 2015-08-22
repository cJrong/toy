#include <kernel/pm.h>
#include <kernel/sched.h>

char stacka[0x8000] = {0};
char stackb[0x8000] = {0};
char stackc[0x8000] = {0};
extern void testa(void);
extern void testb(void);
extern void testc(void);

char tty1_stack[0x8000] = {0};
extern void tty_task(void);
TABLE_STRUCT global_task_table[NR_TASKS] = { 
											
											 {testa, &stacka+1, USER_TASK},	\
											 {testb, &stackb+1, USER_TASK},	\
											 {testc, &stackc+1, USER_TASK},	\
											 {tty_task, &tty1_stack+1, KERNEL_TASK}, \
										   };									   
void sechedule(void)
{
	TASK_STRUCT *traversal;
	
	global_jiffies++;

	if (global_reenter != 0) {
		return ;
	}
	
	global_current++;
	if (global_current > &global_task_list[NR_TASKS-1]) {
		global_current = global_task_list;
	}
	
	update_segmdesc_base(global_current->ldt_selector, \
						(unsigned int)&global_current->ldts[0]);
	load_ldt(global_current->ldt_selector);
}


void init_task(void)
{
	TASK_STRUCT *ptask = global_task_list;
	TABLE_STRUCT *ptask_table = global_task_table;
	
	int i;
	
	for (i = 0; i < NR_TASKS; i++) {
		ptask->pid = i;
		memset(&ptask->reg, 0, sizeof(REGISTER_STRUCT));
		ptask->ldt_selector = LDT_SELECOTR;
		
		switch (ptask_table->flags) {
		case KERNEL_TASK:
			set_segmdesc(&ptask->ldts[0], 0, 0, 0);
			set_segmdesc(&ptask->ldts[1], 0x00000000, 0xfffff, DESC_ATTRI_CODE32_RC | DESC_ATTRI_DPL1 | DESC_ATTRI_G);
			set_segmdesc(&ptask->ldts[2], 0x00000000, 0xfffff, DESC_ATTRI_DATA32_RW | DESC_ATTRI_DPL1 | DESC_ATTRI_G);
			ptask->ldt_selector = LDT_SELECOTR;
			
			ptask->reg.cs = KERNEL_TASK_SELECTOR_CS;
			ptask->reg.ds = KERNEL_TASK_SELECTOR_DS;
			ptask->reg.es = KERNEL_TASK_SELECTOR_DS;
			ptask->reg.fs = KERNEL_TASK_SELECTOR_DS;
			ptask->reg.gs = KERNEL_TASK_SELECTOR_DS;
			ptask->reg.ss = KERNEL_TASK_SELECTOR_DS;
			ptask->reg.eip = (unsigned int)ptask_table->eip;
			ptask->reg.esp = (unsigned int)ptask_table->esp;
			ptask->reg.eflags = 0x1202;	/* IF = 1, IOPL = 0 */
			break;
		case USER_TASK:
			set_segmdesc(&ptask->ldts[0], 0, 0, 0);
			set_segmdesc(&ptask->ldts[1], 0x00000000, 0xfffff, DESC_ATTRI_CODE32_RC | DESC_ATTRI_DPL3 | DESC_ATTRI_G);
			set_segmdesc(&ptask->ldts[2], 0x00000000, 0xfffff, DESC_ATTRI_DATA32_RW | DESC_ATTRI_DPL3 | DESC_ATTRI_G);
		
			ptask->reg.cs = USER_TASK_SELECTOR_CS;
			ptask->reg.ds = USER_TASK_SELECTOR_DS;
			ptask->reg.es = USER_TASK_SELECTOR_DS;
			ptask->reg.fs = USER_TASK_SELECTOR_DS;
			ptask->reg.gs = USER_TASK_SELECTOR_DS;
			ptask->reg.ss = USER_TASK_SELECTOR_DS;
			ptask->reg.eip = (unsigned int)ptask_table->eip;
			ptask->reg.esp = (unsigned int)ptask_table->esp;
			ptask->reg.eflags = 0x202;	/* IF = 1, IOPL = 0 */
			break;
		}
		ptask->tty_id = i % 3;
		ptask++;
		ptask_table++;
	}
	
	ptask = global_task_list;
	update_segmdesc_base(ptask->ldt_selector, (unsigned int)&ptask->ldts[0]);
	load_ldt(ptask->ldt_selector);
	global_reenter = 0;
	global_current = ptask;
	restart();
}
