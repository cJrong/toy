%define	BOOTSEG		0x7c0			;BOOT 段基址
%define	SETUPSEG	0x8000			;SETUP 段基址
%define SYSSEG		0X1000			;内核 段基址
%define	PARAMSEG	0x9000			;参数 段基址


%define	MEMMAP_OFFSET	0x0002		;内存地图偏移地址

%define	VOLUME	0					;FAT32分区的起始扇区
%define BUFFER	0x7e0				;临时缓冲区，大小为512 bytes


BS_OEMName		equ	0x03
BPB_BytesPerSec	equ	0x0b			;每扇区的字节数
BPB_SecPerClus	equ	0x0d			;每簇的扇区数
BPB_ResvdSecCnt equ	0x0e			;保留扇区的数目
BPB_NumFATs		equ	0x10			;分区中FAT表的份数
BPB_RootEntCnt	equ	0x11			;此项在FAT32中必须置为0
BPB_ToSec16		equ	0x13			;此项在FAT32中必须置为0
BPB_Media		equ	0x15			;存储介质类型
BPB_FATSz16		equ	0x16			;此项在FAT32中必须置为0
BPB_SecPerTrk	equ	0x18			;每磁道扇区数
BPB_NumHeads	equ	0x1a			;磁头数
BPB_HidSec		equ	0x1c			;隐藏的扇区数
BPB_ToSec32		equ	0x20			;该卷总扇区数

BPB_FATSz32		equ	0x24			;一个FAT表所占的扇区数
BPB_Flags		equ	0x28			;标志
BPB_FSVer		equ	0x2a			;FAT32的版本号
BPB_RootClus	equ	0x2c			;根目录所在的第一个簇的簇号
BPB_FSIfo		equ	0x30			;保留区中FAT32卷FSINFO结构所占扇区数
BPB_BkBootSec	equ 0x32			;保留区中引导记录的备份数据所占的扇区数
BPB_Reserved	equ	0x34			;此项在FAT32中必须置为0
BS_DrvNum		equ	0x40			;0x80为磁盘，0x00为软盘
BS_Reserved		equ	0x41			;保留
BS_BootSig		equ	0x43			;卷标序列号
BS_VolID		equ	0x00032452		;扩展引导标记
BS_VolLable		equ	0x47			;磁盘卷标
BS_FilSysType32 equ	0x52			;未知，通常定义为FAT32


	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	