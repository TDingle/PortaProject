
#include "SDL/SDL_image.h"
#include "sprite_renderer.h"
#include <map>
#include <vector>
extern SDL_Renderer* renderer;


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
	
	void DrawTile(Sprite sprite, Vector2Int tile) {
		sprite.Draw(18 * tile.x, 18 * tile.y, 18, 18);
	}


	void DrawBlock(Sprite sprite, TetrisBlocks block, Vector2Int startTilePos) {
		std::vector<Vector2Int> currentBlockOffsets = Cells[block];
		for (int i = 0; i < currentBlockOffsets.size(); i++) {
			Vector2Int offset = currentBlockOffsets[i];
			Vector2Int tileSpaceTile = startTilePos + offset;
			DrawTile(sprite, tileSpaceTile);
		}
	}

	char mPieces[7 /*kind */][4 /* rotation */][5 /* horizontal blocks */][5 /* vertical blocks */] =
	{
		// O
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
		// J
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
		// Z
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
		// S
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