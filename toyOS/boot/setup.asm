


;	ds = 0x7c0
;	ss = 0x7c0, sp = 0x800
;	cs = 0x8000
	%include "config.inc"
	%include "pm.inc"
	
	
main:
[bits 16]
;获取内存信息，保存到PARAMSEG:MEMMAP_OFFSET中
	call get_memmap
	
	
;加载SYSTEM模块到SYSSEG处.

	mov edx, dword [BPB_RootClus]
.check_next_cluster:
	call get_culster_section	
	push edx
	
	xor cx, cx
	mov cl, byte [BPB_SecPerClus]
.check_root_next_sector:
;读取根目录的一个扇区到缓冲区
	push eax
	push cx
	xor edx, edx
	add eax, VOLUME
	mov bx, BUFFER
	mov es, bx
	mov di, 0
	mov bx, 1
	call read_sector_ex
	
	mov si, system_filename
.cmp_next:	
;比较文件名是否正确
	push ds
	mov ax, SETUPSEG
	mov ds, ax
	mov cx, 11
	call strncmp
	cmp eax, 0
	pop ds
	jz .defferent_filename
;查找文件成功，取出文件的开始簇号	
;并且保存文件的大小
	xor edx, edx
	mov dx, word [es:di+0x14]
	shl edx, 16
	mov dx, word [es:di+0x1a]
	mov ecx, dword [es:di+0x1c]
	jmp .search_file_ok
.defferent_filename:
	cmp di, 0x01e0
	jz .reset
	add di, 0x20
	jmp .cmp_next
.reset:
	pop cx
	pop eax
	inc eax
	loop .check_root_next_sector

;读取根目录的下一簇，如果有的话!
	pop eax
	call get_current_cluster_next
	cmp eax, 0x0fffffff
	jz .non_find
	mov edx, eax 
	jmp .check_next_cluster
.non_find:
	mov ax, SETUPSEG
	mov es, ax
	mov cx, 10
	mov bx, 0x0007
	mov dx, 0x0000
	mov bp, non_system
	mov ax, 0x1301
	int 0x10
	jmp $
.search_file_ok:

;现在的硬盘读取速度较快，就不做系统读取的信息显示了	
	mov bx, SYSSEG
	mov es, bx
	mov di, 0	
	xor bx, bx
	mov bl, byte [BPB_SecPerClus]
	call read_file

;将system模块移动到0x0000绝对位置
	mov ax, 0
	mov es, ax
	mov di, 0
	mov ax, SYSSEG
	mov ds, ax
	mov si, 0
	rep movsb
	
;现在开始做最后的善后工作，进入保护模式！
;然后就可以告别汇编，使用C或者C++了。太幸福了！
	
	mov ax, SETUPSEG
	mov ds, ax
	
	lgdt [gdtptr]
	
	cli 
	
	in al, 92h
	or al, 0000_0010b
	out 92h, al
	
	mov eax, cr0
	or eax, 0x00000001
	mov cr0, eax 
	
	jmp dword CODESEG:((SETUPSEG<<4)+protected)
	
protected:
[bits 32]	
	mov ax, DATASEG
	mov ds, ax
	mov es, ax
	mov gs, ax
	mov fs, ax
	mov ss, ax
;设置临时堆栈!
	mov esp, 0x90000

;好了，大功告成！进入system模块。
	jmp dword CODESEG:0

;--------------------------------------
;	获取计算机的内存地图映像
;	返回:
;		di	结尾偏移量
;--------------------------------------	
get_memmap:
[bits 16]	
	push ecx 
	push edx 
	push ebx 
	
;	push di 
	push es
	
	mov ax, PARAMSEG
	mov es, ax
	mov ebx, 0
	mov di, MEMMAP_OFFSET
.next_in:
	mov eax, 0xe820
	mov ecx, 20
	mov edx, 0x534d4150
	int 0x15
	jc .get_memmap_err
	add di, 20
	inc word[es:0]
	cmp ebx, 0
	jnz .next_in
	jmp .get_memmap_ok
.get_memmap_err:
	mov word [es:0], 0
.get_memmap_ok:

	pop es
;	pop di
	pop ebx
	pop edx 
	pop ecx
	ret 
	
;---------------------------------------
;获取数据的开始扇区
;	返回：eax(数据区的开始扇区号)
;---------------------------------------
get_data_section:
[bits 16]
	push edx
	
	xor edx, edx 
	mov dx, word [BPB_NumFATs]
	mov eax, dword [BPB_FATSz32]
	mul edx
	
	xor edx, edx
	mov dx, word [BPB_ResvdSecCnt]
	add eax, edx
	
	pop edx
	ret 
	
