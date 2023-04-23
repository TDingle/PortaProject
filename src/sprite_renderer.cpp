
#include "SDL/SDL_image.h"
#include "sprite_renderer.h"
#include <map>
#include <vector>
extern SDL_Renderer* renderer;

Vector2Int gridPosition = Vector2Int(0, 0);

// sets the position of the tetris playgrid
void SetGridPosition(Vector2Int pos) { gridPosition = pos; }

//sprites rects for draw will have to be done for sprite sheets
Sprite::Sprite(const char* filename, int x, int y, int w, int h) {
	sprite.x = x;
	sprite.y = y;
	sprite.w = w;
	sprite.h = h;

	texture = IMG_LoadTexture(renderer, filename);
}
void Sprite::Draw(int x, int y, int w, int h) {
	SDL_Rect pos;
	pos.x = x;
	pos.y = y;
	pos.w = w;
	pos.h = h;
	SDL_Rect* clipSprite = NULL;
	if (sprite.w != 0 && sprite.h != 0) {
		clipSprite = &sprite;
	}
	//position on screen
	SDL_RenderCopy(renderer, texture, clipSprite, &pos);
}


Vector2Int GetTextureSize(SDL_Texture* texture) {
	SDL_Point size;
	SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
	return Vector2Int(size.x, size.y);
}

Spritesheet::Spritesheet(const char* filename, int spriteWidth, int spriteHeight, int numKeyframes, int tickInterval) {
	this->spriteHeight = spriteHeight;
	this->spriteWidth = spriteWidth;
	this->tickInterval = tickInterval;
	this->numKeyframes = numKeyframes;
	this->sprites = IMG_LoadTexture(renderer, filename);
	this->textureSize = GetTextureSize(this->sprites);
}
void Spritesheet::Draw(int x, int y, int w, int h, int frame) {
	SDL_Rect pos = { x, y, w, h };
	SDL_Rect sprite;
	sprite.w = this->spriteWidth;
	sprite.h = this->spriteHeight;
	// epic spritesheet math time
	// num of rows/cols (of sprite keyframes) in our spritesheet
	int numCols = this->textureSize.x / this->spriteWidth;
	int numRows = this->textureSize.y / this->spriteHeight;

	// top-left corner of this frame's sprite
	int xClip = (frame % numCols) * this->spriteWidth;
	int yClip = ((frame / numCols) % numRows) * this->spriteHeight;

	sprite.x = xClip;
	sprite.y = yClip;

	SDL_RenderCopy(renderer, this->sprites, &sprite, &pos);
}
void Spritesheet::DrawAndTick(int x, int y, int w, int h) {
	bool shouldTickFrame = (this->frame+1) % this->tickInterval == 0;
	if (shouldTickFrame) {
		this->currentAnimFrame++;
		int numCols = this->textureSize.x / this->spriteWidth;
		int numRows = this->textureSize.y / this->spriteHeight;
		bool isBlankKeyframe = this->currentAnimFrame % (numCols * numRows) >= this->numKeyframes;
		if (isBlankKeyframe) {
			this->currentAnimFrame = 0;
		}
	}
	this->frame++;
	this->Draw(x, y, w, h, this->currentAnimFrame);
}
	
	
std::map<TetrisBlocks, Sprite> sprites = {};
float cos_90 = cos(M_PI / 2.0f);
float sin_90 = sin(M_PI / 2.0f);
float RotationMatrix[] = {cos_90, sin_90, -sin_90, cos_90};
std::map<TetrisBlocks, std::vector<Vector2Int>> Cells =
{
	{ TetrisBlocks(I), {Vector2Int(-1, 1), Vector2Int(0, 1), Vector2Int(1, 1), Vector2Int(2, 1)}},
	{ TetrisBlocks(J), {Vector2Int(-1, 1),  Vector2Int(-1, 0), Vector2Int(0, 0), Vector2Int(1, 0)}},
	{ TetrisBlocks(L), {Vector2Int(1, 1), Vector2Int(-1, 0), Vector2Int(0, 0), Vector2Int(1, 0)}},
	{ TetrisBlocks(O), {Vector2Int(0, 1), Vector2Int(1, 1), Vector2Int(0, 0), Vector2Int(1, 0)}},
	{ TetrisBlocks(S), {Vector2Int(0, 1), Vector2Int(1, 1), Vector2Int(-1, 0), Vector2Int(0, 0)}},
	{ TetrisBlocks(T), {Vector2Int(0, 1), Vector2Int(-1, 0), Vector2Int(0, 0), Vector2Int(1, 0)}},
	{ TetrisBlocks(Z), {Vector2Int(-1, 1), Vector2Int(0, 1), Vector2Int(0, 0), Vector2Int(1, 0)}},
};

