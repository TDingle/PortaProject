#pragma once
#include "SDL/SDL_image.h"
#include "sprite_renderer.h"
struct TileProperties {
	bool wall;
	Sprite sprite;

};
void InitTilemap();

void DrawTileMap();

void UpdateGrid();

void Tiletime();
