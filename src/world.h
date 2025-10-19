#ifndef WORLD
#define WORLD

typedef enum {
	FLOOR,
	WALL,
	DOOR,
	COMPUTER, // interact with this to make DOORs disapear
	GENERATOR // interact to turn the lights on and win
} Block_type;

typedef struct {
	
} Block;

// todo: add block_data_table
// and world shit etc...

typedef struct {
	
} World;

#endif
