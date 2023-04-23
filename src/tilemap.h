#pragma once
#include "SDL/SDL_image.h"
#include "sprite_renderer.h"

#define GRID_WIDTH 12
#define GRID_HEIGHT 22

struct Block {
	Block() {}
	Vector2Int pos = Vector2Int(0,0);
	int direction = 0;
	TetrisBlocks type;
};

struct TileProperties {
	bool wall;
	Sprite sprite;
};

void MoveBlock(Block& block, Vector2Int moveVec);
void SetBlockPos(Block& block, Vector2Int pos);

void InitTilemap();

void DrawTileMap();

void UpdateGrid();

void Tiletime();

void ClearGridExceptWalls();