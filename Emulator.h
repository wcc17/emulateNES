//
// Created by Christian Curry on 7/26/16.
//

#ifndef EMULATENES_EMULATOR_H
#define EMULATENES_EMULATOR_H

#include <SDL.h>
#include <SDL_image.h>
#include <NES.h>

class Emulator {

private:
    const int SCREEN_WIDTH = 256;
    const int SCREEN_HEIGHT = 240;

    NES* nes;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *screen;

    bool isRunning;

    bool onInit();
    void onEvent(SDL_Event* event);
    void onLoop();
    void onRender();
    void onCleanup();

public:
    Emulator();

    int onExecute();

};


#endif //EMULATENES_EMULATOR_H
