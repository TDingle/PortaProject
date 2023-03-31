#pragma once
#include "SDL/SDL_image.h"
struct Sprite {
	SDL_Texture* texture;
	SDL_Rect sprite;
	SDL_Rect pos;
	Sprite(){}
	Sprite(const char* filename, int x, int y, int w, int h);
		
	
	void Draw(int x, int y, int w, int h);
		
};

enum TetrisBlocks
{
	I,
	O,
	T,
	J,
	L,
	S,
	Z
};
struct Vector2Int {
	int x;
	int y;
	Vector2Int(int x, int y) {
		this->x = x;
		this->y = y;
	}
	Vector2Int operator+(Vector2Int other) {
		Vector2Int result = Vector2Int(0, 0);
		result.x = this->x + other.x;
		result.y = this->y + other.y;
		return result;
	}
};
void DrawBlock(Sprite sprite, TetrisBlocks block, Vector2Int startTilePos);