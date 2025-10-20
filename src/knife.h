#ifndef KNIFE 
#define KNIFE 

#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>

#include "otto-game.h"

#include "snake.h"
#include "world.h"

typedef struct {
	int init;
	int id;
	Img sprite;
	float x;
	float y;
	float w;
	float h;
	float dx;
	float dy;
	int angle;
	int life;
	int dead;
	int spd;
	Point target;
} Knife;

Knife new_knife(Game* game, int sx, int sy, int tx, int ty, Knife* knives); // s=start t=target
void update_knife(Knife* knife, Game* game, Snake* snake, Knife* knives, World* world);
void render_knife(Knife* knife, Game* game);
int end_of_knives(Knife* knives);
int push_knife(Game* game, Knife* knife, Knife* knives);
void pop_knife(int i, Knife* knives);

Knife* new_knives(Game* game);
void update_knives(Knife* knives, Game* game, Snake* snake, World* world);
void render_knives(Knife* knives, Game* game);

#endif

