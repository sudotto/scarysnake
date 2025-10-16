#ifndef FLASH
#define FLASH

#include "otto-game.h"
#include "snake.h"

typedef struct {
	Img overlay;
	int batt;
} Flash;

Flash new_flash(Game* game);
void update_flash(Flash* flash, Game* game);
void render_flash(Flash* flash, int x, int y, int angle, Game* game);

#endif
