#include <stdio.h>
#include <iostream>
#define SDL_MAIN_HANDLED
#include "SDL/SDL.h"

#include "input.h" // for PollEvents()

bool ShouldCloseWindow() {
    SDL_Event event;
    // poll specifically for window close events
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        // handling of X button on window and ESC key closing the game
        case SDL_QUIT: {
            return true;
        } break;
        case SDL_KEYDOWN: {
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                return true;
            }
        } break;
        }
    }
    return false;
}


int main(int argc, char* argv[]) {

    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("DingleTris",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600, 0);

    while (!ShouldCloseWindow()) {
        PollInputs();

    }


    return 0;
}