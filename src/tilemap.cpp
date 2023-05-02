#include "SDL/SDL_image.h"
#include "sprite_renderer.h"
#include "tilemap.h"
#include <vector>
#include <random>
#include <chrono>
#include "input.h"
#include "ui.h"
#include "highscore.h"


Block activeBlock;
Block holdBlock;
Block nextBlock;
Block ghostBlock;

bool canSwap;
bool noHoldBlock;
std::vector<Block> inactiveBlocks;

extern int score;
extern int level;

bool isMoveValid(Block block, Vector2Int movementOffset);

Vector2Int blockSpawnPos = Vector2Int(GRID_WIDTH / 2, 1);

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

Vector2Int WallKicksI[8][5] = {
		{ Vector2Int(0, 0), Vector2Int(-2, 0), Vector2Int(1, 0), Vector2Int(-2,-1), Vector2Int(1, 2) },
		{ Vector2Int(0, 0), Vector2Int(2, 0), Vector2Int(-1, 0), Vector2Int(2, 1), Vector2Int(-1,-2) },
		{ Vector2Int(0, 0), Vector2Int(-1, 0), Vector2Int(2, 0), Vector2Int(-1, 2), Vector2Int(2,-1) },
		{ Vector2Int(0, 0), Vector2Int(1, 0), Vector2Int(-2, 0), Vector2Int(1,-2), Vector2Int(-2, 1) },
		{ Vector2Int(0, 0), Vector2Int(2, 0), Vector2Int(-1, 0), Vector2Int(2, 1), Vector2Int(-1,-2) },
		{ Vector2Int(0, 0), Vector2Int(-2, 0), Vector2Int(1, 0), Vector2Int(-2,-1), Vector2Int(1, 2) },
		{ Vector2Int(0, 0), Vector2Int(1, 0), Vector2Int(-2, 0), Vector2Int(1,-2), Vector2Int(-2, 1) },
		{ Vector2Int(0, 0), Vector2Int(-1, 0), Vector2Int(2, 0), Vector2Int(-1, 2), Vector2Int(2,-1) },
};

Vector2Int WallKicksJLOSTZ[8][5] = {
		{ Vector2Int(0, 0),  Vector2Int(-1, 0),  Vector2Int(-1, 1),  Vector2Int(0,-2),  Vector2Int(-1,-2) },
		{ Vector2Int(0, 0),  Vector2Int(1, 0),  Vector2Int(1,-1),  Vector2Int(0, 2),  Vector2Int(1, 2) },
		{ Vector2Int(0, 0),  Vector2Int(1, 0),  Vector2Int(1,-1),  Vector2Int(0, 2),  Vector2Int(1, 2) },
		{ Vector2Int(0, 0),  Vector2Int(-1, 0),  Vector2Int(-1, 1),  Vector2Int(0,-2),  Vector2Int(-1,-2) },
		{ Vector2Int(0, 0),  Vector2Int(1, 0),  Vector2Int(1, 1),  Vector2Int(0,-2),  Vector2Int(1,-2) },
		{ Vector2Int(0, 0),  Vector2Int(-1, 0),  Vector2Int(-1,-1),  Vector2Int(0, 2),  Vector2Int(-1, 2) },
		{ Vector2Int(0, 0),  Vector2Int(-1, 0),  Vector2Int(-1,-1),  Vector2Int(0, 2),  Vector2Int(-1, 2) },
		{ Vector2Int(0, 0),  Vector2Int(1, 0),  Vector2Int(1, 1),  Vector2Int(0,-2),  Vector2Int(1,-2) },
};

void MoveBlock(Block& block, Vector2Int moveVec) {
	std::vector<Vector2Int> offsets = GetBlockOffsets(block.type, block.direction);
	// clear previous position
	for (Vector2Int offset : offsets) {
		Vector2Int tile = block.pos + offset;
		tilemap[tile.y][tile.x] = TetrisBlocks::BG;
	}
	// make the actual move
	block.pos = block.pos + moveVec;
	// refill tilemap grid
	for (Vector2Int offset : offsets) {
		Vector2Int tile = block.pos + offset;
		tilemap[tile.y][tile.x] = block.type;
	}
}
void SetBlockPos(Block& block, Vector2Int pos) {
	std::vector<Vector2Int> offsets = GetBlockOffsets(block.type, block.direction);
	// clear previous position
	for (Vector2Int offset : offsets) {
		Vector2Int tile = block.pos + offset;
		tilemap[tile.y][tile.x] = TetrisBlocks::BG;
	}
	// make the actual move
	block.pos = pos;
	// refill tilemap grid
	for (Vector2Int offset : offsets) {
		Vector2Int tile = block.pos + offset;
		tilemap[tile.y][tile.x] = block.type;
	}
}

