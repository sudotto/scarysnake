#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>

#include "otto-game.h"
#include "apple.h"

#include "snake.h"

Tail new_tail(Game* game, int x, int y){
	Tail tail;
	tail.init = 1;
	tail.sprite = new_img(game->rend, "assets/tail.png", 1);
	tail.end_sprite = new_img(game->rend, "assets/end.png", 1);
	tail.x = x;
	tail.y = y;
	return tail;
}

void update_tail(Tail* tail, Game* game){
	switch(tail->facing){
		case UP:
			rotate_img(&tail->sprite, 180);
			rotate_img(&tail->end_sprite, 180);
			break;
		case DOWN:
			rotate_img(&tail->sprite, 0);
			rotate_img(&tail->end_sprite, 0);
			break;
		case LEFT:
			rotate_img(&tail->sprite, 90);
			rotate_img(&tail->end_sprite, 90);
			break;
		case RIGHT:
			rotate_img(&tail->sprite, 270);
			rotate_img(&tail->end_sprite, 270);
			break;
	}
}

void render_tail(Tail* tail, Game* game, int scale, int last){
	if(last){
		render_img(game->rend, &tail->end_sprite, tail->x * scale, tail->y * scale, scale, scale);
	} else {
		render_img(game->rend, &tail->sprite, tail->x * scale, tail->y * scale, scale, scale);
	}
}

Snake new_snake(Game* game){
	Snake snake;
	snake.sprite = new_img(game->rend, "assets/snake.png", 1);
	snake.chomp = new_sound("assets/chomp.wav");
	snake.x = 0;
	snake.y = 0;
	snake.facing = DOWN;
	snake.len = 1;
	snake.dead = 0;
	int pre_x = snake.x;
	int pre_y = snake.y;
	for(int i = 0; i < 50; i++){
		snake.tails[i] = new_tail(game, pre_x, pre_y);
	}
	return snake;
}

void control_snake(Snake* snake, Game* game){
	if(game->keystates[SDL_SCANCODE_W]){
		if(snake->facing != DOWN){
			snake->facing = UP;
		}
	} else if(game->keystates[SDL_SCANCODE_A]){
		if(snake->facing != RIGHT){
			snake->facing = LEFT;
		}
	} else if(game->keystates[SDL_SCANCODE_S]){
		if(snake->facing != UP){
			snake->facing = DOWN;
		}
	} else if(game->keystates[SDL_SCANCODE_D]){
		if(snake->facing != LEFT){
			snake->facing = RIGHT;
		}
	} else if(game->keystates[SDL_SCANCODE_P]){
		snake->len++;
		printf("%i\n", snake->len);
	}
}

void update_snake(Snake* snake, Game* game, Apple* apple){
	if(snake->x == apple->x && snake->y == apple->y){
		play_sound(&snake->chomp);
		snake->len++;
		apple->eaten = 1;
	}
	if(snake->x < 0 || snake->x >= 45 || snake->y < 0 || snake->y >= 30){
		snake->dead = 1;
		return;
	}
	int pre_x = snake->x;
	int pre_y = snake->y;
	int pre_face = snake->facing;
	switch(snake->facing){
		case UP:
			snake->y -= 1;
			rotate_img(&snake->sprite, 180);
			break;
		case DOWN:
			snake->y += 1;
			rotate_img(&snake->sprite, 0);
			break;
		case LEFT:
			snake->x -= 1;
			rotate_img(&snake->sprite, 90);
			break;
		case RIGHT:
			snake->x += 1;
			rotate_img(&snake->sprite, 270);
			break;
	}
	for(int i = 0; i < snake->len; i++){
		if(&snake->tails[i] == NULL){
			snake->tails[i] = new_tail(game, pre_x, pre_y);
		}
		int x_save = snake->tails[i].x;
		int y_save = snake->tails[i].y;
		int face_save = snake->tails[i].facing;
		snake->tails[i].x = pre_x;
		snake->tails[i].y = pre_y;
		snake->tails[i].facing = pre_face;
		pre_x = x_save;
		pre_y = y_save;
		pre_face = face_save;
		update_tail(&snake->tails[i], game);
		if(snake->tails[i].x == snake->x && snake->tails[i].y == snake->y){
			snake->dead = 1;
			return;
		}
	}
}

void render_snake(Snake* snake, Game* game){
	int scale = 20;
	render_img(game->rend, &snake->sprite, snake->x * scale, snake->y * scale, scale, scale);
	for(int i = 0; i < snake->len; i++){
		int last = 0;
		if(i == snake->len - 1){
			last = 1;
		}
		render_tail(&snake->tails[i], game, scale, last);
	}
}
