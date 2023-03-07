#include "text.h"

#include "SDL/SDL_ttf.h"
#include "SDL_FontCache.h"

// renderer from main.cpp
extern SDL_Renderer* renderer;

Text::Text(const char* fontFile, unsigned int size, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
    this->font = FC_CreateFont();
    FC_LoadFont(this->font, renderer, fontFile, size, FC_MakeColor(red, green, blue, alpha), TTF_STYLE_NORMAL);
}

void Text::Draw(const char* text, float x, float y) {
    FC_Draw(this->font, renderer, x, y, text);
}

void Text::Delete() {
    FC_FreeFont(this->font);
}