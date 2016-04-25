#ifdef  _WIN32
#define _CRT_SECURE_NO_DEPRECATE    1
#define _CRT_NONSTDC_NO_DEPRECATE   1
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include "device.h"

#ifdef DEBUG
#define debug printf
#else 
#define debug
#endif

struct cpu {
    uint16_t pc;
    uint16_t I;
    uint16_t sp;
    uint8_t delay_timer;
    uint8_t sound_timer;
    int wait_key_down;
    uint8_t regs[16];
};

uint8_t font[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

static uint8_t    memory[0x1000];
static uint16_t   stack[16];
static uint8_t    screen[64 * 32];
static uint8_t    keys[16];
static struct cpu cpu;

static void load_app(const char *app);
static void execute();

int main(int argc, char **argv)
{
    uint32_t cycles = 0;
    uint32_t last_ticks = 0;
    bool quit = false;
    
    device_t dev;
    window_t window;
    audio_t audio;
    keyboard_t keyboard;

    memset(&memory, 0, sizeof(memory));
    memcpy(&memory, &font, sizeof(font));
    memset(&stack, 0, sizeof(stack));
    memset(&keys, 0, sizeof(keys));
    memset(&screen, 0, sizeof(screen));
    memset(&cpu, 0, sizeof(cpu.regs));
    cpu.pc = 0x200;
    cpu.wait_key_down = -1;

    load_app(argv[1]);

    srand((unsigned int)time(NULL));

    dev = create_device();
    window = create_window(dev);
    audio = create_audio(dev);
    keyboard = create_keyboard(dev);

    while (!quit) {
        int event;

        event = window_poll_event(window);
        if (event == WE_QUIT) {
            quit = true;
            break;
        }
        
        if (system_ticks() - cycles > 1) {      
            keyboard_press_process(keyboard, keys, cpu.wait_key_down);
            execute();
            cycles = system_ticks();
        }
       
        if (system_ticks() - last_ticks > (1000 / 66)) {
            if (cpu.delay_timer) {
                --cpu.delay_timer;
            }

            if (cpu.sound_timer) {
                if (!--cpu.sound_timer) {
                    audio_beep(audio, 1);

                } else {
                    audio_beep(audio, 0);
                }
            }

            window_render(window, screen, sizeof(screen));
            last_ticks = system_ticks();
        }
    }

    destroy_device(dev);
    destroy_window(window);
    destroy_audio(audio);
    destroy_keyboard(keyboard);
    return 0;
}

static void execute()
{
/*
*   NNN: address
*   NN: 8-bit constant
*   N: 4-bit constant
*   X and Y: 4-bit register identifier
*/
    uint16_t opcode;
    
    opcode = (uint16_t)(memory[cpu.pc] << 8) |
        (uint16_t)(memory[cpu.pc + 1]);

    switch (opcode & 0xF000) {
    case 0x0000:
        switch (opcode & 0x00FF) {
        case 0x00E0:
            /* 0x00E0 Clears the screen. */
            debug("exec 00E0\n");
            memset(screen, 0, sizeof(screen));
            break;

        case 0x00EE:
            /* 0x00EE Returns from a subroutine. */
            debug("exec 00EE\n");
            --cpu.sp;
            cpu.pc = stack[cpu.sp];
            break;

        default:
            debug("unkown [0x%.4x] opcode!\n", opcode);
            return;
        }
        break;

    case 0x1000:
        /* 0x1NNN Jumps to address NNN. */
        debug("exec 1NNN\n");
        cpu.pc = opcode & 0x0FFF;
        return ;

    case 0x2000:
        /* 0x2NNN Calls subroutine at NNN. */
        debug("exec 2NNN\n");
        stack[cpu.sp] = cpu.pc;
        ++cpu.sp;
        cpu.pc = opcode & 0x0FFF;
        return ;

    case 0x3000:
        /* 0x3XNN Skips the next instruction if VX equals NN. */
        debug("exec 3XNN\n");
        if (cpu.regs[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
            cpu.pc += 2;
        }
        break;

    case 0x4000:
        /* 0x4XNN Skips the next instruction if VX doesn't equal NN. */
        debug("exec 4XNN\n");
        if (cpu.regs[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
            cpu.pc += 2;
        }
        break;

    case 0x5000:
        /* 0x5XY0 Skips the next instruction if VX equals VY. */
        debug("exec 5XY0\n");
        if (cpu.regs[(opcode & 0x0F00) >> 8] == cpu.regs[(opcode & 0x00F0) >> 4]) {
            cpu.pc += 2;
        }
        break;

    case 0x6000:
        /* 0x6XNN Sets VX to NN. */
        debug("exec 6XNN\n");
        cpu.regs[(opcode & 0x0F00) >> 8] = (uint8_t)(opcode & 0x00FF);
        break;

    case 0x7000:
        /* 0x7XNN Adds NN to VX. */
        debug("exec 7XNN\n");
        cpu.regs[(opcode & 0x0F00) >> 8] += (uint8_t)(opcode & 0x00FF);
        break;

    case 0x8000:
        switch (opcode & 0x000F) {
        case 0x0000:
            /* 0x8XY0 Sets VX to the value of VY. */
            debug("exec 8XY0\n");
            cpu.regs[(opcode & 0x0F00) >> 8] = cpu.regs[(opcode & 0x00F0) >> 4];
            break;

        case 0x0001:
            /* 0x8XY1 Sets VX to VX or VY. */
            debug("exec 8XY1\n");
            cpu.regs[(opcode & 0x0F00) >> 8] |= cpu.regs[(opcode & 0x00F0) >> 4];
            break;

        case 0x0002:
            /* 0x8XY2 Sets VX to VX and VY. */
            debug("exec 8XY2\n");
            cpu.regs[(opcode & 0x0F00) >> 8] &= cpu.regs[(opcode & 0x00F0) >> 4];
            break;

        case 0x0003:
            /* 0x8XY3 Sets VX to VX xor VY. */
            debug("exec 8XY3\n");
            cpu.regs[(opcode & 0x0F00) >> 8] ^= cpu.regs[(opcode & 0x00F0) >> 4];
            break;

        case 0x0004:
            /* 
            *  0x8XY4 Adds VY to VX. VF is set to 1 when there's a carry, 
            *  and to 0 when there isn't. 
            */
            debug("exec 8XY4\n");
            if (cpu.regs[(opcode & 0x00F0) >> 4] > (0xFF - cpu.regs[(opcode & 0x0F00) >> 8])) {
                cpu.regs[0xF] = 1;
            } else {
                cpu.regs[0xF] = 0;
            }

            cpu.regs[(opcode & 0x0F00) >> 8] += cpu.regs[(opcode & 0x00F0) >> 4];
            break;

        case 0x0005:
            /* 
            *  0x8XY5 VY is subtracted from VX. VF is set to 0 when there's a borrow,
            *  and 1 when there isn't.
            */
            debug("exec 8XY5\n");
            if (cpu.regs[(opcode & 0x0F00) >> 8] >= cpu.regs[(opcode & 0x00F0) >> 4]) {
                cpu.regs[0xF] = 1;
            } else {
                cpu.regs[0xF] = 0;
            }

            cpu.regs[(opcode & 0x0F00) >> 8] -= cpu.regs[(opcode & 0x00F0) >> 4];
            break;

        case 0x0006:
            /*
            *  0x8XY6 Shifts VX right by one. 
            *  VF is set to the value of the least significant bit of VX before the shift.
            */
            debug("exec 8XY6\n");
            cpu.regs[0xF] = cpu.regs[(opcode & 0x0F00) >> 8] & 1;
            cpu.regs[(opcode & 0x0F00) >> 8] >>= 1;
            break;

        case 0x0007:
            /*
            * 0x8XY7 Sets VX to VY minus VX. 
            * VF is set to 0 when there's a borrow, and 1 when there isn't.
            */
            debug("exec 8XY7\n");
            if (cpu.regs[(opcode & 0x0F00) >> 8] <= cpu.regs[(opcode & 0x00F0) >> 4]) {
                cpu.regs[0xF] = 1;
            } else {
                cpu.regs[0xF] = 0;
            }

            cpu.regs[(opcode & 0x0F00) >> 8] = cpu.regs[(opcode & 0x00F0) >> 4] - cpu.regs[(opcode & 0x0F00) >> 8];
            break;

        case 0x000E:
            /*
            * 0x8XYE Shifts VX left by one.
            * VF is set to the value of the most significant bit of VX before the shift.
            */
            debug("exec 8XYE\n");
            cpu.regs[0xF] = cpu.regs[(opcode & 0x0F00) >> 8] >> 7;
            cpu.regs[(opcode & 0x0F00) >> 8] <<= 1;
            break;

        default:
            debug("unkown [0x%x] opcode!\n", opcode);
            return ;
        }
        break;

    case 0x9000:
        /* 0x9XY0 Skips the next instruction if VX doesn't equal VY. */
        debug("exec 9XY0\n");
        if (cpu.regs[(opcode & 0x0F00) >> 8] != cpu.regs[(opcode & 0x00F0) >> 4]) {
            cpu.pc += 2;
        }
        break;

    case 0xA000:
        /* 0xANNN Sets I to the address NNN. */
        debug("exec ANNN\n");
        cpu.I = opcode & 0x0FFF;
        break;

    case 0xB000:
        /* 0xBNNN Jumps to the address NNN plus V0. */
        debug("exec BNNN\n");
        cpu.pc = cpu.regs[(opcode & 0x0FFF)] + cpu.regs[0];
        break;

    case 0xC000:
        /* 0xCXNN Sets VX to the result of a bitwise and operation on a random number and NN. */
        debug("exec CXNN\n");
        cpu.regs[(opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (opcode & 0x00FF);
        break;

    case 0xD000:
        /*
        * 0xDXYN Sprites stored in memory at location in index register (I), 8bits wide. 
        * Wraps around the screen. 
        * If when drawn, clears a pixel, register VF is set to 1 otherwise it is zero. 
        * All drawing is XOR drawing (i.e. it toggles the screen pixels). 
        * Sprites are drawn starting at position VX, VY. 
        * N is the number of 8bit rows that need to be drawn. 
        * If N is greater than 1, second line continues at position VX, VY+1, and so on.
        */
        debug("exec DXYN\n");
        {
        uint16_t x = cpu.regs[(opcode & 0x0F00) >> 8];
        uint16_t y = cpu.regs[(opcode & 0x00F0) >> 4];
        uint16_t h = opcode & 0x000F;
        uint8_t pixel;
        int i, j;

        cpu.regs[0xF] = 0;

        for (i = 0; i < h; i++) {
            pixel = memory[cpu.I + i];
            for (j = 0; j < 8; j++) {
                if ((pixel & (0x80 >> j)) != 0) {
                    if (screen[x + j + (y + i) * 64] == 1) {
                        cpu.regs[0xF] = 1;
                    }
                    screen[x + j + (y + i) * 64] ^= 1;
                }
            }
        }
        }
        break;

    case 0xE000:
        switch (opcode & 0x00FF) {
        case 0x009E:
            /* 0xEX9E Skips the next instruction if the key stored in VX is pressed. */
            debug("exec EX9E\n");
            if (keys[cpu.regs[(opcode & 0x0F00) >> 8]] != 0) {
                cpu.pc += 2;
            }
            break;

        case 0x00A1:
            /* 0xEXA1 Skips the next instruction if the key stored in VX isn't pressed. */
            debug("exec EXA1\n");
            if (keys[cpu.regs[(opcode & 0x0F00) >> 8]] == 0) {
                cpu.pc += 2;
            }
            break;

        default:
            debug("unkown [0x%x] opcode!\n", opcode);
            return ;
        }
        break;

    case 0xF000:
        switch (opcode & 0x00FF) {
        case 0x0007:
            /* 0xFX07 Sets VX to the value of the delay timer. */
            debug("exec FX07\n");
            cpu.regs[(opcode & 0x0F00) >> 8] = cpu.delay_timer;
            break;

        case 0x000A:
            /* 0xFX0A A key press is awaited, and then stored in VX. */
            debug("exec FX0A\n");
            {
            int i;
            bool press = false;
           
            for (i = 0; i < 16; ++i) {
                if (keys[i] != 0) {
                    cpu.regs[(opcode & 0x0F00) >> 8] = i;
                    press = true;
                }
            }

            if (!press) {
                return;
            }
            }
            break;

        case 0x0015:
            /* 0xFX15 Sets the delay timer to VX. */
            debug("exec FX15\n");
            cpu.delay_timer = cpu.regs[(opcode & 0x0F00) >> 8];
            break;

        case 0x0018:
            /* 0xFX18 Sets the sound timer to VX. */
            debug("exec FX18\n");
            cpu.sound_timer = cpu.regs[(opcode & 0x0F00) >> 8];
            break;

        case 0x001E:
            /* 0xFX1E Adds VX to I */
            debug("exec FX1E\n");

            if (cpu.I + cpu.regs[(opcode & 0x0F00) >> 8] > 0xFFF) {
                cpu.regs[0xF] = 1;
            } else {
                cpu.regs[0xF] = 0;
            }
            cpu.I += cpu.regs[(opcode & 0x0F00) >> 8];
            break;

        case 0x0029:
            /* 
             * 0xFX29 Sets I to the location of the sprite for the character in VX. 
             * Characters 0-F (in hexadecimal) are represented by a 4x5 font. 
             */
            debug("exec FX29\n");
            cpu.I = (cpu.regs[(opcode & 0x0F00) >> 8]) * 5;
            break;

        case 0x0033:
            /*
            * 0xFX33 Stores the Binary-coded decimal representation of VX, 
            * with the most significant of three digits at the address in I,
            * the middle digit at I plus 1, and the least significant digit at I plus 2. 
            * (In other words, take the decimal representation of VX, 
            * place the hundreds digit in memory at location in I, 
            * the tens digit at location I+1,
            * and the ones digit at location I+2.)
            */
            debug("exec FX33\n");
            memory[cpu.I] = cpu.regs[(opcode & 0x0F00) >> 8] / 100;
            memory[cpu.I + 1] = (cpu.regs[(opcode & 0x0F00) >> 8] / 10) % 10;
            memory[cpu.I + 2] = (cpu.regs[(opcode & 0x0F00) >> 8] % 100) % 10;
            break;

        case 0x0055:
            /* 0xFX55 Stores V0 to VX in memory starting at address I. */
            debug("exec FX55\n");
            {
            int i;
            for (i = 0; i <= (opcode & 0x0F00) >> 8; ++i) {
                memory[cpu.I + i] = cpu.regs[i];
            }

            cpu.I += ((opcode & 0x0F00) >> 8) + 1;
            }
            break;

        case 0x0065:
            /* 0XFX65 Fills V0 to VX with values from memory starting at address I. */
            debug("exec FX65\n");
            {
            int i;
            for (i = 0; i <= (opcode & 0x0F00) >> 8; ++i) {
                cpu.regs[i] = memory[cpu.I + i];
            }

            cpu.I += ((opcode & 0x0F00) >> 8) + 1;
            }
            break;

        default:
            debug("unkown [0x%x] opcode!\n", opcode);
            return ;
        }
        break;

    default:
        debug("unkown [0x%x] opcode!\n", opcode);
        return;
    }

    cpu.pc += 2;
}

static void load_app(const char *app)
{
    FILE *fp;
    struct stat st;
    uint8_t *pmem;

    fp = fopen(app, "rb");
    if (fp == NULL) {
        fprintf(stderr, "unable to load the app.\n");
        exit(-1);
    }

    fstat(fileno(fp), &st);

    if (st.st_size > 0x1000 - 0x200) {
        fprintf(stderr, "ROM too big for memory.\n");
        exit(-1);
    }

    pmem = malloc(st.st_size * sizeof(uint8_t));
    if (pmem == NULL) {
        fprintf(stderr, "memory allocation failure.\n");
        exit(-1);
    }

    if (fread(pmem, sizeof(uint8_t), st.st_size, fp) != st.st_size) {
        fprintf(stderr, "unable to load the app.\n");
        exit(-1);
    }

    memcpy(&memory[0x200], pmem, st.st_size);
    
    fclose(fp);
    free(pmem);
}