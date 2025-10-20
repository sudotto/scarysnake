#ifndef MOB 
#define MOB 

#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>

#include "otto-game.h"
#include "knife.h"
#include "world.h"

#include "snake.h"

typedef struct {
	Img sprite;
	Img jumpscare_screen;
	Sound jumpscare_sound;
	float x;
	float y;
	float w;
	float h;
	int dead;
	float spd;
	int cooldown;
	int jumpscare;
} Mob;

Mob new_mob(Game* game);
void update_mob(Mob* mob, Game* game, Snake* snake, Knife* knives, World* world);
void render_mob(Mob* mob, Game* game);

#endif 
