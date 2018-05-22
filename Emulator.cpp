//
// Created by Christian Curry on 7/26/16.
//

#include "Emulator.h"

Emulator::Emulator() {
    isRunning = true;
}

bool Emulator::onInit() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL could not initialize. SDL_ERROR: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("emulateNES", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

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

int frame = 0;
uint8_t red;
uint8_t blue = 85;
uint8_t green = 170;
void Emulator::onRender() {
    //clear screen
    SDL_RenderClear(renderer);

    if(frame % 8 == 0) {
        red++;
        green++;
        blue++;
    }

    //set the color that the renderer will draw shapes with
    for(int x = 0; x < SCREEN_WIDTH; x++) {
        for(int y = 0; y < SCREEN_HEIGHT; y++) {
            SDL_SetRenderDrawColor(renderer, red, blue, green, 0xFF);
            SDL_RenderDrawPoint(renderer, x, y);
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
//    SDL_DestroyTexture(tileImageTexture);
//    tileImageTexture = NULL;

    //destory the window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = NULL;
    window = NULL;

    IMG_Quit();
    SDL_Quit();
}

//TODO: probably will not need this
//SDL_Texture* TileGame::loadTexture(std::string path) {
//    //final texture
//    SDL_Texture* newTexture = NULL;
//
//    //Load image at specfied path
//    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
//    if(loadedSurface == NULL) {
//        printf("Unable to load image %s. SDL_image Error: %s\n", path.c_str(), IMG_GetError());
//    } else {
//        //Create texture from surface pixels
//        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
//        if(newTexture == NULL) {
//            printf("Unable to create texture from %s. SDL Error: %s\n", path.c_str(), SDL_GetError());
//        }
//
//        SDL_FreeSurface(loadedSurface);
//    }
//
//    return newTexture;
//}
