#include <kernel/keybord.h>
#include <kernel/keymap.h>
#include <kernel/io.h>
#include <disp.h>
#include <stdio.h>
#include <string.h>
#include <kernel/tty.h>

#define VIDEO_BASE_ADDR 0xb8000
#define NR_TTY	3

TTY_STRUCT tty_list[NR_TTY];
TTY_STRUCT *tty_current;


void set_cursor(int cursor_pos)
{
	io_cli();
	io_out8(0x3d4, 0x0e);
	io_out8(0x3d5, 0xff&(cursor_pos>>8));
	io_out8(0x3d4, 0x0f);
	io_out8(0x3d5, 0xff&cursor_pos);
	io_sti();
}

void set_vedio_start_addr(unsigned int addr)
{
	io_cli();
	io_out8(0x3d4, 0x0c);			/* high */
	io_out8(0x3d5, (addr>>8)&0xff);
	io_out8(0x3d4, 0x0d);
	io_out8(0x3d5, addr&0xff);		/* lower */
	io_sti();
}

/* 向下滚动一行  */
static void scrdown(TTY_STRUCT *tty)
{
	if (tty->pointer_addr < tty->end_addr) {
		tty->pointer_addr += 80;
		set_vedio_start_addr(tty->pointer_addr);
	}
}

/* 向上滚动一行 */
static void scrup(TTY_STRUCT *tty)
{
	if (tty->pointer_addr > tty->current_addr) {
		tty->pointer_addr -= 80;
		set_vedio_start_addr(tty->pointer_addr);
	}
}

void init_tty(void)
{
	int i;
	
	for (i = 0; i < NR_TTY; i++) {
		tty_list[i].start_addr = VIDEO_BASE_ADDR + (i * 0x3000);
		tty_list[i].current_addr = i * 0x3000;
		tty_list[i].pointer_addr = i * 0x3000 / 2;
		tty_list[i].end_addr = tty_list[i].current_addr + 0x3000 / 2;

		tty_list[i].char_pos = 0;
		tty_list[i].cursor_pos = i * 0x3000 / 2;
	}
	
	tty_current = &tty_list[0];
}

int tty_putchar(TTY_STRUCT *tty, char ch, unsigned char attr)
{
	char *video = (char *)tty->start_addr;
	int i;
	switch (ch) {
	case '\n':
		tty->char_pos = (tty->char_pos+80) - (tty->char_pos%80);
		tty->cursor_pos = (tty->current_addr/2) +  tty->char_pos;
		break;
	case '\t':
		for (i=0; i < 4; i++) {
			*(video+(tty->char_pos*2)) = ' ';
			*(video+(tty->char_pos*2)+1) = attr;
			tty->cursor_pos++;
			tty->char_pos++;		
		}
		break;
	default :
		*(video+(tty->char_pos*2)) = ch;
		*(video+(tty->char_pos*2)+1) = attr;
		tty->cursor_pos++;
		tty->char_pos++;
		break;
	}
	set_cursor(tty_current->cursor_pos);
}

static void keybord_handler()
{
	unsigned char scan_code = getkey();
	int make = !(scan_code & 0x80);
	static unsigned char shift_make;
	static unsigned char alt_make;
	static unsigned char e0_make;
	char buf[10]={0};
	
	
	if (scan_code == NOKEY) {
		return;
	}
	
	if (scan_code == 0xe0 || e0_make) {
		e0_make = 1;
		switch (scan_code) {
		case 0x51:
			scrdown(tty_current);
			break;
		case 0x49:
			scrup(tty_current);
			break;
		case 0xc9:
		case 0xd1:
			e0_make = 0;
			break;
		}

		return ;
	}
	
	if(make) {
		switch (scan_code) {
		case 0x2a:
		case 0x36:	/* shift press */
			shift_make = 1;
			break; 
		case 0x38:	/* alt press */
			alt_make = 1;
			break;
		case 0x3a: 
			set_caps_led();
			break;
		case 0x45:
			set_num_led();
			break;
		case 0x46:
			set_scroll_led();
			break;
		case 0x3b:/*F1 ~ F3*/
		case 0x3c:
		case 0x3d:
			if (alt_make) {
				tty_current = &tty_list[scan_code-0x3b];
				set_vedio_start_addr(tty_current->current_addr/2);
				set_cursor(tty_current->cursor_pos);
				/*
				sprintf(buf, "hello to tty%d ", scan_code-0x3b);
				tty_write(tty_current, buf, strlen(buf));*/
			}
			break;
		default:
		
			if (get_caps_stat()) {
				if (shift_make) 
					if (shift_key_table[scan_code] >= 'A' && shift_key_table[scan_code] <= 'Z')
						buf[0] = shift_key_table[scan_code]+32;
					else 
						buf[0] = shift_key_table[scan_code];
				else 
					buf[0] = caps_key_table[scan_code];
			}
			else {
				if (shift_make) 
					buf[0] = shift_key_table[scan_code];
				else 
					buf[0] = key_table[scan_code];		
			}
		
			tty_write(tty_current, buf, strlen(buf));
			break; 
		}
		
	}
	else {
		
		switch (scan_code)
		{
		case 0xaa:
		case 0xb6:
			shift_make = 0;
			break;
		case 0xb8:
			alt_make = 0;
			break; 
		}
	}
	
}


void tty_write(TTY_STRUCT *tty, char *buf, int n)
{
	while (n--)
		tty_putchar(tty, *buf++, 0x07);
}


void tty_task(void)
{
	while (1) {
		keybord_handler();
	}
}

