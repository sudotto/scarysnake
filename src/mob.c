#include <stdio.h>
#include <math.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>

#include "otto-game.h"

#include "snake.h"

#include "mob.h"

int dist(int x_dist, int y_dist){
	return sqrt((pow(x_dist, 2) + pow(y_dist, 2)));
}

Mob new_mob(Game* game){
	Mob mob;
	mob.sprite = new_img(game->rend, "assets/spooky_monster.png", 0);
	mob.x = 100;
	mob.y = 100;
	mob.w = 50;
	mob.h = 50;
	mob.dead = 0;
	mob.spd = 1.1;
	return mob;
}

void update_mob(Mob* mob, Game* game, Snake* snake){
	float dx = snake->x - mob->x;
	float dy = snake->y - mob->y;
	float d = fabs(dist(dx, dy));
	float scale = fabs(mob->spd / d);
	dx *= scale;
	dy *= scale;
	mob->x += dx;
	mob->y += dy;
	if(mob->x == snake->x && mob->y == snake->y){
		mob->dead = true;
	}
	Point snake_p = {snake->x, snake->y};
	Rect mob_r = {mob->x, mob->y, mob->w, mob->h};
	if(point_in_rect(snake_p, mob_r)){
		snake->dead = 1;
	}
	for(int i = 0; i < snake->len; i++){
		Point tail_p = {snake->tails[i].x, snake->tails[i].y};
		if(point_in_rect(tail_p, mob_r)){
			snake->dead = 1;
		}
	}
}

void render_mob(Mob* mob, Game* game){
	render_img(game->rend, &mob->sprite, mob->x, mob->y, mob->w, mob->h);
}
