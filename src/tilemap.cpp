#include "SDL/SDL_image.h"
#include "sprite_renderer.h"
#include "tilemap.h"
#include <vector>
#include <random>
#include <chrono>
#include "input.h"


Block activeBlock;
Block holdBlock;
Block nextBlock;
std::vector<Block> inactiveBlocks;

char tilemap[GRID_HEIGHT][GRID_WIDTH] =
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
	for (int row = 0; row < GRID_HEIGHT; row++) {
		for (int col = 0; col < GRID_WIDTH; col++) {
			TetrisBlocks blocktype = (TetrisBlocks)tilemap[row][col];
			Sprite blockSprite = getSprites()[blocktype];
			DrawTile(blockSprite, Vector2Int(col, row));
		}
	}
}

void ClearGridExceptWalls() {
	for (int row = 1; row < GRID_HEIGHT - 1; row++) {
		for (int col = 1; col < GRID_WIDTH - 1; col++) {
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
	b.pos = Vector2Int(GRID_WIDTH / 2, 1);
	return b;
}
long long previousTime = 0;
void InitTilemap() {
	activeBlock = CreatRandomBlockAtStartPos();
	inactiveBlocks.clear();
	previousTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}



bool isMoveValid(Vector2Int movementOffset) {
	std::vector<Vector2Int> offsets = Cells[activeBlock.type];
	Vector2Int tileCenterPos = activeBlock.pos;
	for (Vector2Int offset : offsets) {
		Vector2Int tileToCheck = tileCenterPos + offset;
		tileToCheck = tileToCheck + movementOffset;

		// make sure we don't collide with ourself
		bool isOwnBlock = false;
		for (Vector2Int offsetAgain : offsets) {
			if (tileToCheck == offsetAgain + tileCenterPos) isOwnBlock = true;
		}
		if (isOwnBlock) continue;

		char squareWeAreCollidingWith = tilemap[tileToCheck.y][tileToCheck.x];
		bool isCollision = squareWeAreCollidingWith != 0;
		if (isCollision) {
			return false;
		}
	}
	return true;
}

void LockActiveBlock() {
	inactiveBlocks.push_back(activeBlock);
	activeBlock = CreatRandomBlockAtStartPos();
}

void TryMoveActiveBlock(InputAction direction) {
	if (direction == InputAction::DOWN) {
		// if we're trying to move down and can't - we lock the block and spawn a new one
		if (!isMoveValid(Vector2Int(0, 1))) {
			LockActiveBlock();
		}
		else {
			activeBlock.pos.y += 1;
		}
	}
	// for moving left/right, if we try to but can't, don't spawn a new one. Just do nothing
	else if (direction == InputAction::LEFT) {
		if (isMoveValid(Vector2Int(-1, 0))) {
			activeBlock.pos.x -= 1;
		}
	}
	else if (direction == InputAction::RIGHT) {
		if (isMoveValid(Vector2Int(1, 0))) {
			activeBlock.pos.x += 1;
		}
	}
}


void Tiletime() {
	long long seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	long long secondPassedSinceLastFrame = seconds - previousTime;
	if (secondPassedSinceLastFrame >= 1) {
		TryMoveActiveBlock(InputAction::DOWN);
	}
	previousTime = seconds;


	if (isActionPressed(InputAction::LEFT)) {
		TryMoveActiveBlock(InputAction::LEFT);
	}
	else if (isActionPressed(InputAction::RIGHT)) {
		TryMoveActiveBlock(InputAction::RIGHT);
	}
	else if (isActionPressed(InputAction::DOWN)) {
		TryMoveActiveBlock(InputAction::DOWN);
	}
}



