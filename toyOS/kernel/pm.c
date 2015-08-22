#include <kernel/config.h>
#include <kernel/io.h>
#include <kernel/pm.h>
#include <kernel/traps.h>
#include <kernel/i8259a.h>
#include <string.h>
#include <kernel/sched.h>

void set_segmdesc(SEGMENT_DESCRIPTOR *sd, unsigned int base,
        unsigned int limit, unsigned short attr)
{
	sd->limit = limit & 0xffff;
	sd->base_low = base & 0xffff;
	sd->base_mid = (base>>16) & 0xff;
	sd->attr = (attr & 0xf0ff) | ((limit>>8) & 0x0f00);
	sd->base_high = (base >> 24) & 0xff;
}

void set_gatedesc(GATE_DESCRIPTOR *gd, unsigned short selector,
		unsigned int offset, unsigned char dcount, unsigned char attr)
{
	gd->offset_low = offset & 0xffff;
	gd->selector = selector & 0xffff;
	gd->count = dcount;
	gd->attr = attr;
	gd->offset_high = (offset >> 16) & 0xffff;
}

void update_segmdesc_base(unsigned short selector, unsigned int base)
{
	SEGMENT_DESCRIPTOR *gdt = (SEGMENT_DESCRIPTOR *) GDT_BASE_ADDR;
	gdt[selector>>3].base_low = base & 0xffff;
	gdt[selector>>3].base_mid = (base >> 16) & 0xff;
	gdt[selector>>3].base_high = (base >> 24) & 0xff;
}

void init_gdt(void)
{
	SEGMENT_DESCRIPTOR *gdt_base = (SEGMENT_DESCRIPTOR *) GDT_BASE_ADDR;
	int i;
	
	for (i = 0; i < 8192; i++) {
		set_segmdesc(gdt_base+i, 0, 0, 0);
	}
	set_segmdesc(gdt_base + 1, 0x00000000, 0xfffff, DESC_ATTRI_CODE32_RX | DESC_ATTRI_DPL0 | DESC_ATTRI_G);
	set_segmdesc(gdt_base + 2, 0x00000000, 0xfffff, DESC_ATTRI_DATA32_RW | DESC_ATTRI_DPL0 | DESC_ATTRI_G);
	set_segmdesc(gdt_base + 3, 0x00000000, sizeof(SEGMENT_DESCRIPTOR)*LDT_SIZE, DESC_ATTRI_LDT| DESC_ATTRI_DPL0);
	set_segmdesc(gdt_base + 4, (unsigned int)&global_tss, sizeof(global_tss)-1, DESC_ATTRI_386TSS | DESC_ATTRI_DPL0);
	
	memset(&global_tss, 0, sizeof(global_tss));
	global_tss.ss0 = KERNEL_SELECTOR_DS;
	global_tss.trace_bitmap = sizeof(global_tss);
	
	load_gdt(0xffff, GDT_BASE_ADDR);
	load_tr(TR_SELECOTR);
	
	update_selector();
}

void init_idt(void)
{
	GATE_DESCRIPTOR	*idt_base = (GATE_DESCRIPTOR *) IDT_BASE_ADDR;
	int i;
	
	for (i = 0; i < 256; i++) {
		set_gatedesc(idt_base + i, 0, (unsigned int)0, 0, 0);
	}
	load_idt(0x7ff, IDT_BASE_ADDR);
	trap_init(idt_base);
	init_8259a(idt_base);
	
	set_gatedesc(idt_base+INTR_SYS_CALL, KERNEL_SELECTOR_CS, \
	(unsigned int)sys_call, 0, DESC_ATTRI_386_INT_GATE | DESC_ATTRI_DPL3);
}


