#pragma once
#include "SDL/SDL_image.h"
struct Sprite {
	SDL_Texture* texture;
	SDL_Rect sprite;
	SDL_Rect pos;
	Sprite(){}
	Sprite(const char* filename, SDL_Rect _sprite, SDL_Rect _pos);
		
	
	void Draw();
		
	
};