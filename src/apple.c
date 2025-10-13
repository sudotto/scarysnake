#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>

#include "otto-game.h"

#include "apple.h"

int better_rand(int min, int max){
	return rand() % (max - min + 1) + min;
}

Apple new_apple(Game* game){
	Apple apple;
	apple.sprite = new_img(game->rend, "assets/apple.png", 1);
	apple.x = better_rand(0,44);
	apple.y = better_rand(0,29);
	apple.eaten = 0;
	return apple;
}

void update_apple(Apple* apple, Game* game){
	if(apple->eaten){
		*apple = new_apple(game);
	}
}

void render_apple(Apple* apple, Game* game){
	int scale = 20;
	render_img(game->rend, &apple->sprite, apple->x * scale, apple->y * scale, scale, scale);
}
