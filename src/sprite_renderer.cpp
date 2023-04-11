
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
	//position on screen
	SDL_RenderCopy(renderer, texture, &sprite, &pos);

}

	
	
	std::map<TetrisBlocks, Sprite> sprites = {};
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
		};
	}

	std::map<TetrisBlocks, Sprite> getSprites() {
		return sprites;
	}

	void DrawTile(Sprite sprite, Vector2Int tile) {
		sprite.Draw(gridPosition.x + TILE_SIZE * tile.x, gridPosition.y + TILE_SIZE * tile.y, TILE_SIZE, TILE_SIZE);
	}


	void DrawBlock(TetrisBlocks block, Vector2Int startTilePos) {
		std::vector<Vector2Int> currentBlockOffsets = Cells[block];
		for (int i = 0; i < currentBlockOffsets.size(); i++) {
			Vector2Int offset = currentBlockOffsets[i];
			Vector2Int tileSpaceTile = startTilePos + offset;
			DrawTile(sprites[block], tileSpaceTile);
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