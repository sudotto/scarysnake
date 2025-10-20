#include "otto-game.h"

#include "world.h"

Block_data block_data_table[BLOCK_TYPE_COUNT] = {
	{"assets/anthonymackieshirtless.png", 0}, //never gets rendered so :)
	{"assets/blocks/wall.png", 1},
	{"assets/blocks/door.png", 1},
	{"assets/blocks/generator.png", 0},
	{"assets/blocks/exit.png", 0},
};

Block new_block(Block_type type, Game* game){
	Block block;
	block.type = type;
	block.sprite = new_img(game->rend, block_data_table[type].filename, 0);
	block.solid = block_data_table[type].solid;
	return block;
}

void render_block(Block* block, Game* game, int x, int y, int w, int h){
	if(block->type == BLOCK_VOID){
		return;
	}
	render_img(game->rend, &block->sprite, x, y, w, h);
} 

// WORLD

World new_world(Game* game){
	World world;
	world.unlock = 0;
	world.win = 0;
	for(int y = 0; y < 30; y++){
		for(int x = 0; x < 45; x++){
			if(x == 0 || x == 44 || y == 0 || y == 29){
				world.blocks[y][x] = new_block(BLOCK_WALL, game);
			} else {
				world.blocks[y][x] = new_block(BLOCK_VOID, game);
			}
		}
	}
	for(int i = 0; i < 2; i++){
		int rx = 0;
		int ry = 0;
		if(i == 0){
			rx = gen_rand(44/2,44-5);
			ry = gen_rand(0,26/2);
		} else {
			rx = gen_rand(44/2,44-5);
			ry = gen_rand(30/2,29-5);
		}
		for(int y = ry; y < ry+5; y++){
			for(int x = rx; x < rx+5; x++){
				if(x == rx || x == rx+4 || y == ry || y == ry+4){
					world.blocks[y][x] = new_block(BLOCK_WALL, game);
					if(i == 0 && y == ry+4){
						world.blocks[y][x] = new_block(BLOCK_VOID, game);
					}
				}
			}
		}
		if(i == 0){
			world.blocks[ry+2][rx+2] = new_block(BLOCK_GENERATOR, game);
		} else {
			world.blocks[ry][rx+2] = new_block(BLOCK_DOOR, game);
			world.blocks[ry+2][rx+2] = new_block(BLOCK_EXIT, game);
		}
	}
	return world;
}

void update_world(World* world, Game* game){
	if(world->unlock == 1){
		for(int y = 0; y < 30; y++){
			for(int x = 0; x < 45; x++){
				if(world->blocks[y][x].type == BLOCK_DOOR){
					world->blocks[y][x] = new_block(BLOCK_VOID, game);
					world->unlock++;
					return;
				}
			}
		}
	}
}

void render_world(World* world, Game* game){
	Img floor = new_img(game->rend, "assets/blocks/floor.png", 0);
	for(int y = 0; y < 30; y++){
		for(int x = 0; x < 45; x++){
			render_img(game->rend, &floor, x*20, y*20, 20, 20);
			render_block(&world->blocks[y][x], game, x*20, y*20, 20, 20);
		}
	}
}
