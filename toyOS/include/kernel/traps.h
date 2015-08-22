#ifndef _TRAP_H
#define _TRAP_H


void trap_init(GATE_DESCRIPTOR	*idt_base);

extern void interrupt_vector_0(void);
extern void interrupt_vector_1(void);
extern void interrupt_vector_2(void);
extern void interrupt_vector_3(void);
extern void interrupt_vector_4(void);
extern void interrupt_vector_5(void);
extern void interrupt_vector_6(void);
extern void interrupt_vector_7(void);
extern void interrupt_vector_8(void);
extern void interrupt_vector_9(void);
extern void interrupt_vector_10(void);
extern void interrupt_vector_11(void);
extern void interrupt_vector_12(void);
extern void interrupt_vector_13(void);
extern void interrupt_vector_14(void);
extern void interrupt_vector_15(void);
extern void interrupt_vector_16(void);

void divide_err(int eip, int cs, int eflags);
void debug(int eip, int cs, int eflags);
void nmi(int eip, int cs, int eflags);
void breakpoint(int eip, int cs, int eflags);
void overflow(int eip, int cs, int eflags);
void bounds(int eip, int cs, int eflags);
void invalid_op(int eip, int cs, int eflags);
void device_not_available(int eip, int cs, int eflags);
void double_fault(int err_code, int eip, int cs, int eflags);
void coprocessor_segment_overrun(int eip, int cs, int eflags);
void invalid_TSS(int err_code, int eip, int cs, int eflags);
void segment_not_present(int err_code, int eip, int cs, int eflags);
void stack_segment(int err_code, int eip, int cs, int eflags);
void general_protection(int err_code, int eip, int cs, int eflags);
void page_fault(int err_code, int eip, int cs, int eflags);
void reserved(int err_code, int eip, int cs, int eflags);
void coprocessor_error(int eip, int cs, int eflags);


#endif 