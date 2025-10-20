#ifndef FLASH
#define FLASH

#include "otto-game.h"

#include "world.h"

typedef struct {
	Img sprite;
	int x;
	int y;
	int gotten;
} Battery;

Battery new_battery(Game* game, World* world);
void update_battery(Battery* battery, Game* game, World* world);
void render_battery(Battery* battery, Game* game);

typedef struct {
	Img overlay;
	int batt;
	int max;
} Flash;

Flash new_flash(Game* game);
void update_flash(Flash* flash, Game* game);
void render_flash(Flash* flash, int x, int y, int angle, Game* game);

#endif
