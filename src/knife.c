#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>

#include "otto-game.h"

#include "snake.h"

#include "knife.h"

Knife new_knife(Game* game, int sx, int sy, int tx, int ty, int id){ // s=start t=target
	Knife knife;
	knife.init = 1;
	knife.id = 1;
	knife.sprite = new_img(game->rend, "assets/knife.png", 0);
	knife.x = sx;
	knife.y = sy;
	knife.w = 25;
	knife.h = 50;
	knife.dead = 0;
	knife.spd = 1.1;
	Point target = {tx, ty};
	knife.target = target;
	return knife;
}

void update_knife(Knife* knife, Game* game, Snake* snake){
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
		knife->dead = 1;
	}
	if(rect_in_rect(snake_r, knife_r)){
		knife->dead = 1;
		snake->len--;
	}
	for(int i = 0; i < snake->len; i++){
		Rect tail_r = {snake->tails[i].x, snake->tails[i].y, 20, 20};
		if(rect_in_rect(tail_r, knife_r)){
			knife->dead = 1;
			snake->len--;
		}
	}
}

void render_knife(Knife* knife, Game* game){
	rotate_img(&knife->sprite, knife->angle);
	render_img(game->rend, &knife->sprite, knife->x, knife->y, knife->w, knife->h);
}

void push_knives(Game* game, Knife* knife, Knife* knives){
	for(int i = 0; i < 100; i++){
		if(!knives[i].init){
			knives[i] = *knife;
		}
	}
}

void pop_knives(Knife* knife, Knife* knives){
	for(int i = 0; i < 100; i++){
		if(!knives[i].id == knife->id){
			for(int j = i; j < 100; j++){
				knives[i] = knives[i+1];
			}
			break;
		}
	}
}

Knife* new_knives(){
	Knife* knives = malloc(sizeof(Knife) * 100);
	for(int i = 0; i < 100; i++){
		knives[i].init = 0;
	}
}

void update_knives(Knife* knives, Game* game, Snake* snake){
	for(int i = 0; i < 100; i++){
		update_knife(&knives[i], game, snake);
	}
}

void render_knives(Knife* knives, Game* game){
	for(int i = 0; i < 100; i++){
		render_knife(&knives[i], game);
	}
}
