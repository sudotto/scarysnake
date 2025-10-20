#include "otto-game.h"

#include "world.h"

#include "flash.h"

Battery new_battery(Game* game, World* world){
	Battery battery;
	battery.sprite = new_img(game->rend, "assets/battery.png", 1);
	while(1){
		battery.x = gen_rand(0,44)*20;
		battery.y = gen_rand(0,29)*20;
		if(!world->blocks[battery.y/20][battery.x/20].solid){
			break;
		}
	}
	battery.gotten = 0;
	return battery;
}

void update_battery(Battery* battery, Game* game, World* world){
	if(battery->gotten){
		*battery = new_battery(game, world);
	}
}

void render_battery(Battery* battery, Game* game){
	int scale = 20;
	render_img(game->rend, &battery->sprite, battery->x, battery->y, scale, scale);
}

Flash new_flash(Game* game){
	Flash flash;
	flash.overlay = new_img(game->rend, "assets/flashlight.png", 0);
	flash.max = 255;
	flash.batt = flash.max;
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
	set_img_alpha(&dimmer, 255 - (flash->batt));
	render_img(game->rend, &flash->overlay, x - ((900*4)/2), y - ((600*4)/2), 900*4, 600*4);
	render_img(game->rend, &dimmer, x - 900, y - 600, 900*4, 600*4);
}
