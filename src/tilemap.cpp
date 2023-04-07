#include "SDL/SDL_image.h"
#include "sprite_renderer.h"
#include "tilemap.h"
#include <vector>
#include <random>
#include <chrono>
#include "input.h"

//TileProperties tileset[2] = {
    //{true, getSprites()[TetrisBlocks::Wall]},
	//{false, getSprites()[TetrisBlocks::BG]}//want to access stored sprite in sprite rrenderer
//};

char tilemap[22][12] =//dont know how I will draw this
{
	{1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1},

};
void DrawTileMap() {
	for (int row = 0; row < 22; row++) {
		for (int col = 0; col < 12; col++) {
			TetrisBlocks blocktype = (TetrisBlocks)tilemap[row][col];
			Sprite blockSprite = getSprites()[blocktype];
			DrawTile(blockSprite, Vector2Int(col, row));
		}
	}
}
struct Block {
	Block(){}
	Vector2Int pos;
	int direction;
	TetrisBlocks type;
	
};

Block activeBlock;
std::vector<Block> inactiveBlocks;

void ClearGridExceptWalls() {
	for (int row = 1; row < 22 - 2; row++) {
		for (int col = 1; col < 12 - 2; col++) {
			tilemap[row][col] = 0;
		}
	}
}
void UpdateGridWithBlock(Block block) {
	std::vector<Vector2Int> offsets = Cells[block.type];
	Vector2Int pieceCenter = block.pos;
	for (Vector2Int offset : offsets) {
		Vector2Int fillSquare = pieceCenter + offset;
		tilemap[fillSquare.y][fillSquare.x] = block.type;
	}

}
void UpdateGrid() {
	ClearGridExceptWalls();
	for (Block b : inactiveBlocks) {
		UpdateGridWithBlock(b);
	}
	UpdateGridWithBlock(activeBlock);

}

int RandomRange(int min, int max) {
	srand(time(0));
	int num = (rand() % max) + min;
	return num;
}
Block CreatRandomBlockAtStartPos() {
	Block b;
	b.type = (TetrisBlocks)RandomRange(2, 8);
	b.direction = 0;
	b.pos = Vector2Int(12 / 2, 1);
	return b;
}
long long previousTime = 0;
void InitTilemap() {
	activeBlock = CreatRandomBlockAtStartPos();
	inactiveBlocks.clear();
	previousTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}



bool isMoveValid() {
	std::vector<Vector2Int> offsets = Cells[activeBlock.type];
	Vector2Int tileCenterPos = activeBlock.pos;
		for (Vector2Int offset : offsets) {
			Vector2Int tileToCheck = tileCenterPos + offset;
			tileToCheck.y += 1;
			bool isOwnBlock = false;
			for (Vector2Int offsetAgain : offsets) {
				if (tileToCheck == offsetAgain + tileCenterPos) isOwnBlock = true;
			}
			if (isOwnBlock) continue;
			bool isCollision = tilemap[tileToCheck.y][tileToCheck.x] != 0;
			if (isCollision) {
				return false;
			}
		}
		return true;


}
void step() {
	if (!isMoveValid()) {
		inactiveBlocks.push_back(activeBlock);
		activeBlock = CreatRandomBlockAtStartPos();
	}
	else {

		activeBlock.pos.y += 1;
	}
}


void Tiletime() {
	long long seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	long long secondPassedSinceLastFrame = seconds - previousTime;
	if (secondPassedSinceLastFrame >= 1) {
		step();
	}
	previousTime = seconds;
	if (isActionPressed(InputAction::LEFT)) {
		activeBlock.pos.x -= 1;
	}
	else if (isActionPressed(InputAction::RIGHT)) {
		activeBlock.pos.x += 1;
	}
	else if (isActionPressed(InputAction::DOWN)) {
		step();
	}
}



