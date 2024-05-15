#include <SDL2/SDL.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdbool.h>

#define pixelsize 0xa
#define width (64 * pixelsize)
#define height (32 * pixelsize)
#define fps 60


struct chip8
{
    uint8_t mem[0x1000];
    uint8_t vn[0x10]; // from v0 to vf
    uint16_t i;
    uint16_t pc;
    uint8_t sp;
    uint16_t stack[0x11];
    uint8_t dt; // delay timer
    uint8_t st; // sound timer
    uint64_t display[0x20];
    bool displaychanged;
    bool keyboad[0x10];
};

struct ssdl {
    SDL_Window* window;
    SDL_Renderer* rend;
    SDL_Event event;
};

// chip8 stuff
void initchip8 (struct chip8** cpu, char* file);
uint16_t fetch(struct chip8* cpu);
void execute(struct chip8* cpu);
void timerz(struct chip8* cpu);

// sdl stuff
void initsdl(struct ssdl* psdl);
void updatesdl(struct ssdl* psdl, struct chip8* cpu);
void destroysdl(struct ssdl* psdl);
void dodisplay (SDL_Renderer* rend, uint64_t display[0x20]);
void drawpixel(SDL_Renderer* rend, int x, int y);
void handleeventssdl(struct ssdl* psdl, bool keyboad[0x10]);
uint8_t waitforkey(struct ssdl* psdl);
