#include "ui.h"
#include <ctime>


extern int screenWidth;
extern int screenHeight;
extern SDL_Renderer* renderer;

extern Block holdBlock;
extern Block nextBlock;

static Text uiText;

static Spritesheet prisonerL;

bool gameStarted = false;
bool IsGameStarted() { return gameStarted; }
void SetGameStarted(bool isStart) { gameStarted = isStart; }

// NOTE: all of these UI drawing parts are hardcoded lol...
// if we resize the screen everything will kinda jus break

void DrawPlayButton() {
    if (!IsGameStarted()) {
        // draw button when game is not started
        SDL_Rect playButtonRect = { screenWidth / 2.0f - 35, screenHeight / 1.5f, 75, 35 };
        SDL_SetRenderDrawColor(renderer, 232, 80, 111, 255);
        SDL_RenderFillRect(renderer, &playButtonRect);
        uiText.Draw("PLAY", playButtonRect.x, playButtonRect.y);

        // and check if we've clicked it. If so, start the game
        SDL_Point mousePos;
        uint32_t mouseState = SDL_GetMouseState(&mousePos.x, &mousePos.y);
        bool isClick = SDL_BUTTON(1) & mouseState;
        bool isOnButton = SDL_PointInRect(&mousePos, &playButtonRect);
        bool isClickedPlayButton = isClick && isOnButton;
        if (isClickedPlayButton) {
            SetGameStarted(true);
        }
    }
}

void DrawBGSquareOnTileGrid(int left, int right, int top, int bottom) {
    Sprite& bgSprite = getSprites()[TetrisBlocks::BG];
    for (int x = left; x < right; x++) {
        for (int y = top; y < bottom; y++) {
            bgSprite.Draw(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        }
    }
}

void DrawBlockBackground() {
    Sprite& wallSprite = getSprites()[TetrisBlocks::Wall];
    for (int x = 0; x < screenWidth; x+= TILE_SIZE) {
        for (int y = 0; y < screenHeight; y += TILE_SIZE) {
            wallSprite.Draw(x, y, TILE_SIZE, TILE_SIZE);
        }
    }
}
void DrawHoldBox() {
    DrawBGSquareOnTileGrid(1, 6, 1, 5);
    uiText.Draw("HOLD", 2 * TILE_SIZE, TILE_SIZE - 7);
    DrawBlockAtWorldPos(holdBlock.type, Vector2Int(TILE_SIZE * 3, 2 * TILE_SIZE + 10));
}
void DrawPrisonerBoxes() {
    // LEFT PRISONER
    DrawBGSquareOnTileGrid(1, 6, 6, 10);
    static Sprite prisonerLBG;
    static Spritesheet prisonerLSpritesheet;
    if (!prisonerLBG.texture) {
        // this is called "Lazy initialization"
        // because we only actually initialize our resource (spritesheet) when it is first needed
        // as opposed to initializing it when we first boot the program
        prisonerLBG = Sprite("Assets/prisonerL_BG.png");
        prisonerLSpritesheet = Spritesheet("Assets/PlayerSprites/prisonerL.png", 32, 32, 5, 30);
    }
    prisonerLBG.Draw(TILE_SIZE, TILE_SIZE*6, TILE_SIZE*5, TILE_SIZE*4);
    prisonerLSpritesheet.DrawAndTick(TILE_SIZE * 3, TILE_SIZE * 8, 32, 32);

    // RIGHT PRISONER ( bottom right of screen )
    DrawBGSquareOnTileGrid(screenWidth / TILE_SIZE - 6, screenWidth / TILE_SIZE - 1, 17, GRID_HEIGHT-1);
    static Sprite prisonerRBG;
    static Spritesheet prisonerRSpritesheet;
    if (!prisonerRBG.texture) {
        prisonerRBG = Sprite("Assets/prisonerR_BG.png");
        prisonerRSpritesheet = Spritesheet("Assets/PlayerSprites/prisonerR.png", 32, 32, 3, 30);
    }
    prisonerRBG.Draw(TILE_SIZE * 18, TILE_SIZE * 17, TILE_SIZE * 5, TILE_SIZE * 4);
    prisonerRSpritesheet.DrawAndTick(TILE_SIZE * 20, TILE_SIZE * 19, 32, 32);
}
void DrawInfoBox() {
    DrawBGSquareOnTileGrid(1, 6, 11, GRID_HEIGHT - 1);

    // Score
    uiText.Draw("SCORE:", TILE_SIZE + 5, 11 * TILE_SIZE);
    // TODO: put actual score here
    uiText.Draw("0", TILE_SIZE * 3, 13 * TILE_SIZE);

    // Level
    uiText.Draw("LEVEL:", TILE_SIZE + 5, 16 * TILE_SIZE);
    // TODO: put actual level here
    uiText.Draw("0", TILE_SIZE * 3, 18 * TILE_SIZE);
}
void DrawBabyBox() {
    DrawBGSquareOnTileGrid(screenWidth/TILE_SIZE - 6, screenWidth / TILE_SIZE - 1, 1, 5);
    static Spritesheet bigBaby;
    if (!bigBaby.sprites) {
        bigBaby = Spritesheet("Assets/BigBaby.png", 32, 32, 2, 45);
    }
    bigBaby.DrawAndTick(TILE_SIZE * 18, TILE_SIZE, TILE_SIZE * 5, TILE_SIZE * 4);
}
void DrawNextBlockBox() {
    DrawBGSquareOnTileGrid(screenWidth / TILE_SIZE - 6, screenWidth / TILE_SIZE - 1, 6, 16);
    uiText.Draw("NEXT", screenWidth - (TILE_SIZE*5), TILE_SIZE*6);
    DrawBlockAtWorldPos(nextBlock.type, Vector2Int(screenWidth - (TILE_SIZE*4), TILE_SIZE*9));
}


void DrawUIBackground() {
    if (!uiText.isValid()) {
        uiText = Text("KenneyMini.ttf", 25, 129, 29, 95);
    }
    DrawBlockBackground();
    DrawHoldBox();
    DrawPrisonerBoxes();
    DrawInfoBox();
    DrawBabyBox();
    DrawNextBlockBox();
}
void DrawUIForeground() {
    DrawPlayButton();
}