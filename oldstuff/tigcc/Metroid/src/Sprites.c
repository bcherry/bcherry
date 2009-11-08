// C Source File
// Created 3/5/2005; 4:27:54 PM

#include "all.h"

void makeSprites(void) {
	if((sprites = malloc(sizeof(struct sprites))) == NULL)
		exit(0);
	struct sprites s = {
		{
			{8,8,&gfx->samus.ball1_C0,&gfx->samus.ball1_C1,NULL,&sprites->samus.ball2},
			{8,8,&gfx->samus.ball2_C0,&gfx->samus.ball2_C1,NULL,&sprites->samus.ball3},
			{8,8,&gfx->samus.ball3_C0,&gfx->samus.ball3_C1,NULL,&sprites->samus.ball1},
			{20,12,&gfx->samus.leftFall_C0,&gfx->samus.leftFall_C1,NULL,&sprites->samus.leftFall},
			{15,14,&gfx->samus.leftFlip1_C0,&gfx->samus.leftFlip1_C1,NULL,&sprites->samus.leftFlip2},
			{15,15,&gfx->samus.leftFlip2_C0,&gfx->samus.leftFlip2_C1,NULL,&sprites->samus.leftFlip3},
			{15,14,&gfx->samus.leftFlip3_C0,&gfx->samus.leftFlip3_C1,NULL,&sprites->samus.leftFlip4},
			{15,15,&gfx->samus.leftFlip4_C0,&gfx->samus.leftFlip4_C1,NULL,&sprites->samus.leftFlip1},
			{26,13,&gfx->samus.leftHurt_C0,&gfx->samus.leftHurt_C1,NULL,&sprites->samus.leftHurt},
			{24,10,&gfx->samus.leftJump1_C0,&gfx->samus.leftJump1_C1,NULL,&sprites->samus.leftJump2},
			{25,13,&gfx->samus.leftJump2_C0,&gfx->samus.leftJump2_C1,NULL,&sprites->samus.leftJump2},
			{18,12,&gfx->samus.leftKneel_C0,&gfx->samus.leftKneel_C1,NULL,&sprites->samus.leftKneel},
			{25,12,&gfx->samus.leftLand_C0,&gfx->samus.leftLand_C1,NULL,&sprites->samus.leftStand},
			{12,9,&gfx->samus.leftMorph_C0,&gfx->samus.leftMorph_C1,NULL,&sprites->samus.ball1},
			{23,10,&gfx->samus.leftRun1_C0,&gfx->samus.leftRun1_C1,NULL,&sprites->samus.leftRun2},
			{24,16,&gfx->samus.leftRun2_C0,&gfx->samus.leftRun2_C1,NULL,&sprites->samus.leftRun3},
			{23,16,&gfx->samus.leftRun3_C0,&gfx->samus.leftRun3_C1,NULL,&sprites->samus.leftRun4},
			{23,12,&gfx->samus.leftRun4_C0,&gfx->samus.leftRun4_C1,NULL,&sprites->samus.leftRun5},
			{23,12,&gfx->samus.leftRun5_C0,&gfx->samus.leftRun5_C1,NULL,&sprites->samus.leftRun1},
			{23,10,&gfx->samus.leftStand_C0,&gfx->samus.leftStand_C1,NULL,&sprites->samus.leftStand},
			{20,12,&gfx->samus.rightFall_C0,&gfx->samus.rightFall_C1,NULL,&sprites->samus.rightFall},
			{15,14,&gfx->samus.rightFlip1_C0,&gfx->samus.rightFlip1_C1,NULL,&sprites->samus.rightFlip2},
			{15,15,&gfx->samus.rightFlip2_C0,&gfx->samus.rightFlip2_C1,NULL,&sprites->samus.rightFlip3},
			{15,14,&gfx->samus.rightFlip3_C0,&gfx->samus.rightFlip3_C1,NULL,&sprites->samus.rightFlip4},
			{15,15,&gfx->samus.rightFlip4_C0,&gfx->samus.rightFlip4_C1,NULL,&sprites->samus.rightFlip1},
			{26,13,&gfx->samus.rightHurt_C0,&gfx->samus.rightHurt_C1,NULL,&sprites->samus.rightHurt},
			{24,10,&gfx->samus.rightJump1_C0,&gfx->samus.rightJump1_C1,NULL,&sprites->samus.rightJump2},
			{25,13,&gfx->samus.rightJump2_C0,&gfx->samus.rightJump2_C1,NULL,&sprites->samus.rightJump2},
			{18,12,&gfx->samus.rightKneel_C0,&gfx->samus.rightKneel_C1,NULL,&sprites->samus.rightKneel},
			{25,12,&gfx->samus.rightLand_C0,&gfx->samus.rightLand_C1,NULL,&sprites->samus.rightStand},
			{12,9,&gfx->samus.rightMorph_C0,&gfx->samus.rightMorph_C1,NULL,&sprites->samus.ball1},
			{23,10,&gfx->samus.rightRun1_C0,&gfx->samus.rightRun1_C1,NULL,&sprites->samus.rightRun2},
			{24,16,&gfx->samus.rightRun2_C0,&gfx->samus.rightRun2_C1,NULL,&sprites->samus.rightRun3},
			{23,16,&gfx->samus.rightRun3_C0,&gfx->samus.rightRun3_C1,NULL,&sprites->samus.rightRun4},
			{23,12,&gfx->samus.rightRun4_C0,&gfx->samus.rightRun4_C1,NULL,&sprites->samus.rightRun5},
			{23,12,&gfx->samus.rightRun5_C0,&gfx->samus.rightRun5_C1,NULL,&sprites->samus.rightRun1},
			{23,10,&gfx->samus.rightStand_C0,&gfx->samus.rightStand_C1,NULL,&sprites->samus.rightStand},
			{17,16,&gfx->samus.rightWallJump_C0,&gfx->samus.rightWallJump_C1,NULL,&sprites->samus.rightWallJump},
			{16,16,&gfx->samus.leftWallJump_C0,&gfx->samus.leftWallJump_C1,NULL,&sprites->samus.leftWallJump},
			{16,16,&gfx->samus.spaceJump4_C0,&gfx->samus.spaceJump4_C1,NULL,&sprites->samus.spaceJump1},
			{13,16,&gfx->samus.spaceJump3_C0,&gfx->samus.spaceJump3_C1,NULL,&sprites->samus.spaceJump2},
			{16,16,&gfx->samus.spaceJump2_C0,&gfx->samus.spaceJump2_C1,NULL,&sprites->samus.spaceJump3},
			{13,16,&gfx->samus.spaceJump1_C0,&gfx->samus.spaceJump1_C1,NULL,&sprites->samus.spaceJump4}
		},{
			{8,16,&gfx->enemies.waver_C0,&gfx->enemies.waver_C1,NULL,&sprites->enemies.waver},
			{12,16,&gfx->enemies.sideHopper_C0,&gfx->enemies.sideHopper_C1,NULL,&sprites->enemies.sideHopper},
			{8,16,&gfx->enemies.ripper_C0,&gfx->enemies.ripper_C1,NULL,&sprites->enemies.ripper},
			{8,16,&gfx->enemies.ripper2_C0,&gfx->enemies.ripper2_C1,NULL,&sprites->enemies.ripper2},
			{19,16,&gfx->enemies.SpacePirateWalk1Left_C0,&gfx->enemies.SpacePirateWalk1Left_C1,NULL,&sprites->enemies.SpacePirateWalk2Left},
			{20,16,&gfx->enemies.SpacePirateWalk2Left_C0,&gfx->enemies.SpacePirateWalk2Left_C1,NULL,&sprites->enemies.SpacePirateWalk1Left},
			{19,16,&gfx->enemies.SpacePirateStandLeft_C0,&gfx->enemies.SpacePirateStandLeft_C1,NULL,&sprites->enemies.SpacePirateStandLeft},
			{19,16,&gfx->enemies.SpacePirateLookLeft_C0,&gfx->enemies.SpacePirateLookLeft_C1,NULL,&sprites->enemies.SpacePirateStandLeft},
			{17,16,&gfx->enemies.SpacePirateShootLeft_C0,&gfx->enemies.SpacePirateShootLeft_C1,NULL,&sprites->enemies.SpacePirateStandLeft},
			{19,16,&gfx->enemies.SpacePirateWalk1Right_C0,&gfx->enemies.SpacePirateWalk1Right_C1,NULL,&sprites->enemies.SpacePirateWalk2Right},
			{20,16,&gfx->enemies.SpacePirateWalk2Right_C0,&gfx->enemies.SpacePirateWalk2Right_C1,NULL,&sprites->enemies.SpacePirateWalk1Right},
			{19,16,&gfx->enemies.SpacePirateStandRight_C0,&gfx->enemies.SpacePirateStandRight_C1,NULL,&sprites->enemies.SpacePirateStandRight},
			{19,16,&gfx->enemies.SpacePirateLookRight_C0,&gfx->enemies.SpacePirateLookRight_C1,NULL,&sprites->enemies.SpacePirateStandRight},
			{17,16,&gfx->enemies.SpacePirateShootRight_C0,&gfx->enemies.SpacePirateShootRight_C1,NULL,&sprites->enemies.SpacePirateStandRight},
			{3,16,&gfx->enemies.SpacePirateLaser_C0,&gfx->enemies.SpacePirateLaser_C1,NULL,&sprites->enemies.SpacePirateLaser},
			{11,8,&gfx->enemies.skree_C0,&gfx->enemies.skree_C1,NULL,&sprites->enemies.skree},
			{11,8,&gfx->enemies.skree2_C0,&gfx->enemies.skree2_C1,NULL,&sprites->enemies.skree3},
			{11,8,&gfx->enemies.skree3_C0,&gfx->enemies.skree3_C1,NULL,&sprites->enemies.skree4},
			{11,8,&gfx->enemies.skree4_C0,&gfx->enemies.skree4_C1,NULL,&sprites->enemies.skree2},
			{4,4,&gfx->enemies.skreeProjectile_C0,&gfx->enemies.skreeProjectile_C1,NULL,&sprites->enemies.skreeProjectile},
			{7,8,&gfx->enemies.zebboL_C0,&gfx->enemies.zebboL_C1,NULL,&sprites->enemies.zebboL},
			{7,8,&gfx->enemies.zebboR_C0,&gfx->enemies.zebboR_C1,NULL,&sprites->enemies.zebboR},
			{7,8,&gfx->tiles.pipe1[0],&gfx->tiles.pipe1[1],&gfx->tiles.pipe1[2],&sprites->enemies.zebboSpawn}
		},{
			{8,8,&gfx->weapons.missile1_C0,&gfx->weapons.missile1_C1,NULL,&sprites->weapons.missile1},
			{8,8,&gfx->weapons.missile2_C0,&gfx->weapons.missile2_C1,NULL,&sprites->weapons.missile2},
			{8,8,&gfx->weapons.missile3_C0,&gfx->weapons.missile3_C1,NULL,&sprites->weapons.missile3},
			{8,8,&gfx->weapons.missile4_C0,&gfx->weapons.missile4_C1,NULL,&sprites->weapons.missile4},
			{5,8,&gfx->weapons.missile5_C0,&gfx->weapons.missile5_C1,NULL,&sprites->weapons.missile5},
			{5,8,&gfx->weapons.missile6_C0,&gfx->weapons.missile6_C1,NULL,&sprites->weapons.missile6},
			{8,5,&gfx->weapons.missile7_C0,&gfx->weapons.missile7_C1,NULL,&sprites->weapons.missile7},
			{8,5,&gfx->weapons.missile8_C0,&gfx->weapons.missile8_C1,NULL,&sprites->weapons.missile8},
			{4,4,&gfx->weapons.powerBeam_C0,&gfx->weapons.powerBeam_C1,NULL,&sprites->weapons.powerBeam}
		},{
			{8,8,&gfx->sfx.bombExplode1_C0,&gfx->sfx.bombExplode1_C1,NULL,&sprites->sfx.bombExplode2},
			{8,8,&gfx->sfx.bombExplode2_C0,&gfx->sfx.bombExplode2_C1,NULL,&sprites->sfx.bombExplode3},
			{8,8,&gfx->sfx.bombExplode3_C0,&gfx->sfx.bombExplode3_C1,NULL,&sprites->sfx.bombExplode2},
			{8,8,&gfx->sfx.breakAway1_C0,&gfx->sfx.breakAway1_C1,NULL,&sprites->sfx.breakAway2},
			{8,8,&gfx->sfx.breakAway2_C0,&gfx->sfx.breakAway2_C1,NULL,&sprites->sfx.breakAway3},
			{8,8,&gfx->sfx.breakAway3_C0,&gfx->sfx.breakAway3_C1,NULL,&sprites->sfx.empty8x8},
			{8,8,&gfx->tiles.empty8x8[0],&gfx->tiles.empty8x8[1],&gfx->tiles.empty8x8[2],&sprites->sfx.empty8x8},
			{16,16,&gfx->sfx.enemyX1_C0,&gfx->sfx.enemyX1_C1,&gfx->sfx.enemyX1_M0,&sprites->sfx.enemyX2},
			{16,16,&gfx->sfx.enemyX2_C0,&gfx->sfx.enemyX2_C1,&gfx->sfx.enemyX2_M0,&sprites->sfx.enemyX3},
			{16,16,&gfx->sfx.enemyX3_C0,&gfx->sfx.enemyX3_C1,&gfx->sfx.enemyX3_M0,&sprites->sfx.empty8x8}
		},{
			{8,8,&gfx->pickups.energy1_C0,&gfx->pickups.energy1_C1,&gfx->pickups.energy1_M0,&sprites->pickups.energy2},
			{8,8,&gfx->pickups.energy2_C0,&gfx->pickups.energy2_C1,&gfx->pickups.energy2_M0,&sprites->pickups.energy3},
			{8,8,&gfx->pickups.energy3_C0,&gfx->pickups.energy3_C1,&gfx->pickups.energy3_M0,&sprites->pickups.energy},
			{8,8,&gfx->pickups.missile_C0,&gfx->pickups.missile_C1,&gfx->pickups.missile_M0,&sprites->pickups.missile2},
			{8,8,&gfx->pickups.missile_C1,&gfx->tiles.empty8x8[0],&gfx->pickups.missile_M0,&sprites->pickups.missile},
			{8,8,&gfx->pickups.superMissile_C0,&gfx->pickups.superMissile_C1,&gfx->pickups.superMissile_M0,&sprites->pickups.superMissile2},
			{8,8,&gfx->pickups.superMissile_C1,&gfx->tiles.empty8x8[0],&gfx->pickups.superMissile_M0,&sprites->pickups.superMissile},
			{8,8,&gfx->pickups.powerBomb_C0,&gfx->pickups.powerBomb_C1,&gfx->pickups.powerBomb_M0,&sprites->pickups.powerBomb2},
			{8,8,&gfx->pickups.powerBomb_C1,&gfx->tiles.empty8x8[0],&gfx->pickups.powerBomb_M0,&sprites->pickups.powerBomb}
		},{
			{8,8,&gfx->tiles.missile[0],&gfx->tiles.missile[1],&gfx->tiles.missile[2],&sprites->items.missile},
			{8,8,&gfx->tiles.superMissile[0],&gfx->tiles.superMissile[1],&gfx->tiles.superMissile[2],&sprites->items.superMissile},
			{8,8,&gfx->tiles.powerBomb[0],&gfx->tiles.powerBomb[1],&gfx->tiles.powerBomb[2],&sprites->items.powerBomb},
			{8,8,&gfx->tiles.energyTank[0],&gfx->tiles.energyTank[1],&gfx->tiles.energyTank[2],&sprites->items.energy}
		}
	};

	memcpy(sprites, &s, sizeof(s));
}

void makeGfx(void) {
	if((gfx = malloc(sizeof(struct graphics))) == NULL) {
		printf("Not Enough free RAM!");
		while(!_keytest(RR_ENTER));
		exit(0);
	}
	FILE* f = fopen("metgfx","rb");
	if(f==NULL) {
		printf("metgfx.MET: Failed to open file\n");
		while(!_keytest(RR_ENTER));
		exit(0);
	}
	if(fread(gfx, sizeof(struct graphics), 1, f)<1) {
		printf("metgfx.MET: Failed to read file\n");
		fclose(f);
		while(!_keytest(RR_ENTER));
		exit(0);
	}
	if((v0 = malloc(LCD_SIZE)) == NULL)
		exit(0);
	if((v1 = malloc(LCD_SIZE)) == NULL)
		exit(0);
}
