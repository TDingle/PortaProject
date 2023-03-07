#pragma once


struct FC_Font;

struct Text {
    FC_Font* font;

    Text(const char* fontFile, unsigned int size, unsigned char red = 0, unsigned char green = 0, unsigned char blue = 0, unsigned char alpha = 255);
    void Draw(const char* text, float x, float y);
    void Delete();
};