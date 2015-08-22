#ifndef _IO_H
#define _IO_H

extern void io_hlt(void);
extern void io_cli(void);
extern void io_sti(void);
extern int  io_in8(int port);
extern int	io_in16(int port);
extern int	io_in32(int port);
extern int  io_out8(int port, int data);
extern int  io_out16(int port, int data);
extern int  io_out32(int port, int data);
extern int  io_delay(void);

extern void load_ldt(int selector);
extern void load_gdt(int limit, int addr);
extern void load_idt(int limit, int addr);
extern void load_tr(int selector);

#endif