int Wrap(int input, int min, int max)
{
	if (input < min)
	{
		return max - (min - input) % (max - min);
	}
	else
	{
		return min + (input - min) % (max - min);
	}
}

void RotateBlock(Block& block, int lr) {
	int prevDirection = block.direction;
	if (lr == 1) {
		block.direction = (block.direction + 1) % 4;
	}
	else if (lr == -1) {
		block.direction = block.direction - 1;
		if (block.direction < 0) {
			block.direction = 3;
		}
	}

	std::vector<Vector2Int> offsets = GetBlockOffsets(block.type, prevDirection);
	// clear previous position
	for (Vector2Int offset : offsets) {
		Vector2Int tile = block.pos + offset;
		tilemap[tile.y][tile.x] = TetrisBlocks::BG;
	}
	if (isMoveValid(block, Vector2Int(0, 0))) {
		offsets = GetBlockOffsets(block.type, block.direction);
		for (Vector2Int offset : offsets) {
			Vector2Int tile = block.pos + offset;
			tilemap[tile.y][tile.x] = block.type;
		}
	}
	else {
		int rotation = block.direction;
		int wallKickIndex = rotation * 2;
		if (lr < 0) {
			wallKickIndex--;
		}
		wallKickIndex = Wrap(wallKickIndex, 0, 8);

		if (block.type == I) {
			for (int i = 0; i < 5; i++) {
				Vector2Int translation = WallKicksI[wallKickIndex][i];
				if (isMoveValid(block, translation)) {
					MoveBlock(block, translation);
					break;
				}
			}
		}
		else {
			for (int i = 0; i < 5; i++) {
				Vector2Int translation = WallKicksJLOSTZ[wallKickIndex][i];
				if (isMoveValid(block, translation)) {
					MoveBlock(block, translation);
					break;
				}
			}
		}



	}

}

void DrawTileMap() {
	for (int row = 0; row < GRID_HEIGHT; row++) {
		for (int col = 0; col < GRID_WIDTH; col++) {
			TetrisBlocks blocktype = (TetrisBlocks)tilemap[row][col];
			Sprite blockSprite = getSprites()[blocktype];
			DrawTile(blockSprite, Vector2Int(col, row));
		}
	}
	DrawBlock(activeBlock.type, activeBlock.direction, ghostBlock.type, ghostBlock.pos);
}

void ClearGridExceptWalls() {
	for (int row = 1; row < GRID_HEIGHT - 1; row++) {
		for (int col = 1; col < GRID_WIDTH - 1; col++) {
			tilemap[row][col] = TetrisBlocks::BG;
		}
	}
}
void ReAffirmGridWalls() {
	for (int col = 0; col < GRID_WIDTH; col++) {
		tilemap[0][col] = TetrisBlocks::Wall;
		tilemap[GRID_HEIGHT-1][col] = TetrisBlocks::Wall;
	}
	for (int row = 0; row < GRID_HEIGHT; row++) {
		tilemap[row][0] = TetrisBlocks::Wall;
		tilemap[row][GRID_WIDTH-1] = TetrisBlocks::Wall;
	}
}
void UpdateGridWithBlock(Block block) {
	std::vector<Vector2Int> offsets = GetBlockOffsets(block.type, block.direction);
	for (Vector2Int offset : offsets) {
		Vector2Int fillSquare = block.pos + offset;
		tilemap[fillSquare.y][fillSquare.x] = block.type;
	}
}
bool isMoveValid(Block block, Vector2Int movementOffset) {
	std::vector<Vector2Int> offsets = GetBlockOffsets(block.type, block.direction);
	Vector2Int tileCenterPos = block.pos;
	for (Vector2Int offset : offsets) {
		Vector2Int tileToCheck = tileCenterPos + offset;
		tileToCheck = tileToCheck + movementOffset;

		// make sure we don't collide with ourself
		bool isOwnBlock = false;
		if (movementOffset.x == 0 && movementOffset.y == 0) {
			
		}
		else {
			for (Vector2Int offsetAgain : offsets) {
				if (tileToCheck == offsetAgain + tileCenterPos) isOwnBlock = true;
			}

			if (isOwnBlock) continue;
			
		}
			char squareWeAreCollidingWith = tilemap[tileToCheck.y][tileToCheck.x];
			bool isCollision = !isBlankBlock((TetrisBlocks)squareWeAreCollidingWith);
			if (isCollision) {
				return false;
			}
		
	}
	return true;
}
void GhostPiece() {
	//ghost piece needs the ghost piece sprite from enum
	//needs x activeblocks type and pos
	// y pos needs to be max valid
	ghostBlock = activeBlock;
	while (isMoveValid(ghostBlock, Vector2Int(0, 1))) {
		ghostBlock.pos.y += 1;
	}
	ghostBlock.type = TetrisBlocks::Ghost;
}