;---------------------------------------
;获取簇号的开始扇区
;	输入: edx(簇号)
;	返回：eax(簇的开始扇区号)
;---------------------------------------
get_culster_section:
[bits 16]
	push edx

	call get_data_section
	push eax
	sub edx, 2
	mov eax, edx
	xor edx, edx
	mov dl, byte [BPB_SecPerClus]
	mul edx
	mov edx, eax
	pop eax
	add eax, edx

	pop edx
	ret
	
;---------------------------------------
;取得当前簇的下一簇号
;	输入：eax
;	输出：eax
;---------------------------------------
get_current_cluster_next:
[bits 16]
	push ebx
	push edx
	push di
	push es

;eax = edx:eax / 128	
;edx = edx:eax % 128
	xor edx, edx
	mov ebx, 128
	div ebx

	push edx 
	
	mov bx, word [BPB_ResvdSecCnt]
	add eax, ebx
	add eax, VOLUME
	xor edx, edx	
	mov bx, BUFFER
	mov es, bx
	mov bx, 1
	mov di, 0x00
	call read_sector_ex

	pop edx
	mov eax, 4
	mul edx
	mov di, ax
	mov eax, dword [es:di]

	pop es
	pop di
	pop edx
	pop ebx
	ret 

;----------------------------------------
;读取一个文件
;	输入:
;		edx：文件的开始簇号
;		bx：每簇扇区数
;		es:di：缓冲区
;----------------------------------------
read_file:
[bits 16]
	push ecx
	push eax
	
.read_next_cluster:
	mov ecx, edx
	call get_culster_section
	add eax, VOLUME
	xor edx, edx
	call read_sector_ex

	mov eax, ecx
	call get_current_cluster_next
	cmp eax, 0x0fffffff
	jz .read_file_ok
	mov edx, eax
	
	push edx 
	xor ax, ax
	mov ax, 0x200
	mul bx
	add di, ax
	pop edx
	
	jmp .read_next_cluster
.read_file_ok:
	
	pop eax
	pop ecx
	ret
	
;----------------------------------------
;使用扩展int 0x13从磁盘读取一个扇区
;	输入：
;		es:di: 缓冲区
;		bx:	一次性读取多少扇区
;		edx:eax: 读取的扇区号
;	返回:
;		eax(返回0无错误，返回1读取失败)
;----------------------------------------
read_sector_ex:
[bits 16]
	push edx
	push eax
	push es
	push di
	push bx
	push 0x0010
	
	mov si, sp
	mov ah, 0x42
	mov dl, 0x80
	int 0x13
	setc al
	movzx eax, al
	add sp, 0x10
	
	ret
	
;-----------------------------------------
;测试BIOS是否支持使用扩展int 0x13
;	返回：eax (返回0支持，返回1不支持)
;-----------------------------------------
;check_int13h_ex:
;	push bx
;	push dx
;	
;	mov bx, 0x55aa
;	mov dl, 0x80
;	mov ah, 0x41
;	int 0x13
;	setc al
;	movzx eax, al
;	
;	pop dx
;	pop bx
;	ret

;-----------------------------------------
;比较两个字符串
;	输入：
;		es:di	string1
;		ds:si	string2
;		cx		长度
;	返回：
;		eax:	为0比较失败，为1比较成功
;-----------------------------------------
strncmp:
[bits 16]
	push di
	push si
	
	cld 
.cmp_again:
	lodsb 
	cmp al, byte [es:di]
	jnz .cmp_err
	inc di
	loop .cmp_again
	mov eax, 1
	jmp .cmp_ok
.cmp_err:
	mov eax, 0
.cmp_ok:	
	pop si
	pop di
	ret 
	
system_filename	db	"SYSTEM     "
non_system	db	"No system!"

gdt:
	DESCRIPTOR		0,		0,		0
	DESCRIPTOR		0,	0xfffff,	DESC_ATTRI_CODE32_RX | DESC_ATTRI_DPL0 | DESC_ATTRI_G
	DESCRIPTOR		0,	0xfffff,	DESC_ATTRI_DATA32_RW | DESC_ATTRI_DPL0 | DESC_ATTRI_G
gdtlen		equ			$-gdt
gdtptr:		GDTPTR		(SETUPSEG<<4)+gdt, gdtlen

CODESEG		equ			SELECTOR_GDT(1, 0)
DATASEG		equ			SELECTOR_GDT(2, 0)
