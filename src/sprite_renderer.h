#pragma once
#include "SDL/SDL_image.h"
#include <map>
#include <vector>

#define TILE_SIZE 18

struct Sprite {
	SDL_Texture* texture;
	SDL_Rect sprite;
	SDL_Rect pos;

	Sprite(){}
	Sprite(const char* filename, int x, int y, int w, int h);
		
	void Draw(int x, int y, int w, int h);
};

struct Spritesheet {
	Spritesheet(const char* filename, int spriteWidth, int spriteHeight);
	void Draw(int x, int y, int w, int h);
	std::vector<Sprite> sprites = {};
};

enum TetrisBlocks
{
	BG,
	Wall,
	I,
	O,
	T,
	J,
	L,
	S,
	Z,

	NUM_BLOCKS
};


struct Vector2Int {
	Vector2Int(){}
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
	bool operator==(Vector2Int other) {
		return other.x == this->x && other.y == this->y;
	}
	Vector2Int operator*(Vector2Int other) {
		other.x *= this->x;
		other.y *= this->y;
		return other;
	}
	Vector2Int operator*(int scalar) {
		Vector2Int res = Vector2Int(0, 0);
		res.x = this->x * scalar;
		res.y = this->y * scalar;
		return res;
	}
};

void DrawTile(Sprite sprite, Vector2Int tile);

void DrawBlock(TetrisBlocks block, Vector2Int startTilePos);
void DrawBlockAtWorldPos(TetrisBlocks block, Vector2Int worldPos);

extern std::map<TetrisBlocks, std::vector<Vector2Int>> Cells;

std::map<TetrisBlocks, Sprite>& getSprites();

void SetGridPosition(Vector2Int pos);

void initRenderer();