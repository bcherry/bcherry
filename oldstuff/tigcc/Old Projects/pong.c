// C Source File
// Created 18/Nov/03; 17:43:41

#define USE_TI89              // Compile for TI-89

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files
#include "pong.h"
#include "extgraph.h"

short int noWall(char axis, int pos, int change) {
	if (axis == 'x') {
		if (change < 0) {
			if (pos + change >= 0) return 1;
		}
		if (change > 0) {
			if (pos + change <= 152) return 1;
		}
	}
	if (axis == 'y') {
		if (change < 0) {
			if (pos + change >= 0) return 1;
		}
		if (change > 0) {
			if (pos + change <= 92) return 1;
		}
	}
	return 0;
}

short int quit(void) {
	if (_rowread(TI89_ESCROW) & TI89_ESCKEY) {
		die = 0;
		return 1;
	}	
	return 0;
}

// delay program
inline void delay(unsigned int number) {
	unsigned short loop, loop2;
	for (loop=0;loop<number;loop++) {
		for (loop2=0;loop2<10000;loop2++);
	}
}

short int hitPaddle(BALL ball, PADDLE player, PADDLE computer, int secret_on) {
	if (secret_on) {
		if (TestCollide8(ball.x_pos+ball.x_change,ball.y_pos,player.x_pos,player.y_pos+8,16,ball_sprite, secret_sprite)) return 1;
		if (TestCollide8(ball.x_pos+ball.x_change,ball.y_pos,computer.x_pos,computer.y_pos+8,16,ball_sprite, secret_sprite)) return 2;
	} else {
		if (TestCollide8(ball.x_pos+ball.x_change,ball.y_pos,player.x_pos,player.y_pos+8,16,ball_sprite, paddle_sprite)) return 1;
		if (TestCollide8(ball.x_pos+ball.x_change,ball.y_pos,computer.x_pos,computer.y_pos+8,16,ball_sprite, paddle_sprite)) return 2;
	}
	return 0;
}

short int paddle_y_change(PADDLE player, PADDLE computer, int hit_paddle, BALL ball) {
	int new_y_change = ball.y_change;
	int change = 0;
	if (hit_paddle == 1) {
		int diff = (ball.y_pos + 4)-(player.y_pos);
		if (diff > 16) change++;
		if (diff < 0) change--;
		new_y_change = ball.y_change + change;
	} else if (hit_paddle == 2) {
		int diff = (ball.y_pos + 4)-(computer.y_pos);
		if (diff > 16) change++;
		if (diff < 0) change--;
		new_y_change = ball.y_change + change;
	}
	return new_y_change;
}

