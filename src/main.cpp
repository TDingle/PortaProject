#include <stdio.h>
#include <iostream>
#define SDL_MAIN_HANDLED
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "sprite_renderer.h"
#include "input.h"
#include "audio.h"
#include "text.h"

SDL_Window* window;
SDL_Renderer* renderer;

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
        800, 600, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

    //InitMusic();

    //Text testText = Text("KenneyMini.ttf", 30, 129, 29, 95, 255);


    Sprite testBlock = Sprite("Assets/otherBlocks.png", 0, 0, 18, 18);
    

    while (!ShouldCloseWindow()) {
        PollInputs();


        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        // do rendering in here
        for (int row = 0; row < 10; row++) {
            for (int col = 0; col < 20; col++) {
                //playGrid[row][col].Draw();
                
            }
            
        }
        DrawBlock(testBlock, TetrisBlocks::I, Vector2Int(3, 3));

        testBlock.Draw(0, 0, 20, 20);
        //testText.Draw("BRUH :)", 20, 20);
        
        SDL_RenderPresent(renderer);
    }

    //testText.Delete();

    SDL_Quit();
    IMG_Quit();

    return 0;
}