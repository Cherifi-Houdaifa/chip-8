#include <SDL2/SDL.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

struct chip8
{
    uint8_t mem[0x1000];
    uint8_t vn[0x10]; // from v0 to vf
    uint16_t i;
    uint16_t pc;
    uint8_t sp;
    uint16_t stack[0x10];
    uint8_t dt; // delay timer
    uint8_t st; // sound timer
    uint8_t display[8][0x20];
};

