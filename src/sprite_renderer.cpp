
#include "SDL/SDL_image.h"
#include "sprite_renderer.h"
extern SDL_Renderer* renderer;


	//sprites rects for draw will have to be done for sprite sheets
	Sprite::Sprite(const char* filename, SDL_Rect _sprite, SDL_Rect _pos) {
		sprite = _sprite;
		pos = _pos;
		texture = IMG_LoadTexture(renderer, filename);
	}
	void Sprite::Draw() {
		SDL_RenderCopy(renderer, texture, &sprite, &pos);
	}


//SDL_Texture* texture = IMG_LoadTexture(renderer, "otherBlocks.png");
//SDL_RenderCopy(renderer, texture, NULL, NULL);