void MoveLineDownOneRow(int row) {
	// copy specified row into row below
	for (int col = 1; col < GRID_WIDTH - 1; col++) {
		tilemap[row+1][col] = tilemap[row][col];
		tilemap[row][col] = TetrisBlocks::BG;
	}
}
void ClearLine(int row) {
	// actually clear the line
	for (int col = 1; col < GRID_WIDTH - 1; col++) {
		tilemap[row][col] = TetrisBlocks::BG;
	}

	// move all blocks above this row down one
	for (int aboveRow = row-1; aboveRow > 2; aboveRow--) {
		MoveLineDownOneRow(aboveRow);
	}
}
void IncrementScore() {
	score++;
	OnScoreChange(score);
	if (score > 10) {
		level++;
	}
}

void LineClears() {
	for (int row = GRID_HEIGHT-2; row > 1; row--) {
		bool isRowFilled = true;
		for (int col = 1; col < GRID_WIDTH - 1; col++) {
			if (isBlankBlock((TetrisBlocks)tilemap[row][col])) {
				isRowFilled = false;
				break;
			}
		}
		if (isRowFilled) {
			ClearLine(row);
			IncrementScore();
		}
	}
}

void UpdateGrid() {
	LineClears();
	GhostPiece();
}

int RandomRangeInclusive(int min, int max) {
	srand(time(0));
	int num = rand() % (max + 1 - min) + min;
	SDL_assert(num >= min && num <= max);
	return num;
}
Block CreatRandomBlockAtStartPos() {
	Block b;
	int randBlockType = RandomRangeInclusive(3, 8);
	b.type = (TetrisBlocks)randBlockType;
	b.direction = 0;
	b.pos = blockSpawnPos;
	return b;
}
long long previousTime = 0;
void InitTilemap() {
	SetGameStarted(false);
	ClearGridExceptWalls();
	activeBlock = CreatRandomBlockAtStartPos();
	UpdateGridWithBlock(activeBlock);
	nextBlock = CreatRandomBlockAtStartPos();
	noHoldBlock = true;
	score = 0;
	OnScoreChange(score);
	level = 1;
	
	inactiveBlocks.clear();
	previousTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}




void LockActiveBlock() {
	inactiveBlocks.push_back(activeBlock);
	activeBlock = nextBlock;
	nextBlock = CreatRandomBlockAtStartPos();
	canSwap = true;
	bool isStartingPosBlocked = !isBlankBlock((TetrisBlocks)tilemap[blockSpawnPos.y][blockSpawnPos.x]);
	if (isStartingPosBlocked) {
		// we lose!
		printf("YOU LOSE\n");
		InitTilemap();
	}
	UpdateGridWithBlock(activeBlock);
}

void TryMoveActiveBlock(InputAction direction) {
	if (direction == InputAction::DOWN) {
		// if we're trying to move down and can't - we lock the block and spawn a new one
		if (!isMoveValid(activeBlock, Vector2Int(0, 1))) {
			LockActiveBlock();
		}
		else {
			MoveBlock(activeBlock, Vector2Int(0, 1));
		}
	}
	// for moving left/right, if we try to but can't, don't spawn a new one. Just do nothing
	else if (direction == InputAction::LEFT) {
		if (isMoveValid(activeBlock, Vector2Int(-1, 0))) {
			MoveBlock(activeBlock, Vector2Int(-1, 0));
		}
	}
	else if (direction == InputAction::RIGHT) {
		if (isMoveValid(activeBlock, Vector2Int(1, 0))) {
			MoveBlock(activeBlock, Vector2Int(1, 0));
		}
	}
}
void SwapBlock(InputAction key) {
	if (key == InputAction::HOLD) {
		if (canSwap) {//canswap needs to check if the player has already swapped before the active piece has been placed

			if (noHoldBlock == false) {//need some way of checking if the hold block is there
				Block holdBlockcopy = holdBlock;
				holdBlock = activeBlock;

				holdBlock.pos = blockSpawnPos;
				holdBlockcopy.pos = blockSpawnPos;

				activeBlock = holdBlockcopy;

				
				
			}
			else {
				holdBlock = activeBlock;
				holdBlock.pos = blockSpawnPos;

				activeBlock = nextBlock;
				
				noHoldBlock = false;
				
			}
			canSwap = false;
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
	else if (isActionPressed(InputAction::HOLD)) {
		SwapBlock(InputAction::HOLD);
	}
	else if (isActionPressed(InputAction::DROP)) {
		SetBlockPos(activeBlock, ghostBlock.pos);
	}
	else if (isActionPressed(InputAction::ROTLEFT)) {
		RotateBlock(activeBlock, -1);
	}
	else if (isActionPressed(InputAction::ROTRIGHT)) {
		RotateBlock(activeBlock, 1);
	}
	ReAffirmGridWalls();
}



