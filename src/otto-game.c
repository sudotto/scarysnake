#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_audio.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "otto-game.h"
#include "otto-game_info.h"

///////////////////
// BAR
///////////////////

void render_bar(SDL_Renderer* rend, float val, int max, int x, int y, int w, int h){
	int scaled = max * (w / max);
	SDL_FRect empty = {x, y, scaled, h};
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
	SDL_RenderFillRect(rend, &empty);
	scaled = val * (w / max);
	SDL_FRect time = {x, y, scaled, h};
	SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
	SDL_RenderFillRect(rend, &time);
}

///////////////////
// UTILITY
///////////////////

int gen_rand(int min, int max){
	return rand() % (max - min) + min;
}

bool point_in_rect(Point point, Rect rect){
	if( point.x >= rect.x &&
		point.y >= rect.y &&
		point.x <= rect.x + rect.w &&
		point.y <= rect.y + rect.h){
		return true;
	}
	return false;
}

bool rect_in_rect(Rect rect, Rect rect2){
	Point TL = {rect.x, rect.y};
	Point TR = {rect.x + rect.w, rect.y};
	Point BL = {rect.x, rect.y + rect.h};
	Point BR = {rect.x + rect.w, rect.y + rect.h};
	if( point_in_rect(TL, rect2) ||
		point_in_rect(TR, rect2) ||
		point_in_rect(BL, rect2) ||
		point_in_rect(BR, rect2)){
		return true;
	}
	return false;
}

int dist(int x_dist, int y_dist){
	return sqrt((pow(x_dist, 2) + pow(y_dist, 2)));
}

///////////////////
// CAMERA
///////////////////

Camera camera = {0, 0, 1};

///////////////////
// IMAGE
///////////////////

Img new_img(SDL_Renderer* rend, char* file, bool camera_affected){
	Img img;
	img.surf = IMG_Load(file);
	img.tex = SDL_CreateTextureFromSurface(rend, img.surf);
	img.cropped = false;
	img.flipped = false;
	img.camera_affected = camera_affected;
	img.angle = 0;
	return img;
}

void crop_img(Img* img, int x, int y, int w, int h){
	img->crop.x = x;
	img->crop.y = y;
	img->crop.w = w;
	img->crop.h = h;
	img->cropped = true;
}

void flip_img(Img* img, bool flipped){
	img->flipped = flipped;
}

void rotate_img(Img* img, int angle){
	img->angle = angle;
}

void recolor_img(Img* img, SDL_Renderer* rend, SDL_Color target, SDL_Color replace){
	const SDL_PixelFormatDetails *format_details = SDL_GetPixelFormatDetails(img->surf->format);
	Uint32 replaceRGB = SDL_MapRGB(format_details, NULL, replace.r, replace.g, replace.b);
	Uint32 targetRGB = SDL_MapRGB(format_details, NULL, target.r, target.g, target.b);
	Uint32 *pixels = img->surf->pixels;
	int total = img->surf->w * img->surf->h;
	for(int i = 0; i < total; i++){
		if(pixels[i] == targetRGB){
			pixels[i] = replaceRGB;
		}
	}
	img->cropped = false;
	img->tex = SDL_CreateTextureFromSurface(rend, img->surf);
}

void set_img_alpha(Img* img, int alpha){
	SDL_SetTextureAlphaMod(img->tex, alpha);
}

void render_img(SDL_Renderer* rend, Img *img, int x, int y, int w, int h){
	SDL_FRect dest;
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;
	if(img->camera_affected){
		dest.x -= camera.x;
		dest.y -= camera.y;
		dest.x *= camera.scale;
		dest.y *= camera.scale;
		dest.w *= camera.scale;
		dest.h *= camera.scale;
	}
	if(x < 0 && y < 0 && x > GAME_W && y > GAME_H){
		return;
	}
	if(img->cropped){
		SDL_RenderTextureRotated(rend, img->tex, &img->crop, &dest, img->angle, NULL, img->flipped);
	} else {
		SDL_RenderTextureRotated(rend, img->tex, NULL, &dest, img->angle, NULL, img->flipped);
	}
}

///////////////////
// ANIMATION
///////////////////

Anim new_anim(SDL_Renderer* rend, char* filename, int framecount, int row, int w, int h, bool camera_affected){
	Anim anim;
	anim.frames = malloc(sizeof(Img) * framecount);
	anim.frame = 0;
	anim.framecount = framecount;
	for(int x = 0; x < framecount; x++){
		anim.frames[x] = new_img(rend, filename, camera_affected);
		crop_img(&anim.frames[x], x * w, row * h, w, h);
	}
	return anim;
}

