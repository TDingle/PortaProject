#include <stdio.h>
#include <iostream>
#define SDL_MAIN_HANDLED
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "sprite_renderer.h"
#include "input.h"
#include "audio.h"
#include "text.h"
#include "tilemap.h"

SDL_Window* window;
SDL_Renderer* renderer;

int screenWidth = 600;
int screenHeight = 400;

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

    if (IMG_Init(IMG_INIT_PNG) != 2) {
        printf("error initializing SDL_image: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("DingleTris",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth, screenHeight, 0);
    SDL_SetWindowResizable(window, SDL_FALSE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    initRenderer();

    Vector2Int gridPosition = Vector2Int(screenWidth / 2.0 - (GRID_WIDTH*TILE_SIZE/2.0), screenHeight / 2.0 - (GRID_HEIGHT*TILE_SIZE/2.0));
    SetGridPosition(gridPosition);
    
    //InitMusic();

    InitTilemap();

    while (!ShouldCloseWindow()) {
        PollInputs();

        SDL_SetRenderDrawColor(renderer, 240, 240, 179, 255);
        SDL_RenderClear(renderer);
        { // rendering
            Tiletime();
            UpdateGrid();
            DrawTileMap();
        }
        SDL_RenderPresent(renderer);
    }


    SDL_Quit();
    IMG_Quit();

    return 0;
}