#ifndef WORLD
#define WORLD

#define BLOCK_TYPE_COUNT 5

#include "otto-game.h"

typedef enum {
	BLOCK_VOID,
	BLOCK_WALL,
	BLOCK_DOOR,
	BLOCK_GENERATOR, // interact with this to make DOORs disapear
	BLOCK_EXIT       // interact to turn the lights on and win
} Block_type;

typedef struct {
	char* filename;
	int solid;
} Block_data;

extern Block_data block_data_table[BLOCK_TYPE_COUNT];

typedef struct {
	Block_type type;
	Img sprite;
	int solid;
} Block;

Block new_block(Block_type type, Game* game);
void render_block(Block* block, Game* game, int x, int y, int w, int h);

typedef struct {
	Block blocks[30][45];
	int unlock;
	int win;
} World;

World new_world(Game* game);
void update_world(World* world, Game* game);
void render_world(World* world, Game* game); 

#endif