void render_anim(SDL_Renderer* rend, Anim* anim, int x, int y, int w, int h, float framerate){
	anim->frame += framerate;
	if(anim->frame >= anim->framecount){
		anim->frame = 0;
	}
	int frame = (int)floor(anim->frame);
	render_img(rend, &anim->frames[(int)floor(anim->frame)], x, y, w, h);
}

void del_anim(Anim* anim){
	free(anim->frames);
}

///////////////////
// SOUNDS 
///////////////////

Sound new_sound(char* filename){
	Sound sound;
	SDL_LoadWAV(filename, &sound.spec, &sound.wav_data, &sound.wav_data_len);
	sound.stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &sound.spec, NULL, NULL);
	return sound;
}

void play_sound(Sound* sound){
	SDL_ResumeAudioStreamDevice(sound->stream);
	if(SDL_GetAudioStreamQueued(sound->stream) < (int)sound->wav_data_len){
		SDL_PutAudioStreamData(sound->stream, sound->wav_data, sound->wav_data_len);
	}
}

void stop_sound(Sound* sound){
	SDL_PauseAudioStreamDevice(sound->stream);
}

void quick_sound(char* filename){
	uint8_t* wav_data;
	uint32_t wav_data_len;
	SDL_AudioSpec spec;
	SDL_LoadWAV(filename, &spec, &wav_data, &wav_data_len);
	SDL_AudioStream* stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
	SDL_PutAudioStreamData(stream, wav_data, wav_data_len);
	SDL_ResumeAudioStreamDevice(stream);
}

///////////////////
// TEXT
///////////////////

Text new_text(SDL_Renderer* rend, char* msg){
	Text text;
	text.len = strlen(msg);
	SDL_Color clr = {255, 255, 255, 255};
	TTF_Font* font = TTF_OpenFont("assets/font.ttf", 100);
	text.surf = TTF_RenderText_Solid(font, msg, text.len, clr);
	text.tex = SDL_CreateTextureFromSurface(rend, text.surf);
	TTF_CloseFont(font);
	return text;
}

void render_text(Text* text, SDL_Renderer* rend, int x, int y, int c_w, int c_h){
	SDL_FRect dest;
	dest.x = x;
	dest.y = y;
	dest.w = c_w * text->len;
	dest.h = c_h;
	/*if(text->camera_affected){
		dest.x -= camera.x;
		dest.y -= camera.y;
		dest.x *= camera.scale;
		dest.y *= camera.scale;
		dest.w *= camera.scale;
		dest.h *= camera.scale;
	}
	if(x < 0 && y < 0 && x > GAME_W && y > GAME_H){
		return;
	}*/ 
	SDL_RenderTextureRotated(rend, text->tex, NULL, &dest, 0, NULL, 0);
}

///////////////////
// GAME
///////////////////

Game new_game(char* title, int w, int h){
	Game game = {0};

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	TTF_Init();
	game.win = SDL_CreateWindow(title, w, h, 0);
	game.rend = SDL_CreateRenderer(game.win, NULL);
	//game.text_eng = TTF_CreateRendererTextEngine(game.rend);

	game.icon = new_img(game.rend, "assets/icon.png", false);
	game.cursor = new_img(game.rend, "assets/cursor.png", false);
	SDL_SetWindowIcon(game.win, game.icon.surf);

	game.keystates = SDL_GetKeyboardState(NULL);
	game.mousestates = SDL_GetMouseState(&game.mouse_x, &game.mouse_y);
	SDL_HideCursor();

	game.frame_start;
	game.running = true;
	return game;
}

void cap_game_framerate(Game* game, Uint8 fps){
	Uint32 frame_time = SDL_GetTicks() - game->frame_start;
	if(frame_time < 1000/fps){
		SDL_Delay(1000/fps - frame_time);
	}
}

bool get_game_events(Game* game){
	game->keystates = SDL_GetKeyboardState(NULL);
	game->mousestates = SDL_GetMouseState(&game->mouse_x, &game->mouse_y);
	if(SDL_PollEvent(&game->event)){
		return true;
	}
	return false;
}

void render_game_cursor(Game* game, int w, int h){
	render_img(game->rend, &game->cursor, game->mouse_x, game->mouse_y, w, h);
}

void fill_game(Game* game, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
	SDL_SetRenderDrawBlendMode(game->rend, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(game->rend, r, g, b, a);
}

void clear_game(Game* game, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
	SDL_SetRenderDrawColor(game->rend, r, g, b, a);
	SDL_RenderClear(game->rend);
}

void update_game(Game* game){
	SDL_RenderPresent(game->rend);
}

void destroy_game(Game* game){
	SDL_DestroyRenderer(game->rend);
	SDL_DestroyWindow(game->win);
}
