#include <stdio.h>
#include <math.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>

#include "otto-game.h"

#include "snake.h"
#include "knife.h"
#include "world.h"

#include "mob.h"

Mob new_mob(Game* game){
	Mob mob;
	mob.sprite = new_img(game->rend, "assets/spooky_monster.png", 0);
	mob.jumpscare_screen = new_img(game->rend, "assets/jumpscare.png", 0);
	mob.jumpscare_sound = new_sound("assets/jumpscare.wav");
	mob.x = 900;
	mob.y = 600;
	mob.w = 25;
	mob.h = 50;
	mob.dead = 0;
	mob.spd = 0.5;
	mob.cooldown = 100;
	mob.jumpscare = 0;
	return mob;
}

void update_mob(Mob* mob, Game* game, Snake* snake, Knife* knives, World* world){
	float dx = snake->x - mob->x;
	float dy = snake->y - mob->y;
	float d = fabs(dist(dx, dy));
	float scale = fabs(mob->spd / d);
	dx *= scale;
	dy *= scale;
	mob->x += dx;
	mob->y += dy;
	if(mob->cooldown == 0){
		new_knife(game, mob->x, mob->y, snake->x, snake->y, knives);
		mob->cooldown = 300;
	}
	mob->cooldown--;
	Rect snake_r = {snake->x, snake->y, 20, 20};
	Rect mob_r = {mob->x, mob->y, mob->w, mob->h};
	if(rect_in_rect(snake_r, mob_r)){
		play_sound(&mob->jumpscare_sound);
		snake->paralyse = 1;
		mob->jumpscare = 50;
	}
	for(int i = 0; i < snake->len; i++){
		Rect tail_r = {snake->tails[i].x, snake->tails[i].y, 20, 20};
		if(rect_in_rect(tail_r, mob_r)){
			snake->paralyse = 1;
			mob->jumpscare = 50;
		}
	}
	if(mob->jumpscare){
		mob->jumpscare--;
		if(!mob->jumpscare){
			stop_sound(&mob->jumpscare_sound);
			snake->dead = 1;
		}
	}
}

void render_mob(Mob* mob, Game* game){
	if(mob->jumpscare){
		render_img(game->rend, &mob->jumpscare_screen, 0, 0, 900, 600);
	} else {
		render_img(game->rend, &mob->sprite, mob->x, mob->y, mob->w, mob->h);
	}
}
