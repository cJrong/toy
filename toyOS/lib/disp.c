#include <disp.h>

void disp_string(char *str, char attr, int row, int col)
{
    char *vedio = (char *)0xb8000;
    row *= 160;
    col *= 2;
    vedio += row+col;
    while ( *str != 0 ) {
        *vedio = *str;
        *(vedio+1) = attr;
        vedio += 2;
        str++;
    }
}

void clear()
{
    char *vedio = (char *)0xb8000;
    int i;
    for (i = 0; i < 160*25; i++, vedio++) {
	
        if ((i % 2) == 0)
			*vedio = 0;
		else 
			*vedio = 0x07;
    }
	
	/* 重新设置光标! */
	io_out8(0x3d4, 0x0e);
	io_out8(0x3d5, 0);
	io_out8(0x3d4, 0x0f);
	io_out8(0x3d5, 0);
}


void hex_to_string(unsigned int hex, char *str)
{
    int i;
	
    for (i = 7; i >= 0; i--, hex >>= 4) {
        int j = hex & 0x0000000f;
        if ( j >= 0 && j <= 9 )
            str[i] = j + '0';
        else    
            str[i] = (j-10) + 'A';
    }
}
