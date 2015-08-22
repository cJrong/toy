%ifndef _PM_INC
%define _PM_INC

;********************************************************
;	描述符宏定义										
;	宏大小：48Bit										
;	使用方法： 
;		DESCRIPTOR		base_addr, limit, attribute 
;						32Bit	   20Bit    16Bit
;		其中attribute 8~11Bit 应填充为0
;********************************************************
%macro DESCRIPTOR 3
	dw	(%2 & 0xffff)							;段界限0~15b
	dw	(%1 & 0xffff)							;段基址0~15b
	db	(%1 >> 16) & 0xff						;段基址16~23b
	dw	(%3 & 0xf0ff) | ((%2 >> 8) & 0xf00)  	;TYPE, S, DPL, P, 段界限16~19b, AVL, L, D/B, G
	db	(%1 >> 24) & 0xff						;段基址24~32b
%endmacro

;********************************************************
;	描述符指针宏定义
;	宏大小：48Bit
;	使用方法： 
;		GDTPTR		base_addr, limit
;********************************************************
%macro GDTPTR 2
	dw	%2 			;GDT界限
	dd	%1 			;GDT基地址
%endmacro

;********************************************************
;	描述符选择子宏定义
;	使用方法： 
;		SELECTOR(index, TI, RPL)
;********************************************************
%define SELECTOR(index, TI, RPL)	( (index<<3)|(TI<<2)|(RPL) )
%define SELECTOR_GDT(index, RPL)	( SELECTOR((index), (0), (RPL)) )
%define SELECTOR_LDT(index, RPL)	( SELECTOR((index), (1), (RPL)) )

;********************************************************
;	门描述符宏定义										
;	宏大小：48Bit										
;	使用方法： 
;		GATE		selector, offset, dcount, attribute 
;					16bit	  32bit	  5bit	  8bit
;********************************************************
%macro GATE 4
	dw	%2 & 0xffff
	dw	%1 & 0xffff
	db	%3 & 0x1f
	db	%4 & 0xff
	dw	((%2>>16) & 0xffff)
%endmacro



;********************************************************
;	描述符属性相关常量定义
;	缩写约定：
;		DESC: Descriptor
;		ATTRI:	Attribute
;********************************************************

;DESC_ATTRI_DATA_TYPE_A		EQU		0000_0000_0000_0001b	; Accessed(已访问)位，A=1表示被访问过。

;数据段type属性
DESC_ATTRI_DATA_TYPE_W		EQU		0000_0000_0000_0010b	; Write(可写)位，W=1表示可写入
DESC_ATTRI_DATA_TYPE_E		EQU		0000_0000_0000_0100b	; Extend(扩长方向)位，E=1表示向下扩展

;代码段type属性
DESC_ATTRI_CODE_TYPE_R		EQU		0000_0000_0000_0010b	; Read(可读)位，R=1表示可读
DESC_ATTRI_CODE_TYPE_C		EQU		0000_0000_0000_0100b	; Conforming(依从，有些译为一致)位。C=1表示允许从低等权级的程序转移到该段执行
DESC_ATTRI_CODE_TYPE_X		EQU		0000_0000_0000_1000b	; Executable(可执行)位，X=1表示可以执行，代码段应该总是为1


DESC_ATTRI_S				EQU		0000_0000_0001_0000b	; Descriptor Type ，S=0表示是一个系统段，S=1表示是一个存储段
DESC_ATTRI_DPL0				EQU		0000_0000_0000_0000b	; Descriptor Privilege Level DPL = 0
DESC_ATTRI_DPL1				EQU		0000_0000_0010_0000b	; Descriptor Privilege Level DPL = 1
DESC_ATTRI_DPL2				EQU		0000_0000_0100_0000b	; Descriptor Privilege Level DPL = 2
DESC_ATTRI_DPL3				EQU		0000_0000_0110_0000b	; Descriptor Privilege Level DPL = 3
DESC_ATTRI_P				EQU		0000_0000_1000_0000b	; Segment Present，P=1表示该段存在

;DESC_ATTRI_AVL				EQU		0000_0000_0000_0000b	; 保留位，默认不使用
DESC_ATTRI_L				EQU		0010_0000_0000_0000b	; 64bit Code Segment 长模式标志位
DESC_ATTRI_DB				EQU		0100_0000_0000_0000b	; (1)在可执行代码中，这一位叫D位，D=1时，则使用32位。(2)在向下扩展的段中，这一位叫B位，B=1时，段的上部界限位4GB。否则段的上部界限为64kb (3)在描述堆栈段中的描述符中，这一位叫D位， D=1时，隐式堆栈访问指令使用esp，否则使用sp
DESC_ATTRI_G				EQU		1000_0000_0000_0000b	; Granularity 段界限粒度位，G=1表示粒度位 4KB。


