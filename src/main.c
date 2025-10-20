#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_audio.h>

#include "otto-game.h"

#include "snake.h"
#include "apple.h"
#include "mob.h"
#include "knife.h"
#include "flash.h"
#include "world.h"

typedef enum {
	STATE_MENU,
	STATE_PLAYING,
	STATE_DEAD,
	STATE_WIN
} Gamestate;

void menu(Gamestate* state, Game* game){
	Img tip = new_img(game->rend, "assets/menu.png", 1);
	clear_game(game, 0, 0, 0, 255);
	render_img(game->rend, &tip, 0,0,900,600);
	render_game_cursor(game, 32, 32);
}

void playing(Gamestate* state, int* updates, Snake* dog, Game* game, Apple* mango, Mob* mob, Knife* knives, Flash* flash, Battery* battery, World* world){
	if(*updates == 10){
		update_snake(dog, game, mango, battery, world);
		update_apple(mango, game, world);
		update_battery(battery, game, world);
		*updates = 0;
	}
	update_mob(mob, game, dog, knives, world);
	update_knives(knives, game, dog, world);
	update_world(world, game);
	clear_game(game, 0, 0, 0, 255);

	render_world(world, game);
	render_snake(dog, game);
	render_apple(mango, game);
	render_battery(battery, game);
	render_knives(knives, game);
	render_mob(mob, game);
	if(!mob->jumpscare && !world->unlock){
		render_flash(&dog->flash, dog->x, dog->y, dog->sprite.angle, game);
		render_bar(game->rend, dog->flash.batt / 4, dog->flash.max / 4, 10, 10, dog->flash.max / 4, 12);
	} else {
		Img lightfx = new_img(game->rend, "assets/lights.png", 0);
		render_img(game->rend, &lightfx, 0, 0, 900, 600);
	}
	*updates += 1;
}

void dead(Gamestate* state, Game* game){
	Img tip = new_img(game->rend, "assets/death_screen.png", 1);
	clear_game(game, 0, 0, 0, 255);
	render_img(game->rend, &tip, 0,0,900,600);
	render_game_cursor(game, 32, 32);
}

void win(Gamestate* state, Game* game){
	Img tip = new_img(game->rend, "assets/win_screen.png", 1);
	clear_game(game, 0, 0, 0, 255);
	render_img(game->rend, &tip, 0,0,900,600);
	render_game_cursor(game, 32, 32);
}

int main(){
	srand(time(0));
	Game game = new_game("snake if it was scarier", 900, 600);
	World world;
	Flash flash;
	Snake dog;
	Apple mango;
	Mob monster;
	Battery battery;
	Knife knives[100];
	Text hint1 = new_text(game.rend, "Turn on the generator!");
	Text hint2 = new_text(game.rend, "Find the exit! (its shirtless anthony mackie i wanted to change it but i forgot)");
	Sound click = new_sound("assets/click.wav");
	Sound music = new_sound("assets/music.wav");
	Sound loser = new_sound("assets/loser.wav");
	Sound winner = new_sound("assets/yippee.wav");
	Sound generator = new_sound("assets/generator.wav");
	Gamestate state = STATE_MENU;
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
				case STATE_MENU:
					//if(SDL_GetAudioStreamQueued(music.stream) < (int)music.wav_data_len){
					play_sound(&music);
					//}
					if(game.keystates[SDL_SCANCODE_SPACE]){
						play_sound(&click);
						stop_sound(&music);
						state = STATE_PLAYING;
						dog = new_snake(&game);
						mango = new_apple(&game, &world);
						monster = new_mob(&game);
						flash = new_flash(&game);
						battery = new_battery(&game, &world);
						world = new_world(&game);
					}
					break;
				case STATE_PLAYING:
					control_snake(&dog, &game);
					break;
				case STATE_DEAD:
					play_sound(&loser);
					if(game.keystates[SDL_SCANCODE_SPACE]){
						play_sound(&click);
						stop_sound(&loser);
						state = STATE_MENU;
					}
					break;
				case STATE_WIN:
					play_sound(&winner);
					if(game.keystates[SDL_SCANCODE_SPACE]){
						play_sound(&click);
						stop_sound(&winner);
						state = STATE_MENU;
					}
					break;
			}
		}
		switch(state){
			case STATE_MENU:
				menu(&state, &game);
				break;
			case STATE_PLAYING:
				play_sound(&music);
				playing(&state, &updates, &dog, &game, &mango, &monster, knives, &flash, &battery, &world);
				// i dont care that i could do it better i don't want to work on this anymore -otto 22:40 oct 19 2025
				if(world.unlock){
					render_text(&hint2, game.rend, (900 / 2) - (10 * hint2.len)/2, 600-20, 10, 15);
					play_sound(&generator);                                    
				} else {                                                       
					render_text(&hint1, game.rend, (900 / 2) - (10 * hint1.len)/2, 600-20, 10, 15);
				}
				if(dog.dead){
					stop_sound(&generator);
					stop_sound(&music);
					state = STATE_DEAD;
				}
				if(world.win){
					stop_sound(&generator);
					stop_sound(&music);
					state = STATE_WIN;
				}
				break;
			case STATE_DEAD:
				dead(&state, &game);
				break;
			case STATE_WIN:
				win(&state, &game);
				break;
		}
		update_game(&game);
		cap_game_framerate(&game, 60);
	}
}
