#include <kernel/keybord.h>
#include <string.h>


static KEYBORD_QUEUE key_queue;
static unsigned char caps = 0;
static unsigned char num = 0;
static unsigned char scroll = 0;

void init_keybord(void)
{	
	memset(key_queue.key_buf, 0, KEY_BUFFER_MAX);
	key_queue.count = 0;
	key_queue.front = key_queue.rear = 0;
}

unsigned char getkey(void)
{
	char ret;
	io_cli();
	if (key_queue.count > 0) {
		ret = key_queue.key_buf[key_queue.front];
		key_queue.front = (key_queue.front+1) % KEY_BUFFER_MAX;
		key_queue.count--;
	}
	else {
		ret = NOKEY;
	}
	io_sti();
	return ret;
}

static void putkey(char key)
{
	io_cli();
	if (key_queue.count < KEY_BUFFER_MAX) {
		key_queue.key_buf[key_queue.rear] = key;
		key_queue.rear = (key_queue.rear+1) % KEY_BUFFER_MAX;
		key_queue.count++;
	}
	io_sti();
}

void keyboard_interrupt(void)
{
	char scan_code = io_in8(0x60);
	putkey(scan_code);
}

static void keybord_wait()
{
	unsigned char stat;
	do {
		stat = io_in8(0x64);
	}while (stat & 0x02);
}

static void keybord_ack()
{
	unsigned char stat;
	do {
		stat = io_in8(0x60);
	}while (stat != 0xfa);
}

static void set_led(unsigned char led)
{
	keybord_wait();
	io_out8(0x60, 0xed);
	keybord_ack();
	
	keybord_wait();
	io_out8(0x60, led);
	keybord_ack();
}

void set_caps_led()
{
	caps = !caps;
	set_led(caps << 2);
}

void set_num_led()
{	
	num = !num;
	set_led(num << 1);	
}

void set_scroll_led()
{	
	scroll = !scroll;
	set_led(scroll);	
}


int get_caps_stat()
{
	return caps;
}

int get_num_stat()
{
	return num;
}

int get_scroll_stat()
{
	return scroll;
}





