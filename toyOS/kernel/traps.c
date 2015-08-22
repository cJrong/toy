#include <kernel/pm.h>
#include <kernel/traps.h>
#include <disp.h>

void trap_init(GATE_DESCRIPTOR	*idt_base)
{
	set_gatedesc(idt_base+0, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_0, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+1, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_1, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+2, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_2, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+3, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_3, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+4, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_4, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+5, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_5, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+6, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_6, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+7, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_7, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+8, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_8, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+9, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_9, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+10, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_10, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+11, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_11, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+12, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_12, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+13, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_13, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+14, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_14, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+15, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_15, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+16, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_16, 0, DESC_ATTRI_386_INT_GATE);
}

void divide_err(int eip, int cs, int eflags)
{
	char buf[9] = {0};
	
	hex_to_string(cs, buf);
	disp_string(buf, 0x07, 0, 0);
	hex_to_string(eip, buf);
	disp_string(buf, 0x07, 0, 10);
	hex_to_string(eflags, buf);
	disp_string(buf, 0x07, 0, 20);
	
	disp_string("divide err!", 0x07, 0, 30);
}

void debug(int eip, int cs, int eflags)
{
	char buf[9] = {0};
	
	hex_to_string(cs, buf);
	disp_string(buf, 0x07, 0, 0);
	hex_to_string(eip, buf);
	disp_string(buf, 0x07, 0, 10);
	hex_to_string(eflags, buf);
	disp_string(buf, 0x07, 0, 20);
	
	disp_string("debug!", 0x07, 0, 30);
}

void nmi(int eip, int cs, int eflags)
{
	char buf[9] = {0};
	
	hex_to_string(cs, buf);
	disp_string(buf, 0x07, 0, 0);
	hex_to_string(eip, buf);
	disp_string(buf, 0x07, 0, 10);
	hex_to_string(eflags, buf);
	disp_string(buf, 0x07, 0, 20);
	
	disp_string("nmi!", 0x07, 0, 30);
}

void breakpoint(int eip, int cs, int eflags)
{
	char buf[9] = {0};
	
	hex_to_string(cs, buf);
	disp_string(buf, 0x07, 0, 0);
	hex_to_string(eip, buf);
	disp_string(buf, 0x07, 0, 10);
	hex_to_string(eflags, buf);
	disp_string(buf, 0x07, 0, 20);
	
	disp_string("breakpoint!", 0x07, 0, 30);
}

void overflow(int eip, int cs, int eflags)
{
	char buf[9] = {0};
	
	hex_to_string(cs, buf);
	disp_string(buf, 0x07, 0, 0);
	hex_to_string(eip, buf);
	disp_string(buf, 0x07, 0, 10);
	hex_to_string(eflags, buf);
	disp_string(buf, 0x07, 0, 20);
	
	disp_string("overflow!", 0x07, 0, 30);
}

void bounds(int eip, int cs, int eflags)
{
	char buf[9] = {0};
	
	hex_to_string(cs, buf);
	disp_string(buf, 0x07, 0, 0);
	hex_to_string(eip, buf);
	disp_string(buf, 0x07, 0, 10);
	hex_to_string(eflags, buf);
	disp_string(buf, 0x07, 0, 20);
	
	disp_string("bounds error!", 0x07, 0, 30);
}

void invalid_op(int eip, int cs, int eflags)
{
	char buf[9] = {0};
	
	hex_to_string(cs, buf);
	disp_string(buf, 0x07, 0, 0);
	hex_to_string(eip, buf);
	disp_string(buf, 0x07, 0, 10);
	hex_to_string(eflags, buf);
	disp_string(buf, 0x07, 0, 20);
	
	disp_string("invalid op!", 0x07, 0, 30);
}

void device_not_available(int eip, int cs, int eflags)
{
	char buf[9] = {0};
	
	hex_to_string(cs, buf);
	disp_string(buf, 0x07, 0, 0);
	hex_to_string(eip, buf);
	disp_string(buf, 0x07, 0, 10);
	hex_to_string(eflags, buf);
	disp_string(buf, 0x07, 0, 20);
	
	disp_string("device not available!", 0x07, 0, 30);
}

