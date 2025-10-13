#ifndef APPLE
#define APPLE

#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>

#include "otto-game.h"

typedef struct {
	Img sprite;
	int x;
	int y;
	int eaten;
} Apple;

Apple new_apple(Game* game);
void update_apple(Apple* apple, Game* game);
void render_apple(Apple* apple, Game* game);

#endif
