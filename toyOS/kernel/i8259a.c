#include <kernel/pm.h>
#include <kernel/i8259a.h>
#include <disp.h>


void init_8259a(GATE_DESCRIPTOR * idt_base)
{
	irq_init(idt_base);
	
	io_out8(0x20, 0x11);
	io_delay();
	io_out8(0xa0, 0x11);
	io_delay();

/* 设置外部中断为0x20 ~ 0x2f */
	io_out8(0x21, 0x20);
	io_delay();
	io_out8(0xa1, 0x28);
	io_delay();
	
	io_out8(0x21, 0x04);
	io_delay();
	io_out8(0xa1, 0x02);
	io_delay();
	
	io_out8(0x21, 0x01);
	io_delay();
	io_out8(0xa1, 0x01);
	io_delay();

/* 关闭所有硬件中断 */
	io_out8(0x21, 0xfc);
	io_delay();
	io_out8(0xa1, 0xff);
	io_delay();
}

static void irq_init(GATE_DESCRIPTOR *idt_base)
{
	set_gatedesc(idt_base+32, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_32, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+33, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_33, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+34, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_34, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+35, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_35, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+36, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_36, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+37, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_37, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+38, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_38, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+39, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_39, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+40, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_40, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+41, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_41, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+42, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_42, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+43, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_43, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+44, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_44, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+45, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_45, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+46, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_46, 0, DESC_ATTRI_386_INT_GATE);
	set_gatedesc(idt_base+47, KERNEL_SELECTOR_CS, (unsigned int)interrupt_vector_47, 0, DESC_ATTRI_386_INT_GATE);
}

void init_pit(void)
{
	io_out8(0x43, 0x34);
	io_out8(0x40, LATCH & 0xff);
	io_out8(0x40, LATCH >> 8);
}

#include <kernel/sched.h>
void handler(int para)
{

	io_send_EOI(0x20);
/*	io_send_EOI(0xa0); */
}