std::vector<Vector2Int> GetBlockOffsets(TetrisBlocks type, int rotationIndex) {
	// TODO: rotations done here
	return Cells[type];
}

void initRenderer() {
	sprites = {
	{TetrisBlocks(I), {Sprite("Assets/linePiece.png", 0, 0, TILE_SIZE, TILE_SIZE)}},
	{TetrisBlocks(J), {Sprite("Assets/otherBlocks.png", 0, TILE_SIZE, TILE_SIZE, TILE_SIZE)}},
	{TetrisBlocks(L), {Sprite("Assets/otherBlocks.png", TILE_SIZE, TILE_SIZE, TILE_SIZE, TILE_SIZE)}},
	{TetrisBlocks(O), {Sprite("Assets/otherBlocks.png", TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE)}},
	{TetrisBlocks(S), {Sprite("Assets/otherBlocks.png", TILE_SIZE * 2, TILE_SIZE, TILE_SIZE, TILE_SIZE)}},
	{TetrisBlocks(T), {Sprite("Assets/otherBlocks.png", TILE_SIZE, 0, TILE_SIZE, TILE_SIZE)}},
	{TetrisBlocks(Z), {Sprite("Assets/otherBlocks.png", 0, TILE_SIZE * 2, TILE_SIZE, TILE_SIZE)}},
	{TetrisBlocks(Wall),{Sprite("Assets/wall.png", 0, 0, TILE_SIZE, TILE_SIZE)}},
	{TetrisBlocks(BG),{Sprite("Assets/otherBlocks.png", TILE_SIZE, TILE_SIZE * 2, TILE_SIZE, TILE_SIZE)}},
	{TetrisBlocks(Ghost),{Sprite("Assets/ghost.png", 0, 0, TILE_SIZE, TILE_SIZE)}}
	};
}

std::map<TetrisBlocks, Sprite>& getSprites() {
	return sprites;
}

void DrawTile(Sprite sprite, Vector2Int tile) {
	sprite.Draw(gridPosition.x + TILE_SIZE * tile.x, gridPosition.y + TILE_SIZE * tile.y, TILE_SIZE, TILE_SIZE);
}


