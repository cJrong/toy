%include "include/kernel/asm/offs.inc"

[section .bss]
;真正的内核栈，仅有4K大小！
resb 1024 * 4
stack_top:	

[section .text]
[bits 32]
extern global_current
extern global_tss
extern global_sys_call_table

extern divide_err
extern debug
extern nmi
extern breakpoint
extern overflow
extern bounds
extern invalid_op
extern device_not_available
extern double_fault
extern coprocessor_segment_overrun
extern invalid_TSS
extern segment_not_present
extern stack_segment
extern general_protection
extern page_fault
extern coprocessor_error
extern reserved
extern parallel_interrupt
extern irq13

extern handler

global interrupt_vector_0
global interrupt_vector_1
global interrupt_vector_2
global interrupt_vector_3
global interrupt_vector_4
global interrupt_vector_5
global interrupt_vector_6
global interrupt_vector_7
global interrupt_vector_8
global interrupt_vector_9
global interrupt_vector_10
global interrupt_vector_11
global interrupt_vector_12
global interrupt_vector_13
global interrupt_vector_14
global interrupt_vector_15
global interrupt_vector_16

global interrupt_vector_32
global interrupt_vector_33
global interrupt_vector_34
global interrupt_vector_35
global interrupt_vector_36
global interrupt_vector_37
global interrupt_vector_38
global interrupt_vector_39
global interrupt_vector_40
global interrupt_vector_41
global interrupt_vector_42
global interrupt_vector_43
global interrupt_vector_44
global interrupt_vector_45
global interrupt_vector_46
global interrupt_vector_47

global sys_call
global restart

;-----------------------------------------------
; Intel 保留中断
;-----------------------------------------------
interrupt_vector_0:
	call divide_err
	jmp down 
	
interrupt_vector_1:
	call debug
	jmp down 

interrupt_vector_2:
	call nmi
	jmp down 
	
interrupt_vector_3:
	call breakpoint
	jmp down 

interrupt_vector_4:
	call overflow
	jmp down 

interrupt_vector_5:
	call bounds
	jmp down 

interrupt_vector_6:
	call invalid_op
	jmp down 
	
interrupt_vector_7:
	call device_not_available
	jmp down 
	
interrupt_vector_8:
	call double_fault
	jmp down 
	
interrupt_vector_9:
	call double_fault
	jmp down 
	
interrupt_vector_10:
	call invalid_TSS
	jmp down
	
interrupt_vector_11:
	call segment_not_present
	jmp down
	
interrupt_vector_12:
	call stack_segment
	jmp down
	
interrupt_vector_13:
	call general_protection
	jmp down
	
interrupt_vector_14:
	call page_fault
	jmp down	

interrupt_vector_15:
	call reserved
	jmp down	
	
interrupt_vector_16:
	call coprocessor_error
	jmp down	
	
;-----------------------------------------------
;	硬件中断
;-----------------------------------------------
extern pos
extern delay
extern global_reenter
extern sechedule

;时钟中断！
interrupt_vector_32:
	call save_reg

	in al, 0x21
	or al, 1
	out 0x21, al
	
	mov al, 0x20		
	out 0x20, al
	
	sti 
	call sechedule
	cli
	
	in al, 0x21
	and al, 0xfe
	out 0x21, al
	
	ret 

extern keyboard_interrupt
interrupt_vector_33:
	call save_reg
	
	in al, 0x21
	or al, 2
	out 0x21, al
	
	mov al, 0x20		
	out 0x20, al
	
	sti
	call keyboard_interrupt
	cli
	
	in al, 0x21
	and al, 0xfd
	out 0x21, al

	ret 

interrupt_vector_34:
	call handler
	iret

interrupt_vector_35:
	call handler
	iret

interrupt_vector_36:
	call handler
	iret

interrupt_vector_37:
	call handler
	iret

interrupt_vector_38:
	call handler
	iret

interrupt_vector_39:
	call handler
	iret

interrupt_vector_40:
	call handler
	iret

interrupt_vector_41:
	call handler
	iret	

interrupt_vector_42:
	call handler
	iret

interrupt_vector_43:
	call handler
	iret

interrupt_vector_44:
	call handler
	iret

interrupt_vector_45:
	call handler
	iret

interrupt_vector_46:
	call handler
	iret

interrupt_vector_47:
	call handler
	iret	
	
down:
	hlt 
	jmp down

sys_call:
	call save_reg
	sti 
	
	push ecx 
	push ebx
	push dword [global_current]
	call [global_sys_call_table + eax * 4]
	mov dword [esi + EAXREG-TASK_STRUCT], eax
	add esp, 12
	
	cli 
	
	ret 

save_reg:
	pushad
	push ds
	push es
	push fs
	push gs
	
	mov esi, esp
	;=0不重入，!=0重入
	inc dword [global_reenter]
	cmp dword [global_reenter], 0
	jne .reenter
	mov esp, stack_top
	push restart
	jmp [esi + RETADR-TASK_STRUCT]
.reenter:
	push restart_reenter
	jmp [esi + RETADR-TASK_STRUCT]
	
	
	
restart:
	mov esp, dword [global_current]
	lea eax, [esp+LDT_SELECTOR]
	mov dword [global_tss+ESP0REG], eax
restart_reenter:
	dec dword [global_reenter]
	pop gs
	pop fs
	pop es 
	pop ds
	popad 	
	add esp, 4
	iretd

	
