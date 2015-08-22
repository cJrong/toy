
;必须和PM里面的INTR_SYS_CALL保持一致
%define	INTR_SYS_CALL 0x80

%define	NR_SYS_GETTICKS	0x00
%define NR_SYS_WRITE	0x01
%define NR_SYS_SENDREC	0x02
global getticks
global write


[section .data]
global global_jiffies
global_jiffies:
	dd 0

[section .text]
[bits 32]
getticks:
	mov eax, NR_SYS_GETTICKS
	int INTR_SYS_CALL
	ret 
	

write:
	mov ebx, dword [esp + 4]	;buf
	mov ecx, dword [esp + 8]	;n
	mov eax, NR_SYS_WRITE
	int INTR_SYS_CALL
	ret 

sendrec:
	mov eax, NR_SYS_SENDREC
	mov ebx, dword [esp + 4]
	mov ecx, dword [esp + 8]
	mov edx, dword [esp + 12]
	int INTR_SYS_CALL
	ret


