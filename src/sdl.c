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
    SDL_SetRenderDrawColor(psdl->rend, 0x2b, 0x2b, 0x2b, 0xff);
    SDL_RenderClear(psdl->rend);
    SDL_SetRenderDrawColor(psdl->rend, 0x8c, 0xbf, 0x26, 0xff);

    dodisplay(psdl->rend, display);
    
    SDL_RenderPresent(psdl->rend);
    SDL_Delay(1000/fps);
}

void handleeventssdl(struct ssdl* psdl, bool keyboad[0x10]){
    while (SDL_PollEvent(&(psdl->event))) {
        if (psdl->event.type == SDL_QUIT) {
            destroysdl(psdl);
            exit(1);
        } else if (psdl->event.type == SDL_KEYDOWN) {
            switch (psdl->event.key.keysym.scancode)
            {
                case SDL_SCANCODE_0:
                    keyboad[0x0] = true;
                    break;
                case SDL_SCANCODE_1:
                    keyboad[0x1] = true;
                    break;
                case SDL_SCANCODE_2:
                    keyboad[0x2] = true;
                    break;
                case SDL_SCANCODE_3:
                    keyboad[0x3] = true;
                    break;
                case SDL_SCANCODE_4:
                    keyboad[0x4] = true;
                    break;
                case SDL_SCANCODE_5:
                    keyboad[0x5] = true;
                    break;
                case SDL_SCANCODE_6:
                    keyboad[0x6] = true;
                    break;
                case SDL_SCANCODE_7:
                    keyboad[0x7] = true;
                    break;
                case SDL_SCANCODE_8:
                    keyboad[0x8] = true;
                    break;
                case SDL_SCANCODE_9:
                    keyboad[0x9] = true;
                    break;
                case SDL_SCANCODE_A:
                    keyboad[0xa] = true;
                    break;
                case SDL_SCANCODE_B:
                    keyboad[0xb] = true;
                    break;
                case SDL_SCANCODE_C:
                    keyboad[0xc] = true;
                    break;
                case SDL_SCANCODE_D:
                    keyboad[0xd] = true;
                    break;
                case SDL_SCANCODE_E:
                    keyboad[0xe] = true;
                    break;
                case SDL_SCANCODE_F:
                    keyboad[0xf] = true;
                    break;

            }
        } else if (psdl->event.type == SDL_KEYUP) {
            switch (psdl->event.key.keysym.scancode)
            {
                case SDL_SCANCODE_0:
                    keyboad[0x0] = false;
                    break;
                case SDL_SCANCODE_1:
                    keyboad[0x1] = false;
                    break;
                case SDL_SCANCODE_2:
                    keyboad[0x2] = false;
                    break;
                case SDL_SCANCODE_3:
                    keyboad[0x3] = false;
                    break;
                case SDL_SCANCODE_4:
                    keyboad[0x4] = false;
                    break;
                case SDL_SCANCODE_5:
                    keyboad[0x5] = false;
                    break;
                case SDL_SCANCODE_6:
                    keyboad[0x6] = false;
                    break;
                case SDL_SCANCODE_7:
                    keyboad[0x7] = false;
                    break;
                case SDL_SCANCODE_8:
                    keyboad[0x8] = false;
                    break;
                case SDL_SCANCODE_9:
                    keyboad[0x9] = false;
                    break;
                case SDL_SCANCODE_A:
                    keyboad[0xa] = false;
                    break;
                case SDL_SCANCODE_B:
                    keyboad[0xb] = false;
                    break;
                case SDL_SCANCODE_C:
                    keyboad[0xc] = false;
                    break;
                case SDL_SCANCODE_D:
                    keyboad[0xd] = false;
                    break;
                case SDL_SCANCODE_E:
                    keyboad[0xe] = false;
                    break;
                case SDL_SCANCODE_F:
                    keyboad[0xf] = false;
                    break;
            }
        }
    }
}

uint8_t waitforkey(struct ssdl* psdl) {
    while (true) {
        while (SDL_PollEvent(&(psdl->event))) {
            if (psdl->event.type == SDL_KEYDOWN) {
                switch (psdl->event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_0:
                        return 0x0;
                    case SDL_SCANCODE_1:
                        return 0x1;
                    case SDL_SCANCODE_2:
                        return 0x2;
                    case SDL_SCANCODE_3:
                        return 0x3;
                    case SDL_SCANCODE_4:
                        return 0x4;
                    case SDL_SCANCODE_5:
                        return 0x5;
                    case SDL_SCANCODE_6:
                        return 0x6;
                    case SDL_SCANCODE_7:
                        return 0x7;
                    case SDL_SCANCODE_8:
                        return 0x8;
                    case SDL_SCANCODE_9:
                        return 0x9;
                    case SDL_SCANCODE_A:
                        return 0xa;
                    case SDL_SCANCODE_B:
                        return 0xb;
                    case SDL_SCANCODE_C:
                        return 0xc;
                    case SDL_SCANCODE_D:
                        return 0xd;
                    case SDL_SCANCODE_E:
                        return 0xe;
                    case SDL_SCANCODE_F:
                        return 0xf;
                }
            }
        }
    }
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