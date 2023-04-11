#pragma once
#include "SDL/SDL_image.h"
#include "sprite_renderer.h"

#define GRID_WIDTH 12
#define GRID_HEIGHT 22

struct Block {
	Block() {}
	Vector2Int pos;
	int direction;
	TetrisBlocks type;
};

struct TileProperties {
	bool wall;
	Sprite sprite;
};

void InitTilemap();

void DrawTileMap();

void UpdateGrid();

void Tiletime();
