#ifndef _I8259A_H
#define _I8259A_H

#define HZ 100
#define LATCH (1193180/HZ)

void init_8259a(GATE_DESCRIPTOR * idt_base);
static void irq_init(GATE_DESCRIPTOR *idt_base);
void init_pit(void);

void handler(int para);

extern void interrupt_vector_32(void);
extern void interrupt_vector_33(void);
extern void interrupt_vector_34(void);
extern void interrupt_vector_35(void);
extern void interrupt_vector_36(void);
extern void interrupt_vector_37(void);
extern void interrupt_vector_38(void);
extern void interrupt_vector_39(void);
extern void interrupt_vector_40(void);
extern void interrupt_vector_41(void);
extern void interrupt_vector_42(void);
extern void interrupt_vector_43(void);
extern void interrupt_vector_44(void);
extern void interrupt_vector_45(void);
extern void interrupt_vector_46(void);
extern void interrupt_vector_47(void);

#endif 