void DrawBlock(TetrisBlocks offsetBlock,TetrisBlocks spriteBlock, Vector2Int startTilePos) {
	std::vector<Vector2Int> currentBlockOffsets = GetBlockOffsets(offsetBlock);
	for (int i = 0; i < currentBlockOffsets.size(); i++) {
		Vector2Int offset = currentBlockOffsets[i];
		Vector2Int tileSpaceTile = startTilePos + offset;
		DrawTile(sprites[spriteBlock], tileSpaceTile);
	}
}
void DrawBlockAtWorldPos(TetrisBlocks block, Vector2Int worldPos) {
	std::vector<Vector2Int> currentBlockOffsets = GetBlockOffsets(block);
	for (int i = 0; i < currentBlockOffsets.size(); i++) {
		Vector2Int offset = currentBlockOffsets[i];
		Vector2Int tilePos = worldPos + (offset*TILE_SIZE);
		sprites[block].Draw(tilePos.x, tilePos.y, TILE_SIZE, TILE_SIZE);
	}
}

	

	char mPieces[7 /*kind */][4 /* rotation */][5 /* horizontal blocks */][5 /* vertical blocks */] =
	{
		// Square
		{
		{
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 2, 1, 0},
		{0, 0, 1, 1, 0},
		{0, 0, 0, 0, 0}
		},
		{
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 2, 1, 0},
		{0, 0, 1, 1, 0},
		{0, 0, 0, 0, 0}
		},
		{
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 2, 1, 0},
		{0, 0, 1, 1, 0},
		{0, 0, 0, 0, 0}
		},
		{
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 2, 1, 0},
		{0, 0, 1, 1, 0},
		{0, 0, 0, 0, 0}
		}
		},

		// I
		{
		{
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 1, 2, 1, 1},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0}
		},
		{
		{0, 0, 0, 0, 0},
		{0, 0, 1, 0, 0},
		{0, 0, 2, 0, 0},
		{0, 0, 1, 0, 0},
		{0, 0, 1, 0, 0}
		},
		{
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{1, 1, 2, 1, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0}
		},
		{
		{0, 0, 1, 0, 0},
		{0, 0, 1, 0, 0},
		{0, 0, 2, 0, 0},
		{0, 0, 1, 0, 0},
		{0, 0, 0, 0, 0}
		}
		}
		,
		// L
		{
		{
		{0, 0, 0, 0, 0},
		{0, 0, 1, 0, 0},
		{0, 0, 2, 0, 0},
		{0, 0, 1, 1, 0},
		{0, 0, 0, 0, 0}
		},
		{
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 1, 2, 1, 0},
		{0, 1, 0, 0, 0},
		{0, 0, 0, 0, 0}
		},
		{
		{0, 0, 0, 0, 0},
		{0, 1, 1, 0, 0},
		{0, 0, 2, 0, 0},
		{0, 0, 1, 0, 0},
		{0, 0, 0, 0, 0}
		},
		{
		{0, 0, 0, 0, 0},
		{0, 0, 0, 1, 0},
		{0, 1, 2, 1, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0}
		}
		},
		// L mirrored
		{
		{
		{0, 0, 0, 0, 0},
		{0, 0, 1, 0, 0},
		{0, 0, 2, 0, 0},
		{0, 1, 1, 0, 0},
		{0, 0, 0, 0, 0}
		},
		{
		{0, 0, 0, 0, 0},
		{0, 1, 0, 0, 0},
		{0, 1, 2, 1, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0}
		},
		{
		{0, 0, 0, 0, 0},
		{0, 0, 1, 1, 0},
		{0, 0, 2, 0, 0},
		{0, 0, 1, 0, 0},
		{0, 0, 0, 0, 0}
		},
		{
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 1, 2, 1, 0},
		{0, 0, 0, 1, 0},
		{0, 0, 0, 0, 0}
		}
		},
		// N
		{
		{
		{0, 0, 0, 0, 0},
		{0, 0, 0, 1, 0},
		{0, 0, 2, 1, 0},
		{0, 0, 1, 0, 0},
		{0, 0, 0, 0, 0}
		},
		{
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 1, 2, 0, 0},
		{0, 0, 1, 1, 0},
		{0, 0, 0, 0, 0}
		},
		{
		{0, 0, 0, 0, 0},
		{0, 0, 1, 0, 0},
		{0, 1, 2, 0, 0},
		{0, 1, 0, 0, 0},
		{0, 0, 0, 0, 0}
		},

		{
		{0, 0, 0, 0, 0},
		{0, 1, 1, 0, 0},
		{0, 0, 2, 1, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0}
		}
		},
		// N mirrored
		{
		{
		{0, 0, 0, 0, 0},
		{0, 0, 1, 0, 0},
		{0, 0, 2, 1, 0},
		{0, 0, 0, 1, 0},
		{0, 0, 0, 0, 0}
		},
		{
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 2, 1, 0},
		{0, 1, 1, 0, 0},
		{0, 0, 0, 0, 0}
		},
		{
		{0, 0, 0, 0, 0},
		{0, 1, 0, 0, 0},
		{0, 1, 2, 0, 0},
		{0, 0, 1, 0, 0},
		{0, 0, 0, 0, 0}
		},
		{
		{0, 0, 0, 0, 0},
		{0, 0, 1, 1, 0},
		{0, 1, 2, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0}
		}
		},
		// T
		{
		{
		{0, 0, 0, 0, 0},
		{0, 0, 1, 0, 0},
		{0, 0, 2, 1, 0},
		{0, 0, 1, 0, 0},
		{0, 0, 0, 0, 0}
		},
		{
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 1, 2, 1, 0},
		{0, 0, 1, 0, 0},
		{0, 0, 0, 0, 0}
		},
		{
		{0, 0, 0, 0, 0},
		{0, 0, 1, 0, 0},
		{0, 1, 2, 0, 0},
		{0, 0, 1, 0, 0},
		{0, 0, 0, 0, 0}
		},
		{
		{0, 0, 0, 0, 0},
		{0, 0, 1, 0, 0},
		{0, 1, 2, 1, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0}
		}
		}
	};
	

//SDL_Texture* texture = IMG_LoadTexture(renderer, "otherBlocks.png");
//SDL_RenderCopy(renderer, texture, NULL, NULL);