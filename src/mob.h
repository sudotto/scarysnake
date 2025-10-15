#ifndef MOB 
#define MOB 

#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>

#include "otto-game.h"
#include "knife.h"

#include "snake.h"

typedef struct {
	Img sprite;
	float x;
	float y;
	float w;
	float h;
	int dead;
	int spd;
	int cooldown;
} Mob;

Mob new_mob(Game* game);
void update_mob(Mob* mob, Game* game, Snake* snake, Knife* knives);
void render_mob(Mob* mob, Game* game);

#endif 
