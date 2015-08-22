	%include "config.inc"
[bits 16]
    jmp short main
	nop 

	db	"Neur    "
	dw	0x0200			;每扇区的字节数
	db	0x10			;每簇的扇区数
	dw	0x0022			;保留扇区的数目
	db	0x02			;分区中FAT表的份数
	dw	0x0000			;此项在FAT32中必须置为0
	dw	0x0000			;此项在FAT32中必须置为0
	db	0xf8			;存储介质类型
	dw	0x0000			;此项在FAT32中必须置为0
	dw	0x003f			;每磁道扇区数
	dw	0x00ff			;磁头数
	dd	0x0000003f		;隐藏的扇区数
	dd	0x01f3d2f8		;该卷总扇区数

	dd	0x00003e6b		;一个FAT表所占的扇区数
	dw	0x0000			;标志
	dw	0x0000			;FAT32的版本号
	dd	0x00000002		;根目录所在的第一个簇的簇号
	dw	0x0001			;保留区中FAT32卷FSINFO结构所占扇区数
	dw	0x0006			;保留区中引导记录的备份数据所占的扇区数
	db	0,0,0,0,0,0,0,0,0,0,0,0	;此项在FAT32中必须置为0
	db	0x80			;0x80为磁盘，0x00为软盘
	db	0x00			;保留
	db	0x29			;卷标序列号
	dd	0x00032452		;扩展引导标记
	db	"NO NAME    "	;磁盘卷标
	db	"FAT32   "		;未知，通常定义为FAT32
	
main:
	mov ax, BOOTSEG
	mov ds, ax
	mov ss, ax
	mov sp, 0x800
	
;eax = get_culster_section(BPB_RootClus)	
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
	
	mov si, setup_filename
.cmp_next:	
;比较文件名是否正确
	mov cx, 11
	call strncmp
	cmp eax, 0
	jz .defferent_filename
;查找文件成功，取出文件的开始簇号	
	xor edx, edx
	mov dx, word [es:di+0x14]
	shl edx, 16
	mov dx, word [es:di+0x1a]
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
;如果啥都找不到，进入死循环！
;没有多余的善后处理，512 bytes
;的限制太大了！。
	jmp $
	
.search_file_ok:

	mov bx, SETUPSEG
	mov es, bx
	mov di, 0	
	xor bx, bx
	mov bl, byte [BPB_SecPerClus]
	call read_file

	jmp word SETUPSEG:0
	
;---------------------------------------
;获取数据的开始扇区
;	返回：eax(数据区的开始扇区号)
;---------------------------------------
get_data_section:
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
	
setup_filename	db	"SETUP      "
;no_find	db	"setup is missing."
times (510-($-$$)) db 0
db 0x55, 0xaa