void double_fault(int err_code, int eip, int cs, int eflags)
{
	char buf[9] = {0};
	
	hex_to_string(cs, buf);
	disp_string(buf, 0x07, 0, 0);
	hex_to_string(eip, buf);
	disp_string(buf, 0x07, 0, 10);
	hex_to_string(eflags, buf);
	disp_string(buf, 0x07, 0, 20);
	
	disp_string("double fault!", 0x07, 0, 30);
}

void coprocessor_segment_overrun(int eip, int cs, int eflags)
{
	char buf[9] = {0};
	
	hex_to_string(cs, buf);
	disp_string(buf, 0x07, 0, 0);
	hex_to_string(eip, buf);
	disp_string(buf, 0x07, 0, 10);
	hex_to_string(eflags, buf);
	disp_string(buf, 0x07, 0, 20);
	
	disp_string("coprocessor segment overrun!", 0x07, 0, 30);
}

void invalid_TSS(int err_code, int eip, int cs, int eflags)
{
	char buf[9] = {0};
	
	hex_to_string(cs, buf);
	disp_string(buf, 0x07, 0, 0);
	hex_to_string(eip, buf);
	disp_string(buf, 0x07, 0, 10);
	hex_to_string(eflags, buf);
	disp_string(buf, 0x07, 0, 20);
	
	disp_string("invalid TSS!", 0x07, 0, 30);
}

void segment_not_present(int err_code, int eip, int cs, int eflags)
{
	char buf[9] = {0};
	
	hex_to_string(cs, buf);
	disp_string(buf, 0x07, 0, 0);
	hex_to_string(eip, buf);
	disp_string(buf, 0x07, 0, 10);
	hex_to_string(eflags, buf);
	disp_string(buf, 0x07, 0, 20);
	
	disp_string("segment not present!", 0x07, 0, 30);
}

void stack_segment(int err_code, int eip, int cs, int eflags)
{
	char buf[9] = {0};
	
	hex_to_string(cs, buf);
	disp_string(buf, 0x07, 0, 0);
	hex_to_string(eip, buf);
	disp_string(buf, 0x07, 0, 10);
	hex_to_string(eflags, buf);
	disp_string(buf, 0x07, 0, 20);
	
	disp_string("stack segment!", 0x07, 0, 30);
}

void general_protection(int err_code, int eip, int cs, int eflags)
{
	char buf[9] = {0};
	
	hex_to_string(cs, buf);
	disp_string(buf, 0x07, 0, 0);
	hex_to_string(eip, buf);
	disp_string(buf, 0x07, 0, 10);
	hex_to_string(eflags, buf);
	disp_string(buf, 0x07, 0, 20);
	disp_string("general protection!", 0x07, 0, 30);
}

void page_fault(int err_code, int eip, int cs, int eflags)
{
	char buf[9] = {0};
	
	hex_to_string(cs, buf);
	disp_string(buf, 0x07, 0, 0);
	hex_to_string(eip, buf);
	disp_string(buf, 0x07, 0, 10);
	hex_to_string(eflags, buf);
	disp_string(buf, 0x07, 0, 20);
	
	disp_string("page fault", 0x07, 0, 30);
}

void reserved(int err_code, int eip, int cs, int eflags)
{
	char buf[9] = {0};
	
	hex_to_string(cs, buf);
	disp_string(buf, 0x07, 0, 0);
	hex_to_string(eip, buf);
	disp_string(buf, 0x07, 0, 10);
	hex_to_string(eflags, buf);
	disp_string(buf, 0x07, 0, 20);
	
	disp_string("reserved", 0x07, 0, 30);
}

void coprocessor_error(int eip, int cs, int eflags)
{
	char buf[9] = {0};
	
	hex_to_string(cs, buf);
	disp_string(buf, 0x07, 0, 0);
	hex_to_string(eip, buf);
	disp_string(buf, 0x07, 0, 10);
	hex_to_string(eflags, buf);
	disp_string(buf, 0x07, 0, 20);
	
	disp_string("coprocessor error", 0x07, 0, 30);
}
