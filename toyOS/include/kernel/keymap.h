#ifndef _KEYMAP_H
#define _KEYMAP_H

#define KEY_NULL		0
#define KEY_ESC			0
#define KEY_BACKSPACE	0
#define KEY_TAB			'\t'
#define KEY_CTRL_LEFT	0
#define KEY_SHIFT_LEFT 	0
#define KEY_SHIFT_RIGHT 0
#define KEY_ALT_LEFT 	0
#define KEY_CAPS_LOCK 	0
#define KEY_F1			0
#define KEY_F2			0
#define KEY_F3			0
#define KEY_F4			0
#define KEY_F5			0
#define KEY_F6			0
#define KEY_F7			0
#define KEY_F8			0
#define KEY_F9			0
#define KEY_F10			0
#define KEY_F11			0
#define KEY_F12			0
#define KEY_NUM_LOCAK	0
#define KEY_SCROLL_LOCK	0
#define KEY_HOME		0
#define KEY_PAGEUP		0
#define KEY_UP			0
#define	KEY_SUB			'-'
#define KEY_LEFT		0
#define	KEY_ENTER		'\n'
#define KEY_RIGHT		0
#define	KEY_ADD			'+'
#define	KEY_END			0
#define KEY_DOWN		0
#define KEY_PAGEDOWN	0
#define KEY_INSERT		0
#define KEY_DEL			'.'

char key_table[] = {	
	KEY_NULL,		KEY_ESC,		'1',			'2',		
	'3',			'4',			'5',			'6',		
	'7',			'8',			'9',			'0',		
	'-',			'=',	KEY_BACKSPACE,			KEY_TAB,		
	'q',			'w',			'e',			'r',		
	't',			'y',			'u',			'i',		
	'o',			'p',			'[',			']',	
	KEY_ENTER,	KEY_CTRL_LEFT,		'a',			's',		
	'd',			'f',			'g',			'h',	
	'j',			'k',			'l',			';',		
	'\'',			'`',	KEY_SHIFT_LEFT,			'\\',		
	'z',			'x',			'c',			'v',
	'b',			'n',			'm',			',',
	'.',			'/',	KEY_SHIFT_RIGHT,		'*',
	KEY_ALT_LEFT,	' ',	KEY_CAPS_LOCK,			KEY_F1,	
	KEY_F2,			KEY_F3,			KEY_F4,			KEY_F5,
	KEY_F6,			KEY_F7,			KEY_F8,			KEY_F8,
	KEY_F10,	KEY_NUM_LOCAK,	KEY_SCROLL_LOCK,	KEY_HOME,
	KEY_UP,		KEY_PAGEUP,		KEY_SUB,			KEY_LEFT,
	KEY_ENTER,	KEY_RIGHT,		KEY_ADD,			KEY_END,
	KEY_DOWN,	KEY_PAGEDOWN,	KEY_INSERT,			KEY_DEL,
	0,				0,				0,				KEY_F11,
	KEY_F12,		0,				0,					0,	
	0,				0,				0,					0,
	0,				0,				0,					0,
	0,				0,				0,					0,
	0,				0,				0,					0,
	0,				0,				0,					0,
	0,				0,				0,					0,
	0,				0,				0,					0,
	0,				0,				0,					0,
	0,				0,				0,
};

char caps_key_table[] = {	
	KEY_NULL,		KEY_ESC,		'1',			'2',		
	'3',			'4',			'5',			'6',		
	'7',			'8',			'9',			'0',		
	'-',			'=',	KEY_BACKSPACE,			KEY_TAB,		
	'Q',			'W',			'E',			'R',		
	'T',			'Y',			'U',			'I',		
	'O',			'P',			'[',			']',	
	KEY_ENTER,	KEY_CTRL_LEFT,		'A',			'S',		
	'D',			'F',			'G',			'H',	
	'J',			'K',			'L',			';',		
	'\'',			'`',	KEY_SHIFT_LEFT,			'\\',		
	'Z',			'X',			'C',			'V',
	'B',			'N',			'M',			',',
	'.',			'/',	KEY_SHIFT_RIGHT,		'*',
	KEY_ALT_LEFT,	' ',	KEY_CAPS_LOCK,			KEY_F1,	
	KEY_F2,			KEY_F3,			KEY_F4,			KEY_F5,
	KEY_F6,			KEY_F7,			KEY_F8,			KEY_F8,
	KEY_F10,	KEY_NUM_LOCAK,	KEY_SCROLL_LOCK,	KEY_HOME,
	KEY_UP,		KEY_PAGEUP,		KEY_SUB,			KEY_LEFT,
	KEY_ENTER,	KEY_RIGHT,		KEY_ADD,			KEY_END,
	KEY_DOWN,	KEY_PAGEDOWN,	KEY_INSERT,			KEY_DEL,
	0,				0,				0,				KEY_F11,
	KEY_F12,		0,				0,					0,	
	0,				0,				0,					0,
	0,				0,				0,					0,
	0,				0,				0,					0,
	0,				0,				0,					0,
	0,				0,				0,					0,
	0,				0,				0,					0,
	0,				0,				0,					0,
	0,				0,				0,					0,
	0,				0,				0,
};

char shift_key_table[] = {	
	KEY_NULL,		KEY_ESC,		'!',			'@',		
	'#',			'$',			'%',			'^',		
	'&',			'*',			'(',			')',		
	'_',			'+',	KEY_BACKSPACE,			KEY_TAB,		
	'Q',			'W',			'E',			'R',		
	'T',			'Y',			'U',			'I',		
	'O',			'P',			'{',			'}',	
	KEY_ENTER,	KEY_CTRL_LEFT,		'A',			'S',		
	'D',			'F',			'G',			'H',	
	'J',			'K',			'L',			':',		
	'"',			'~',	KEY_SHIFT_LEFT,			'|',		
	'Z',			'X',			'C',			'V',
	'B',			'N',			'M',			'<',
	'>',			'?',	KEY_SHIFT_RIGHT,		'*',
	KEY_ALT_LEFT,	' ',	KEY_CAPS_LOCK,			KEY_F1,	
	KEY_F2,			KEY_F3,			KEY_F4,			KEY_F5,
	KEY_F6,			KEY_F7,			KEY_F8,			KEY_F8,
	KEY_F10,	KEY_NUM_LOCAK,	KEY_SCROLL_LOCK,	KEY_HOME,
	KEY_UP,		KEY_PAGEUP,		KEY_SUB,			KEY_LEFT,
	KEY_ENTER,	KEY_RIGHT,		KEY_ADD,			KEY_END,
	KEY_DOWN,	KEY_PAGEDOWN,	KEY_INSERT,			KEY_DEL,
	0,				0,				0,				KEY_F11,
	KEY_F12,		0,				0,					0,	
	0,				0,				0,					0,
	0,				0,				0,					0,
	0,				0,				0,					0,
	0,				0,				0,					0,
	0,				0,				0,					0,
	0,				0,				0,					0,
	0,				0,				0,					0,
	0,				0,				0,					0,
	0,				0,				0,
};

#endif