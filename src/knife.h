#ifndef MOB 
#define MOB 

#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>

#include "otto-game.h"

#include "snake.h"

typedef struct {
	int init;
	int id;
	Img sprite;
	float x;
	float y;
	float w;
	float h;
	int angle;
	int life;
	int dead;
	int spd;
	Point target;
} Knife;

Knife new_knife(Game* game, int sx, int sy, int tx, int ty, int id){ // s=start t=target
void update_knife(Knife* knife, Game* game, Snake* snake){
void render_knife(Knife* knife, Game* game){
void push_knives(Game* game, Knife* knife, Knife* knives){
void pop_knives(Knife* knife, Knife* knives){

Knife* new_knives(){
void update_knives(Knife* knives, Game* game, Snake* snake){
void render_knives(Knife* knives, Game* game){

#endif

