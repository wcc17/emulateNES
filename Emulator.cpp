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

    window = SDL_CreateWindow("Tile Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

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

        //TODO: INITIALIZE TEXTURES HERE?
    }

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

        onLoop();
        onRender();
    }

    onCleanup();

    return 0;
}

void Emulator::onLoop() {

}

void Emulator::onRender() {
    //clear screen
    SDL_RenderClear(renderer);

    //set the color that the renderer will draw shapes with
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

    //render stuff
    screen->pixels

    //update screen
    SDL_RenderPresent(renderer);
}

void Emulator::onEvent(SDL_Event *event) {
    if(event->type == SDL_QUIT) {
        isRunning = false;
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
