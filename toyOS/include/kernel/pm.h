#ifndef _PM_H
#define _PM_H


/* 段描述符 */
typedef struct{
    unsigned short	limit;
    unsigned short	base_low;
	unsigned char	base_mid;
	unsigned short	attr;
	unsigned char	base_high;
}SEGMENT_DESCRIPTOR;

/* 门描述符 */
typedef struct {
	unsigned short	offset_low;
	unsigned short	selector;
	unsigned char 	count;
	unsigned char	attr;
	unsigned short	offset_high;
}GATE_DESCRIPTOR;


void set_segmdesc(SEGMENT_DESCRIPTOR *sd, unsigned int base, unsigned int limit, unsigned short attr);
void set_gatedesc(GATE_DESCRIPTOR *gd, unsigned short selector, unsigned int offset, unsigned char dcount, unsigned char attr);
void update_segmdesc_base(unsigned short selector, unsigned int base);
void init_gdt(void);
void init_idt(void);

#define INTR_SYS_CALL 0x80
extern void sys_call(void);


/* 数据段type属性 */
#define DESC_ATTRI_DATA_TYPE_W	0x0002		/* Write(可写)位，W=1表示可写入 */
#define DESC_ATTRI_DATA_TYPE_E	0x0004		/* Extend(扩长方向)位，E=1表示向下扩展 */


/* 代码段type属性 */
#define DESC_ATTRI_CODE_TYPE_R 0x0002		/* Read(可读)位，R=1表示可读 */
#define DESC_ATTRI_CODE_TYPE_C 0x0004		/* Conforming(依从，有些译为一致)位。C=1表示允许从低等权级的程序转移到该段执行 */
#define DESC_ATTRI_CODE_TYPE_X 0x0008		/* Executable(可执行)位，X=1表示可以执行，代码段应该总是为1 */


#define DESC_ATTRI_S	0x0010					/* Descriptor Type ，S=0表示是一个系统段，S=1表示是一个存储段 */
#define DESC_ATTRI_DPL0	0x0000					/* Descriptor Privilege Level DPL = 0 */
#define DESC_ATTRI_DPL1	0x0020					/* Descriptor Privilege Level DPL = 1 */
#define DESC_ATTRI_DPL2 0x0040					/* Descriptor Privilege Level DPL = 2 */
#define DESC_ATTRI_DPL3	0x0060					/* Descriptor Privilege Level DPL = 3 */

#define DESC_ATTRI_P	0x0080					/* Segment Present，P=1表示该段存在 */

#define DESC_ATTRI_L	0x2000					/* 64bit Code Segment 长模式标志位 */
#define DESC_ATTRI_DB	0x4000					/* (1)在可执行代码中，这一位叫D位，D=1时，则使用32位。(2)在向下扩展的段中，这一位叫B位，B=1时，段的上部界限位4GB。否则段的上部界限为64kb (3)在描述堆栈段中的描述符中，这一位叫D位， D=1时，隐式堆栈访问指令使用esp，否则使用sp */
#define DESC_ATTRI_G	0x8000					/* Granularity 段界限粒度位，G=1表示粒度位 4KB。 */


#define DESC_ATTRI_CODE16_X		( DESC_ATTRI_P | DESC_ATTRI_S | DESC_ATTRI_CODE_TYPE_X ) 													/* 可执行的16位代码段 */
#define DESC_ATTRI_CODE16_RX	( DESC_ATTRI_P | DESC_ATTRI_S | DESC_ATTRI_CODE_TYPE_X | DESC_ATTRI_CODE_TYPE_R )							/* 可执行可读的16位代码段 */						
#define DESC_ATTRI_CODE16_RC	( DESC_ATTRI_P | DESC_ATTRI_S | DESC_ATTRI_CODE_TYPE_X | DESC_ATTRI_CODE_TYPE_C	)							/* 可执行可依从的16位代码段	*/
#define DESC_ATTRI_CODE16_RXC	( DESC_ATTRI_P | DESC_ATTRI_S | DESC_ATTRI_CODE_TYPE_X | DESC_ATTRI_CODE_TYPE_R | DESC_ATTRI_CODE_TYPE_C ) 	/* 可执行可读可依从的16位代码段 */
#define DESC_ATTRI_CODE32_X		( DESC_ATTRI_DB | DESC_ATTRI_CODE16_X )																		/* 可执行的32位代码段 */
#define DESC_ATTRI_CODE32_RX	( DESC_ATTRI_DB | DESC_ATTRI_CODE16_RX )			/* 可执行可读的32位代码段 */							
#define DESC_ATTRI_CODE32_RC	( DESC_ATTRI_DB | DESC_ATTRI_CODE16_RC )			/* 可执行可依从的32位代码段 */
#define DESC_ATTRI_CODE32_RXC	( DESC_ATTRI_DB | DESC_ATTRI_CODE16_RXC )			/* 可执行可读可依从的32位代码段 */