// Main Function
void _main(void)
{		
	int secret_on = 0;
	die = 1;
	ClrScr();
	FontSetSys(F_4x6);
	DrawStr(41,95,"Pong v 1.0 by Ben Cherry",A_NORMAL);
	Sprite8_XOR(76,0,8,ball_sprite,LCD_MEM);
	Sprite8_XOR(0,42,16,paddle_sprite,LCD_MEM);
	Sprite8_XOR(152,42,16,paddle_sprite,LCD_MEM);
	if (ngetchx() == KEY_F1) {
		if (ngetchx() == KEY_UP) {
			if (ngetchx() == KEY_DOWN) {
				if (ngetchx() == KEY_LEFT) {
					if (ngetchx() == KEY_RIGHT) {
						secret_on = 1;
						Sprite8_XOR(0,42,16,paddle_sprite,LCD_MEM);
						Sprite8_XOR(152,42,16,paddle_sprite,LCD_MEM);
						Sprite8_XOR(0,42,16,secret_sprite,LCD_MEM);
						Sprite8_XOR(152,42,16,secret_sprite,LCD_MEM);
						FontSetSys(F_4x6);                     
						DrawStr(29,95,"PENIS PONG!! v 1.0 by Ben Cherry",A_REPLACE);
					}
				}
			}
		}
	}
	randomize();
	
	interrupt1 = GetIntVec(AUTO_INT_1);
	SetIntVec(AUTO_INT_1,DUMMY_HANDLER);
	
	if (!secret_on) DrawStr(41,95,"Pong v 1.0 by Ben Cherry",A_NORMAL);
	else DrawStr(29,95,"PENIS PONG!! v 1.0 by Ben Cherry",A_REPLACE);
	int difficulty = 3;
	int shot = 0;
	int r = random(2);
	if (r == 0) r=-1;
	BALL ball = {76,0,r,1};
	PADDLE player = {0,42};
	PADDLE computer = {152,42};
	SECRET sperm = {0,-1,0};
	int key = 0;
	int just = 0;
	int hit_paddle = 0;
	int game_speed = 15;
	while (!quit()) {
		key = _rowread(ARROW_ROW);
		
		if (shot && secret_on) {
			if (!just) {
				if (sperm.type == 1) Sprite8_XOR(0,sperm.y_pos,8,sperm_sprite,LCD_MEM);
				else Sprite8_XOR(0,sperm.y_pos,8,jizz_sprite,LCD_MEM);
				sperm.y_pos+=sperm.y_change;			
			} else just = 0;
			if (sperm.y_pos<0) shot=0;
			if (shot) {if (sperm.type == 1) Sprite8_XOR(0,sperm.y_pos,8,sperm_sprite,LCD_MEM);
				else Sprite8_XOR(0,sperm.y_pos,8,jizz_sprite,LCD_MEM);}
		}
		
		if (secret_on) Sprite8_XOR(player.x_pos,player.y_pos,16,secret_sprite,LCD_MEM);
	  else Sprite8_XOR(player.x_pos,player.y_pos,16,paddle_sprite,LCD_MEM);
		if (secret_on) Sprite8_XOR(computer.x_pos,computer.y_pos,16,secret_sprite,LCD_MEM);
		else Sprite8_XOR(computer.x_pos,computer.y_pos,16,paddle_sprite,LCD_MEM);
		
		Sprite8_XOR(ball.x_pos,ball.y_pos,8,ball_sprite,LCD_MEM);
		if ((hit_paddle = hitPaddle(ball,player,computer, secret_on))) {
			ball.x_change*=-1; ball.y_change=paddle_y_change(player,computer,hit_paddle,ball);
			if (key & 0x01) ball.y_change--;
			if (key & 0x04) ball.y_change++;
		}	else ball.x_pos+=ball.x_change;
		if (noWall('y',ball.y_pos,ball.y_change)) ball.y_pos+=ball.y_change;
		else ball.y_change*=-1;
		
		if (ball.y_pos > computer.y_pos && computer.y_pos + difficulty <= 84 && ball.x_pos > 80 && random(10) < 6) computer.y_pos+=difficulty;
		if (ball.y_pos < computer.y_pos && computer.y_pos - difficulty >= 0 && ball.x_pos > 80 && random(10) < 6) computer.y_pos-=difficulty;
		
		if (ball.x_pos < 3 || ball.x_pos >149) break;

		if (key & 0x10/*SECOND*/ && !shot) {shot = 1; just = 1; sperm.type = 2; sperm.y_pos = player.y_pos;}
		if (key & 0x40/*DIAMOND*/ && !shot) {shot = 1; just = 1; sperm.type = 1; sperm.y_pos = player.y_pos;}
		
		if (key & 0x01/*UP*/ && player.y_pos - 3 >= 0) player.y_pos-=1;
		if (key & 0x04/*DOWN*/ && player.y_pos + 3 <= 84) player.y_pos+=1;
		
		if (secret_on) Sprite8_XOR(player.x_pos,player.y_pos,16,secret_sprite,LCD_MEM);
	  else Sprite8_XOR(player.x_pos,player.y_pos,16,paddle_sprite,LCD_MEM);
		if (secret_on) Sprite8_XOR(computer.x_pos,computer.y_pos,16,secret_sprite,LCD_MEM);
		else Sprite8_XOR(computer.x_pos,computer.y_pos,16,paddle_sprite,LCD_MEM);
		
		Sprite8_XOR(ball.x_pos,ball.y_pos,8,ball_sprite,LCD_MEM);
		delay(game_speed);
	}
	SetIntVec(AUTO_INT_1,interrupt1);
	if (secret_on) Sprite8_OR(player.x_pos,player.y_pos,16,secret_sprite,LCD_MEM);
	else Sprite8_OR(player.x_pos,player.y_pos,16,paddle_sprite,LCD_MEM);
	if (secret_on) Sprite8_OR(computer.x_pos,computer.y_pos,16,secret_sprite,LCD_MEM);
	else Sprite8_OR(computer.x_pos,computer.y_pos,16,paddle_sprite,LCD_MEM);
	Sprite8_OR(ball.x_pos,ball.y_pos,8,ball_sprite,LCD_MEM);
	if (shot && secret_on){if (sperm.type == 1) Sprite8_XOR(0,sperm.y_pos,8,sperm_sprite,LCD_MEM);
				else Sprite8_XOR(0,sperm.y_pos,8,jizz_sprite,LCD_MEM);}
	
	FontSetSys(F_4x6);
	if (secret_on) DrawStr(29,95,"PENIS PONG!! v 1.0 by Ben Cherry",A_NORMAL);
	else DrawStr(41,95,"Pong v 1.0 by Ben Cherry",A_NORMAL);
	FontSetSys(F_6x8);
	GKeyFlush();
	ngetchx();
	GKeyFlush();
	if (die) _main();
}
