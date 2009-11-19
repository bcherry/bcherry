// C Source File
// Created 3/22/2005; 3:44:12 PM

#include "all.h"
#define LEFT 32
#define RIGHT (LCD_WIDTH - 32 - LEFT)

void drawHud(void) {
	int x, y = 0, i;
	if(cam->hud > 1)
		y = 1 - cam->hud--;
	else if(cam->hud < 1) {
		y = -10 - cam->hud;
		if(++cam->hud > 0)
			cam->hud = 0;
	}

	GrayClipSprite32_TRANW_R(LEFT,y,12,gfx->HUD.left_C0,gfx->HUD.left_C1,v0,v1);
	GrayClipSprite32_TRANW_R(LEFT + 32,y,12,gfx->HUD.middle_C0,gfx->HUD.middle_C1,v0,v1);
	GrayClipSprite32_TRANW_R(RIGHT,y,12,gfx->HUD.right_C0,gfx->HUD.right_C1,v0,v1);

	for(i = 0; i < samus->items.tanks_owned; i++) {
		if(i < samus->items.tanks_full)
			GrayClipSprite8_TRANB_R(LEFT + 4 +(i%6)*5, i<6?y+1:y+5, 3, gfx->HUD.tankFull_C0, gfx->HUD.tankFull_C1, v0, v1);
		else
			GrayClipSprite8_TRANW_R(LEFT + 4 +(i%6)*5, i<6?y+1:y+5, 3, gfx->HUD.tankEmpty_C0, gfx->HUD.tankEmpty_C1, v0, v1);
	}

	x = LEFT + 34;
	GrayClipSprite8_TRANB_R(x, y+1, 7, gfx->HUD.numbers[(samus->HP/10)*2], gfx->HUD.numbers[(samus->HP/10)*2 + 1], v0, v1);
	GrayClipSprite8_TRANB_R(x + 7, y+1, 7, gfx->HUD.numbers[(samus->HP%10)*2], gfx->HUD.numbers[(samus->HP%10)*2 + 1], v0, v1);

	x = LEFT + 50;
	GrayClipSprite32_TRANW_R(x, y + 1, 8, gfx->HUD.weapons_C0, gfx->HUD.weapons_C1, v0, v1);


	/*if(samus->weapons.missile_ammo == 0)
		GrayFastFillRect_R(v0,v1,x,y+1,x+7,y+8,1);
	if(samus->weapons.superMissile_ammo == 0)
		GrayFastFillRect_R(v0,v1,x+9,y+1,x+16,y+8,1);
	if(samus->weapons.powerBomb_ammo == 0)
		GrayFastFillRect_R(v0,v1,x+18,y+1,x+26,y+8,1);*/

	int ammo = 0;
	if(samus->weapons.gun == &templates->weapons.missile) {
		GrayInvertRect2B(x,y+1,x+7,y+8,v0,v1);
		ammo = samus->weapons.missile_ammo;
	} else if(samus->weapons.gun == &templates->weapons.superMissile) {
		GrayInvertRect2B(x+9,y+1,x+16,y+8,v0,v1);
		ammo = samus->weapons.superMissile_ammo;
	} else if(samus->weapons.bomb == &templates->weapons.powerBomb) {
		GrayInvertRect2B(x+18,y+1,x+26,y+8,v0,v1);
		ammo = samus->weapons.powerBomb_ammo;
	} else
		return;

	x = LEFT + 78;

	GrayClipSprite8_TRANB_R(x, y+1, 7, gfx->HUD.numbers[(ammo/10)*2], gfx->HUD.numbers[(ammo/10)*2 + 1], v0, v1);
	GrayClipSprite8_TRANB_R(x + 7, y+1, 7, gfx->HUD.numbers[(ammo%10)*2], gfx->HUD.numbers[(ammo%10)*2 + 1], v0, v1);

}