#define DESC_ATTRI_DATA16_R			( DESC_ATTRI_P | DESC_ATTRI_S )									/* 可读16位数据段 */
#define DESC_ATTRI_DATA16_RW		( DESC_ATTRI_P | DESC_ATTRI_S | DESC_ATTRI_DATA_TYPE_W )		/* 可读可写16位数据段 */
#define DESC_ATTRI_DATA16_RE		( DESC_ATTRI_P | DESC_ATTRI_S | DESC_ATTRI_DATA_TYPE_E )		/* 可读向下扩展16位数据段 */
#define DESC_ATTRI_DATA16_RWE		( DESC_ATTRI_P | DESC_ATTRI_S | DESC_ATTRI_DATA_TYPE_W | DESC_ATTRI_DATA_TYPE_E	)	/* 可读可写向下扩展16位数据段 */
#define DESC_ATTRI_DATA32_R			( DESC_ATTRI_DB | DESC_ATTRI_DATA16_R )							/* 可读32位数据段 */
#define DESC_ATTRI_DATA32_RW		( DESC_ATTRI_DB | DESC_ATTRI_DATA16_RW )						/* 可读可写32位数据段 */
#define DESC_ATTRI_DATA32_RE		( DESC_ATTRI_DB | DESC_ATTRI_DATA16_RE )						/* 可读向下扩展32位数据段 */
#define DESC_ATTRI_DATA32_RWE		( DESC_ATTRI_DB | DESC_ATTRI_DATA16_RWE )						/* 可读可写向下扩展32位数据段 */

/* 系统描述符类型 */
#define DESC_ATTRI_LDT				0x82	/* 局部描述符表段类型值 */
#define DESC_ATTRI_TACKGATE			0x85	/* 任务门类型值 */
#define DESC_ATTRI_386TSS			0x89	/* 可用 386 任务状态段类型值 */
#define DESC_ATTRI_386_CALL_GATE	0x8c	/* 386 调用门类型值 */
#define DESC_ATTRI_386_INT_GATE		0x8e	/* 386 中断门类型值 */
#define DESC_ATTRI_386_TRAP_GATE	0x8f	/* 386 陷阱门类型值 */

#define SELECTOR(index, TI, RPL)	( (index<<3)|(TI<<2)|(RPL) )
#define SELECTOR_GDT(index, RPL)	( SELECTOR((index), (0), (RPL)) )
#define SELECTOR_LDT(index, RPL)	( SELECTOR((index), (1), (RPL)) )

/* 内核段选择子 */
#define	LDT_SELECOTR	SELECTOR_GDT(3, 0)
#define	TR_SELECOTR		SELECTOR_GDT(4, 0)
#define KERNEL_SELECTOR_CS	SELECTOR_GDT(1, 0)
#define KERNEL_SELECTOR_DS	SELECTOR_GDT(2, 0)


/* 任务选择子 */
#define USER_TASK_SELECTOR_CS	SELECTOR_LDT(1, 3)
#define USER_TASK_SELECTOR_DS	SELECTOR_LDT(2, 3)

#define KERNEL_TASK_SELECTOR_CS	SELECTOR_LDT(1, 1)
#define KERNEL_TASK_SELECTOR_DS	SELECTOR_LDT(2, 1)

	
#endif
