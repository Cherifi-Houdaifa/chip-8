#include "hederz.h"


void initsdl(struct ssdl* psdl) {
    psdl->window  = SDL_CreateWindow(
                    "chip-8",
                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                    width, height,
                    SDL_WINDOW_SHOWN
                    );

    if (psdl->window == NULL) {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
        exit(1);
    }

    psdl->rend = SDL_CreateRenderer(psdl->window, -1, 0x0);
    if (psdl->rend == NULL) {
        fprintf(stderr, "could not create renderer: %s\n", SDL_GetError());
        exit(1);
    }


}

void destroysdl(struct ssdl* psdl) {
    SDL_DestroyRenderer(psdl->rend);
    SDL_DestroyWindow(psdl->window);
    SDL_Quit();
}

void updatesdl(struct ssdl* psdl, uint64_t display[0x20]) {
    SDL_SetRenderDrawColor(psdl->rend, 0x0, 0x0, 0x0, 0xff);
    SDL_RenderClear(psdl->rend);
    SDL_SetRenderDrawColor(psdl->rend, 0xff, 0xff, 0xff, 0xff);

    dodisplay(psdl->rend, display);
    
    SDL_RenderPresent(psdl->rend);
    SDL_Delay(1000/fps);
}

void dodisplay (SDL_Renderer* rend, uint64_t display[0x20]) {
    int i = 0,j = 0;
    uint64_t current;

    while (j < 0x20) {
        i = 0;
        current = display[j];
        while (i < 0x40) {
            if (current & 1) {
                drawpixel(rend, (i), j);
            }
            current = current >> 1;
            i++;
        }
        j++;
    }
}

void drawpixel(SDL_Renderer* rend, int x, int y) {
    int bx = x * pixelsize;
    int by = y * pixelsize;
    for (int i = 0; i < pixelsize; i++) {
        for (int j = 0; j < pixelsize; j++) {
            SDL_RenderDrawPoint(rend, bx+i, by+j);
        }
    }
}