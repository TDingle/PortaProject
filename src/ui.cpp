#include "ui.h"


extern int screenWidth;
extern int screenHeight;



void DrawBlockBackground() {
    Sprite& wallSprite = getSprites()[TetrisBlocks::Wall];
    for (int x = 0; x < screenWidth; x+= TILE_SIZE) {
        for (int y = 0; y < screenHeight; y += TILE_SIZE) {
            wallSprite.Draw(x, y, TILE_SIZE, TILE_SIZE);
        }
    }
}
void DrawHoldBox() {

}
void DrawPrisonerBoxes() {

}
void DrawInfoBox() {

}
void DrawBabyBox() {

}
void DrawNextBlockBox() {

}