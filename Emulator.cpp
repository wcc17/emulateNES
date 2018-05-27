//
// Created by Christian Curry on 7/26/16.
//

#include "Emulator.h"
#include "Emulator/Palette.h"

Emulator::Emulator() {
    isRunning = true;
}

int RES_MULTIPLIER = 4;
bool Emulator::onInit() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL could not initialize. SDL_ERROR: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("emulateNES", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH * RES_MULTIPLIER, SCREEN_HEIGHT * RES_MULTIPLIER, SDL_WINDOW_SHOWN);

    if(window == NULL) {
        printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());
    }
    else {

        renderer = SDL_CreateRenderer(window, -1,  SDL_RENDERER_ACCELERATED);
        if(renderer == NULL) {
            printf("Renderer could not be created. SDL Error: %s\n", SDL_GetError());
            return false;
        } else {
            //Initialize renderer background color
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

            //initialize JPG loading
            int imgFlags = IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags) & imgFlags)) {
                printf("SDL_image could not be initialized. SDL_image Error: %s\n", IMG_GetError());
                return false;
            }
        }
    }

    nes = new NES();
    nes->start();

    return true;
}

int Emulator::onExecute() {
    if(onInit() == false) {
        return -1;
    }

    SDL_Event event;

    while(isRunning) {
        while(SDL_PollEvent(&event)) {
            onEvent(&event);
        }

        nes->execute();
        onLoop();
        onRender();
    }

    nes->stop();
    onCleanup();

    return 0;
}

void Emulator::onLoop() {
}

uint8_t red = 0;
uint8_t green = 0;
uint8_t blue = 0;
void Emulator::onRender() {
    //clear screen
    SDL_RenderClear(renderer);

//    for(int x = 0; x < SCREEN_WIDTH; x++) {
//        for(int y = 0; y < SCREEN_HEIGHT; y++) {
//            //    nes->ppu->pixels;
//            red = palleteRGBValues[nes->ppu->pixels[x][y]].r;
//            green = palleteRGBValues[nes->ppu->pixels[x][y]].g;
//            blue = palleteRGBValues[nes->ppu->pixels[x][y]].b;
//
//            SDL_SetRenderDrawColor(renderer, red, blue, green, 0xFF);
//            SDL_RenderDrawPoint(renderer, x, y);
//        }
//    }
    for(int x = 0; x < SCREEN_WIDTH; x++) {
        for(int y = 0; y < SCREEN_HEIGHT; y++) {
            red = palleteRGBValues[nes->ppu->pixels[x][y]].r;
            green = palleteRGBValues[nes->ppu->pixels[x][y]].g;
            blue = palleteRGBValues[nes->ppu->pixels[x][y]].b;

            for(int a = 0; a < RES_MULTIPLIER; a++) {
                for(int b = 0; b < RES_MULTIPLIER; b++) {
                    SDL_SetRenderDrawColor(renderer, red, blue, green, 0xFF);
                    SDL_RenderDrawPoint(renderer, (x*(RES_MULTIPLIER/2)+a), (y*(RES_MULTIPLIER/2)+b));
                }
            }
        }
    }

    //update screen
    SDL_RenderPresent(renderer);
}

void Emulator::onEvent(SDL_Event *event) {
    if(event->type == SDL_QUIT) {
        isRunning = false;
    }

    if(event->type == SDL_KEYDOWN) {
        switch(event->key.keysym.sym) {
            case SDLK_UP:
                std::cout << "up" << std::endl;
                break;
            case SDLK_DOWN:
                std::cout << "down" << std::endl;
                break;
            case SDLK_LEFT:
                std::cout << "left" << std::endl;
                break;
            case SDLK_RIGHT:
                std::cout << "right" << std::endl;
                break;
        }

    }
}

void Emulator::onCleanup() {
    //destory the window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = NULL;
    window = NULL;

    IMG_Quit();
    SDL_Quit();
}