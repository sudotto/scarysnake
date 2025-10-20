#ifndef SNAKE
#define SNAKE

#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>

#include "otto-game.h"

#include "apple.h"
#include "flash.h"
#include "world.h"

typedef enum {
	UP,
	DOWN,
	LEFT,
	RIGHT
} Facing;

typedef struct {
	Img sprite;
	Img end_sprite;
	int x;
	int y; 
	Facing facing;
	int init;
} Tail;

Tail new_tail(Game* game, int start_x, int start_y);
Tail empty_tail();
void update_tail(Tail* tail, Game* game);
void render_tail(Tail* tail, Game* game, int scale, int last);

typedef struct {
	Img sprite;
	Sound chomp;
	int x;
	int y; 
	Facing facing;
	int len;
	Tail tails[50];
	int dead;
	int paralyse;
	Flash flash;
} Snake;

Snake new_snake(Game* game);
void control_snake(Snake* snake, Game* game);
void update_snake(Snake* snake, Game* game, Apple* apple, Battery* battery, World* world);
void render_snake(Snake* snake, Game* game);

#endif 
