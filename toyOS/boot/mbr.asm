;这个程序其实没有也无所谓的，只是为了兼容现在的操作系
;统。所以多出了一个MBR程序。另外这个MBR程序功能很简单
;就是扫描分区表，然后把活动分区的引导扇区加载到0x7c00
;处。并跳到活动分区的引导扇区处运行。这个MBR当然也写得
;很简陋。毕竟赶紧进入正题编写内核才重要。:)


%define     MBR_SEG			0x7c00			;MBR段基址
%define     MBR_TAIL_SEG	0x7e00			;MBR_TAIL段基址

%define		READ_SECT_NUMBER			1		;一次性读取扇区数
%define		MBR_TAIL_SECT_NUMBER		62		;设置MBR_TAIL共占用多少个扇区。因为当前处于实模式，所以最大扇区不应该大于128
%define		MBR_TAIL_BEGIN_SETC			2		;从第MBR_TAIL_BEGIN_SETC个扇区处读取BOOT

%define		DPT_OFFSET					0x1be	;磁盘分区表偏移地址
%define		DPT_NUMBER					0x04	;最大分区个数			

%define		BOOT_SEG		0x7c00				

	
	jmp word (MBR_SEG>>4):main
DAPACK:
DA_SIZE_PACKET:		db	0x10
					db	0x00
DA_SECT_CNT:		dw	READ_SECT_NUMBER
DA_BUF_OFF:			dw	0
DA_BUF_SEG:			dw	(MBR_TAIL_SEG>>4)
DA_READ_SECT_LBA:	dd	(MBR_TAIL_BEGIN_SETC-1)
					dd	0

main:
[bits 16]
	
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax              
    mov sp, 0xffff
	
	call clean
;检查系统是由软盘启动的还是硬盘启动的，如果是软盘启动的，
;则打印出不支持软盘，并进入死循环，没有退路！
	test dl, 0x80
	jnz .main_is_disk
	mov bp, disp_floppy_err
	mov cx, 28
	call disp_msg
	jmp $

.main_is_disk:
;检测BIOS是否支持扩展0x13号中断，如果支持，则读取扇区。如
;果不支持，则打印一串字符，并进入死循环。
	mov bx, 0x55aa
	mov dl, 0x80
	mov ah, 0x41
	int 0x13
	mov di, MBR_TAIL_SECT_NUMBER
	jc .main_disk_not_support
	
.main_read_next:
	mov si, DAPACK
	mov ah, 0x42
	mov dl, 0x80
	int 0x13	
	cmp ah, 0					
	jz .main_disk_read_ok
	mov bp, disp_disk_err
	mov cx, 24
	call disp_msg
	jmp $
.main_disk_read_ok:
	inc dword [DA_READ_SECT_LBA]
	add word [DA_BUF_OFF], 0x200
	cmp di, 0
	dec di
	jnz .main_read_next
	jmp .main_end
	
.main_disk_not_support:
	mov bp, disp_disk_err
	mov cx, 24
	call disp_msg
	jmp $

.main_end:
;引导扇区的功能到此结束，进入MBR_TAIL部分。
	jmp (MBR_TAIL_SEG>>4):0

disp_msg:
	push ax
	push bx
	push dx
	mov ax, 0x1301
	mov bx, 0x0007
	mov dx, 0x0000
	int 0x10
	pop dx
	pop bx
	pop ax
	ret

clean:
	push es
	push si
	push cx
	mov cx, 0xb800
	mov es, cx
	mov si, 0
	mov cx, 160*25
.clean_loop:
	mov word [es:si], 0
	add si, 2
	loop .clean_loop
	pop cx
	pop si
	pop es
	ret
	
		
disp_floppy_err	db	"Can't support a floopy disk!"	
disp_disk_err   db  "Unable to read from disk!"
times 510-($-$$) db 0
db 0x55, 0xaa

;-----------------------------------------------------------
;在mbr_tail程序中，不需要重新设置段寄存器，除了cs=0x7e0外
;其它段寄存器都继承自mbr程序。(因为要用到前面的一些东西)
;----------------------------------------------------------- 
mbr_tail:

;扫描主引导扇区的活动分区
	mov si, DPT_NUMBER
	mov bx, DPT_OFFSET
.mbr_tail_scan_dpt:
	mov al, [bx]
	test al, 0x80
	jnz	.mbr_tail_scan_ok
	add bx, 0x10
	cmp si, 0
	dec si
	jnz .mbr_tail_scan_dpt
	mov bp, disp_disk_err
	mov cx, 33
	mov ax, 0x1301
	mov bx, 0x0007
	mov dx, 0x0000
	int 0x10
	jmp $
	
.mbr_tail_scan_ok:
	xor edx, edx
	mov eax, dword [bx+0x08]
	push edx
	push eax
	push word (BOOT_SEG>>4)
	push word 0
	push word 1
	push word 0x0010
	mov si, sp
	mov ah, 0x42
	mov dl, 0x80
	int 0x13
	cmp ah, 0
	jnz .mbr_tail_read_err
	add sp, 0x10
	mov ax, [0x1fe]
	cmp ax, 0xaa55
	jz .mbr_tail_end
	mov bp, disp_boot_failed
	mov cx, 34
	mov ax, 0x1301
	mov bx, 0x0007
	mov dx, 0x0000
	int 0x10
	jmp $
.mbr_tail_read_err:
	mov bp, disp_par_read_err
	mov cx, 32
	mov ax, 0x1301
	mov bx, 0x0007
	mov dx, 0x0000
	int 0x10
	jmp $

.mbr_tail_end:
	jmp word (BOOT_SEG>>4):0
	
disp_no_find		db	"find the active partition error!"
disp_par_read_err	db	"reading action partition error!"
disp_boot_failed	db 	"Boot failed: not a bootable disk!"

times (512+MBR_TAIL_SECT_NUMBER*512)-($-$$) db 0

