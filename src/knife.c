#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>

#include "otto-game.h"

#include "snake.h"

#include "knife.h"

Knife new_knife(Game* game, int sx, int sy, int tx, int ty, Knife* knives){ // s=start t=target
	Knife knife;
	knife.init = 1;
	knife.sprite = new_img(game->rend, "assets/knife.png", 0);
	knife.x = sx;
	knife.y = sy;
	knife.w = 25;
	knife.h = 50;
	knife.life = 512;
	knife.dead = 0;
	knife.spd = 2;
	Point target = {tx, ty};
	knife.target = target;
	knife.id = 0;
	knife.id = push_knife(game, &knife, knives);
	return knife;
}

void update_knife(Knife* knife, Game* game, Snake* snake, Knife* knives){
	knife->life--;
	if(knife->life <= 0){
		pop_knife(knife->id, knives);
	}
	float dx = knife->target.x - knife->x;
	float dy = knife->target.y - knife->y;
	float d = fabs(dist(dx, dy));
	float scale = fabs(knife->spd / d);
	dx *= scale;
	dy *= scale;
	knife->x += dx;
	knife->y += dy;
	knife->angle = atan(dy/dx) * 180 / 3.14159;
	Rect snake_r = {snake->x, snake->y, 20, 20};
	Rect knife_r = {knife->x, knife->y, knife->w, knife->h};
	if(knife->x == knife->target.x && knife->x == knife->target.y){
		pop_knife(knife->id, knives);
	}
	if(rect_in_rect(snake_r, knife_r)){
		pop_knife(knife->id, knives);
		snake->len--;
	} else {
		for(int i = 0; i < snake->len; i++){
			Rect tail_r = {snake->tails[i].x, snake->tails[i].y, 20, 20};
			if(rect_in_rect(tail_r, knife_r)){
				pop_knife(knife->id, knives);
				snake->len--;
				break;
			}
		}
	}
}

void render_knife(Knife* knife, Game* game){
	rotate_img(&knife->sprite, knife->angle);
	render_img(game->rend, &knife->sprite, knife->x, knife->y, knife->w, knife->h);
}

int end_of_knives(Knife* knives){
	for(int i = 0; i < 100; i++){
		if(!knives[i].init){
			return i;
		}
	}
	return 0;
}

int push_knife(Game* game, Knife* knife, Knife* knives){
	knives[end_of_knives(knives)] = *knife;
}

void pop_knife(int i, Knife* knives){
	for(i; knives[i].init; i++){
		knives[i] = knives[i + 1];
		knives[i].id = i;
	}
}

Knife* new_knives(Game* game){
	Knife* knives = malloc(sizeof(Knife) * 100);
	for(int i = 0; i < 100; i++){
		knives[i].sprite = new_img(game->rend, "assets/knife.png", 0);
		knives[i].init = 0;
	}
}

void update_knives(Knife* knives, Game* game, Snake* snake){
	for(int i = 0; i < 10; i++){
		if(knives[i].init){
			update_knife(&knives[i], game, snake, knives);
		}
	}
}

void render_knives(Knife* knives, Game* game){
	for(int i = 0; i < 100; i++){
		if(knives[i].init){
			render_knife(&knives[i], game);
		}
	}
}
