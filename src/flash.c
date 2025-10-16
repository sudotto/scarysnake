#include "otto-game.h"
#include "snake.h"

#include "flash.h"

Flash new_flash(Game* game){
	Flash flash;
	flash.overlay = new_img(game->rend, "assets/flashlight.png", 0);
	flash.batt = 1000;
	return flash;
}

void update_flash(Flash* flash, Game* game){
	if(flash->batt > 0){
		flash->batt--;
	}
}

void render_flash(Flash* flash, int x, int y, int angle, Game* game){
	flash->overlay.angle = angle;
	Img dimmer = new_img(game->rend, "assets/litterallyjustblack.png", 0);
	set_img_alpha(&dimmer, 100 - (flash->batt/10));
	render_img(game->rend, &flash->overlay, x - ((900*4)/2), y - ((600*4)/2), 900*4, 600*4);
	render_img(game->rend, &dimmer, x - 900, y - 600, 900*4, 600*4);
}
