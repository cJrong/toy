#ifndef _KERBORD_H
#define _KERBORD_H


#define KEY_BUFFER_MAX	256

#define OVERFLOW  0xff
#define NOKEY	0xff

extern char key_table[];
extern char shlft_key_map[];
typedef struct {
	char key_buf[KEY_BUFFER_MAX];
	unsigned int front;
	unsigned int rear;
	unsigned int count;
}KEYBORD_QUEUE;

void init_keybord(void);
unsigned char getkey(void);
void set_caps_led();
void set_num_led();
void set_scroll_led();
int get_caps_stat();





#endif 