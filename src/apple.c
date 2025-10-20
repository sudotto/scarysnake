#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>

#include "otto-game.h"

#include "world.h"

#include "apple.h"

Apple new_apple(Game* game, World* world){
	Apple apple;
	apple.sprite = new_img(game->rend, "assets/apple.png", 1);
	while(1){
		apple.x = gen_rand(0,44)*20;
		apple.y = gen_rand(0,29)*20;
		if(!world->blocks[apple.y/20][apple.x/20].solid){
			break;
		}
	}
	apple.eaten = 0;
	return apple;
}

void update_apple(Apple* apple, Game* game, World* world){
	if(apple->eaten){
		*apple = new_apple(game, world);
	}
}

void render_apple(Apple* apple, Game* game){
	int scale = 20;
	render_img(game->rend, &apple->sprite, apple->x, apple->y, scale, scale);
}
