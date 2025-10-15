#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_audio.h>

#include "otto-game.h"

#include "snake.h"
#include "apple.h"
#include "mob.h"
#include "knife.h"

typedef enum {
	MENU,
	PLAYING,
	DEAD
} Gamestate;

void menu(Gamestate* state, Game* game){
	Img tip = new_img(game->rend, "assets/menu.png", 1);
	clear_game(game, 0, 0, 0);
	render_img(game->rend, &tip, 0,0,900,600);
	render_game_cursor(game, 32, 32);
	update_game(game);
}

void playing(Gamestate* state, int* updates, Snake* dog, Game* game, Apple* mango, Mob* mob){
	if(*updates == 10){
		update_snake(dog, game, mango);
		if(dog->dead){
			*state = DEAD;
		}
		update_apple(mango, game);
		*updates = 0;
	}
	update_mob(mob, game, dog);
	clear_game(game, 0, 0, 0);
	render_snake(dog, game);
	render_apple(mango, game);
	render_mob(mob, game);
	//render_game_cursor(&game, 32, 32);
	update_game(game);
	*updates += 1;
}

void dead(Gamestate* state, Game* game){
	Img tip = new_img(game->rend, "assets/death_screen.png", 1);
	clear_game(game, 0, 0, 0);
	render_img(game->rend, &tip, 0,0,900,600);
	render_game_cursor(game, 32, 32);
	update_game(game);
}

int main(){
	srand(time(0));
	Game game = new_game("snake if it was scarier", 900, 600);
	Snake dog = new_snake(&game);
	Apple mango = new_apple(&game);
	Mob test = new_mob(&game);
	Knife* knives = new_knives();
	Sound click = new_sound("assets/click.wav");
	Sound music = new_sound("assets/music.wav");
	Sound loser = new_sound("assets/loser.wav");
	Gamestate state = MENU;
	int updates = 0; //the fast fix not the smart fix
	while(game.running){
		game.frame_start = SDL_GetTicks();
		while(get_game_events(&game)){
			switch(game.event.type){
				case SDL_EVENT_QUIT:
					game.running = 0;
					break;
			}
			if(game.keystates[SDL_SCANCODE_ESCAPE]){
				game.running = 0;
			}
			switch(state){
				case MENU:
					play_sound(&music);
					if(game.keystates[SDL_SCANCODE_SPACE]){
						play_sound(&click);
						stop_sound(&music);
						state = PLAYING;
						dog = new_snake(&game);
					}
					break;
				case PLAYING:
					control_snake(&dog, &game);
					break;
				case DEAD:
					play_sound(&loser);
					if(game.keystates[SDL_SCANCODE_SPACE]){
						play_sound(&click);
						stop_sound(&loser);
						state = MENU;
					}
					break;
			}
		}
		switch(state){
			case MENU:
				menu(&state, &game);
				break;
			case PLAYING:
				playing(&state, &updates, &dog, &game, &mango, &test);
				break;
			case DEAD:
				dead(&state, &game);
				break;
		}
		cap_game_framerate(&game, 60);
	}
	destroy_game(&game);
}