DESC_ATTRI_CODE16_X					EQU		DESC_ATTRI_P | DESC_ATTRI_S | DESC_ATTRI_CODE_TYPE_X 													;可执行的16位代码段
DESC_ATTRI_CODE16_RX				EQU		DESC_ATTRI_P | DESC_ATTRI_S | DESC_ATTRI_CODE_TYPE_X | DESC_ATTRI_CODE_TYPE_R							;可执行可读的16位代码段							
DESC_ATTRI_CODE16_RC				EQU		DESC_ATTRI_P | DESC_ATTRI_S | DESC_ATTRI_CODE_TYPE_X | DESC_ATTRI_CODE_TYPE_C							;可执行可依从的16位代码段							
DESC_ATTRI_CODE16_RXC				EQU		DESC_ATTRI_P | DESC_ATTRI_S | DESC_ATTRI_CODE_TYPE_X | DESC_ATTRI_CODE_TYPE_R | DESC_ATTRI_CODE_TYPE_C 	;可执行可读可依从的16位代码段
DESC_ATTRI_CODE32_X					EQU		DESC_ATTRI_DB | DESC_ATTRI_CODE16_X				;可执行的32位代码段
DESC_ATTRI_CODE32_RX				EQU		DESC_ATTRI_DB | DESC_ATTRI_CODE16_RX			;可执行可读的32位代码段							
DESC_ATTRI_CODE32_RC				EQU		DESC_ATTRI_DB | DESC_ATTRI_CODE16_RC			;可执行可依从的32位代码段
DESC_ATTRI_CODE32_RXC				EQU		DESC_ATTRI_DB | DESC_ATTRI_CODE16_RXC			;可执行可读可依从的32位代码段

DESC_ATTRI_DATA16_R					EQU		DESC_ATTRI_P | DESC_ATTRI_S								;可读16位数据段
DESC_ATTRI_DATA16_RW				EQU		DESC_ATTRI_P | DESC_ATTRI_S | DESC_ATTRI_DATA_TYPE_W	;可读可写16位数据段
DESC_ATTRI_DATA16_RE				EQU		DESC_ATTRI_P | DESC_ATTRI_S | DESC_ATTRI_DATA_TYPE_E	;可读向下扩展16位数据段
DESC_ATTRI_DATA16_RWE				EQU		DESC_ATTRI_P | DESC_ATTRI_S | DESC_ATTRI_DATA_TYPE_W | DESC_ATTRI_DATA_TYPE_E	;可读可写向下扩展16位数据段
DESC_ATTRI_DATA32_R					EQU		DESC_ATTRI_DB | DESC_ATTRI_DATA16_R 					;可读32位数据段
DESC_ATTRI_DATA32_RW				EQU		DESC_ATTRI_DB | DESC_ATTRI_DATA16_RW					;可读可写32位数据段
DESC_ATTRI_DATA32_RE				EQU		DESC_ATTRI_DB | DESC_ATTRI_DATA16_RE					;可读向下扩展32位数据段
DESC_ATTRI_DATA32_RWE				EQU		DESC_ATTRI_DB | DESC_ATTRI_DATA16_RWE					;可读可写向下扩展32位数据段

;系统描述符类型
DESC_ATTRI_LDT						EQU		0x82	; 局部描述符表段类型值
DESC_ATTRI_TACKGATE					EQU		0x85	; 任务门类型值
DESC_ATTRI_386TSS					EQU		0x89	; 可用 386 任务状态段类型值
DESC_ATTRI_386_CALL_GATE			EQU		0X8c	; 386 调用门类型值
DESC_ATTRI_386_INT_GATE				EQU		0x8e	; 386 中断门类型值
DESC_ATTRI_386_TRAP_GATE			EQU		0x8f	; 386 陷阱门类型值


;********************************************************
;	分页机制属性相关常量定义
;	缩写约定：
;		PAGE: Page
;		ATTRI:	Attribute
;********************************************************

PAGE_ATTRI_P					EQU		0000_0000_0001b		;存在位，p=1时，表示页表或者页位于内存中
PAGE_ATTRI_RW					EQU		0000_0000_0010b		;读/写位（总是可执行的），rw=1时，表示该页可读可写
PAGE_ATTRI_US					EQU		0000_0000_0100b		;用户/管理位，US=1时，允许所有特权级别访问。US=0时，只能由0、1、2特权级访问
PAGE_ATTRI_PWT					EQU		0000_0000_1000b		;页级通写位，和高速缓存相关，此处应该清零
PAGE_ATTRI_PCD					EQU		0000_0001_0000b		;页级高速缓存禁止位，用来间接决定该表项所指向的那个页是否使用高速缓存策略。此处应该清零
PAGE_ATTRI_A					EQU		0000_0010_0000b		;访问位，由处理器使用。指示此表项所指的页是否被访问过。
PAGE_ATTRI_D					EQU		0000_0100_0000b		;脏位，由处理器使用。表示该表项所指的页是否写过数据
PAGE_ATTRI_PAT					EQU		0000_1000_0000b		;页属性表支持位，Pentium III 之后才支持的， 此位置为0
PAGE_ATTRI_PS					EQU		0000_1000_0000b		;页大小，ps=0时，页大小为4kb。
PAGE_ATTRI_G					EQU		0001_0000_0000b		;全局位，用来指示该表项所指的页是否为全局性质的。如果页是全局的，那么它在高速缓存中一直保存。
;PAGE_ATTRI_AVL					;软件可使用位，忽略











%endif