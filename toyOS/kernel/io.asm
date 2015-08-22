[section .text]
[bits 32]
global io_hlt
global io_cli
global io_sti
global io_in8
global io_in16
global io_in32
global io_out8
global io_out16
global io_out32
global io_delay
global io_send_EOI
global load_gdt
global load_idt
global load_ldt
global load_tr
global empty_eflags
global io_load_eflags
global io_store_eflags

	
;void io_hlt(void);
io_hlt: 	
	hlt 
	ret
	
;void io_cli(void);
io_cli:
	cli
	ret

;void io_sti(void);
io_sti:
	sti
	ret 

;vodi io_send_EOI(int port);	
io_send_EOI:
	mov edx, dword [esp + 4]
	mov al, 0x20
	out dx, al
	ret 
	
;int io_in8(int port);
io_in8:
	mov edx, dword [esp + 4]
	xor eax, eax
	in al, dx
	ret 

;int io_in16(int port);
io_in16:
	mov edx, dword [esp + 4]
	xor eax, eax 
	in ax, dx
	ret 

;int io_in32(int port);
io_in32:
	mov edx, dword [esp + 4]
	in eax, dx
	ret 

;void io_out8(int port, int data);
io_out8:
	mov edx, dword [esp + 4]
	mov al, byte [esp + 8]
	out dx, al
	ret 
	
;void io_out16(int port, int data);
io_out16:
	mov edx, dword [esp + 4]
	mov ax, word [esp + 8]
	out dx, al
	ret 

;void io_out32(int port, int data);
io_out32:
	mov edx, dword [esp + 4]
	mov eax, dword [esp + 8]
	out dx, eax 
	ret 

;void io_delay();
io_delay:
	nop
	nop
	nop
	nop
	ret
	
;void load_gdt(int limit, int addr);
load_gdt:
	mov ax, word [esp + 4] ;word[esp+4] = limit
	mov word [esp + 6], ax ;dword[esp+8] = addr
	lgdt [esp + 6]
	ret 
	
;void load_idt(int limit, int addr);
load_idt:
	mov ax, word [esp + 4]
	mov word [esp + 6], ax
	lidt [esp + 6]
	ret 

;void load_ldt(int selector);
load_ldt:
	lldt word [esp + 4]
	ret 

;void load_tr(int selector);
load_tr:
	ltr word [esp + 4]
	ret 
	
;void empty_eflags(void);
empty_eflags:
	push dword 0
	popfd
	ret
	
;int io_load_eflags(void);
io_load_eflags:
	pushfd
	pop eax
	ret 

;void io_store_eflags(int eflags);
io_store_eflags:
	mov eax, dword [esp + 4]
	push eax
	popfd
	ret 
	
%define KERNEL_SELECTOR_CS	0x0008
%define KERNEL_SELECTOR_SS	0x0010
%define KERNEL_SELECTOR_DS	0x0010
%define KERNEL_SELECTOR_ES	0x0010
%define KERNEL_SELECTOR_GS	0x0010
%define KERNEL_SELECTOR_FS	0x0010
global update_selector
update_selector:
	jmp dword KERNEL_SELECTOR_CS:.update_cs
.update_cs:
	mov ax, KERNEL_SELECTOR_DS
	mov ds, ax
	mov es, ax
	mov gs, ax
	mov fs, ax
	mov ss, ax
	ret 

global farjmp
;void farjmp(int eip, int cs);
farjmp:		
	JMP		FAR	[ESP+4]	
	RET



