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
	Sprite(const char* filename, int x = 0, int y = 0, int w = 0, int h = 0);
		
	void Draw(int x, int y, int w, int h);
};


enum TetrisBlocks : char
{
	BG,
	Wall,
	Ghost,
	I,
	O,
	T,
	J,
	L,
	S,
	Z,

	NUM_BLOCKS
};
inline bool isBlankBlock(TetrisBlocks block) {
	return block == BG || block == Ghost;
}

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
	Vector2Int operator-(Vector2Int other) {
		Vector2Int res;
		res.x = this->x - other.x;
		res.y = this->y - other.y;
		return res;
	}
};

struct Spritesheet {
	Spritesheet() {}
	Spritesheet(const char* filename, int spriteWidth, int spriteHeight, int numKeyframes, int tickInterval = 1);
	void Draw(int x, int y, int w, int h, int frame);
	void DrawAndTick(int x, int y, int w, int h);
	SDL_Texture* sprites = nullptr;
	Vector2Int textureSize = Vector2Int(0,0);
	int spriteWidth = 0;
	int spriteHeight = 0;
	int frame = 0;
	int currentAnimFrame = 0;
	int tickInterval = 1;
	int numKeyframes = 0;
};

std::vector<Vector2Int> GetBlockOffsets(TetrisBlocks type, int rotationIndex = 0);

void DrawTile(Sprite sprite, Vector2Int tile);

void DrawBlock(TetrisBlocks offsetBlock, int offsetBlockDir, TetrisBlocks spriteBlock, Vector2Int startTilePos);
void DrawBlockAtWorldPos(TetrisBlocks block, Vector2Int worldPos);

extern std::map<TetrisBlocks, std::vector<Vector2Int>> Cells;

std::map<TetrisBlocks, Sprite>& getSprites();

void SetGridPosition(Vector2Int pos);

void initRenderer();

