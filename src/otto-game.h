#ifndef OTTOGAME
#define OTTOGAME

#include <stdio.h>
#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>

// Otto-game:
// because sdl is a shitty esoteric inbred pile of diseased badly documented gas soaked and burning shit
// (its not that bad im just fucking stupid :) )

///////////////////
// BAR
///////////////////

void render_bar(SDL_Renderer* rend, float val, int max, int x, int y, int w, int h);

///////////////////
// UTILITY
///////////////////

int gen_rand(int min, int max);

typedef struct {
	int x;
	int y;
} Point;

typedef struct {
	int x;
	int y;
	int w;
	int h;
} Rect;

bool point_in_rect(Point point, Rect rect);
bool rect_in_rect(Rect rect, Rect rect2);
int dist(int x_dist, int y_dist);

///////////////////
// CAMERA
///////////////////

typedef struct {
	int x;
	int y;
	float scale;
} Camera;

extern Camera camera;

///////////////////
// IMAGE
///////////////////

typedef struct {
	SDL_Surface* surf;
	SDL_Texture* tex;
	SDL_FRect crop;
	bool cropped;
	bool flipped;
	bool camera_affected;
	int angle;
} Img;

Img new_img(SDL_Renderer* rend, char* file, bool camera_affected);
void crop_img(Img* img, int x, int y, int w, int h);
void flip_img(Img* img, bool flipped);
void rotate_img(Img* img, int angle);
void recolor_img(Img* img, SDL_Renderer* rend, SDL_Color target, SDL_Color replace);
void set_img_alpha(Img* img, int alpha);
void render_img(SDL_Renderer* rend, Img *img, int x, int y, int w, int h);

///////////////////
// ANIMATIONS
///////////////////

typedef struct {
	Img* frames;
	float frame;
	int framecount;
} Anim;

Anim new_anim(SDL_Renderer* rend, char* filename, int framecount, int row, int w, int h, bool camera_affected);
void render_anim(SDL_Renderer* rend, Anim* anim, int x, int y, int w, int h, float framerate);
void del_anim(Anim* anim);

///////////////////
// SOUNDS 
///////////////////

typedef struct {
	uint8_t* wav_data;
	uint32_t wav_data_len;
	SDL_AudioSpec spec;
	SDL_AudioStream* stream;
} Sound;

Sound new_sound(char* filename);
void play_sound(Sound* sound);
void stop_sound(Sound* sound);
void quick_sound(char* filename);

///////////////////
// GAME
///////////////////

typedef struct {
	SDL_Window* win;
	SDL_Renderer* rend;
	SDL_Event event;
	Img icon;
	Img cursor;
	const bool* keystates;
	SDL_MouseButtonFlags mousestates;
	float mouse_x;
	float mouse_y;
	float old_mouse_x;
	float old_mouse_y;
	Uint32 frame_start;
	bool running;
} Game;

Game new_game(char* title, int w, int h);
void cap_game_framerate(Game* game, Uint8 fps);
bool get_game_events(Game* game);
void render_game_cursor(Game* game, int w, int h);
void clear_game(Game* game, Uint8 r, Uint8 g, Uint8 b);
void update_game(Game* game);
void destroy_game(Game* game);

#endif
