// C Source File
// Created 05/Dec/03; 22:40:38
/*============= My New _Main Function ==============*/

void _main(void) {
	INT_HANDLER interrupt1 = GetIntVec(AUTO_INT_1);
	GraySpriteInit(3840);
	SetIntVec(AUTO_INT_1,DUMMY_HANDLER);
	//initializations
	int i;
	int dir;
	int hit_bottom=0;
	
	
	
	//menu goes here
	
	
	//this starts the overall loop
	//while(something){
		//area switching stuff goes one here
		//then the real action begins!
		while (!_rowread(0x40)&0x01/*esc*/) {
			//draw the screen:
			putScreen();
			// collision detections
			//run through each tile in the tile stack
			for (i=0;i<=100;i++){
				if(tile_stack[i].type==EMPTY)break;
				dir=samusHit(tile_stack[i]);
				if(dir==RIGHT){
					samus.x_change=0;
					samus.x_pos=tile_stack[i].x_pos-samus.width;
				}
				if(dir==LEFT){
					samus.x_change=0;
					samus.x_pos=tile_stack[i].x_pos+tile_stack[i].width;
				}
				if(dir==DOWN){
					samus.y_change=0;
					samus.y_pos=tile_stack[i].y_pos-samus.height;
					hit_bottom=1;
				}
				if(dir==UP){
					samus.y_change=0;
					samus.y_pos=tile_stack[i].y_pos+tile_stack[i].height;
				}
			}
			if(samus.y_change>0&&!samus.jumping)samus.y_change=GRAVITY;
			if(samus.y_change<0||!hit_bottom){
				samus.y_change=GRAVITY;
				if(!samus.morph){
					if(samus.face==LEFT)samus.state=LEFT_FALL;
					if(samus.face==RIGHT)samus.state=RIGHT_FALL;
				}
			}
			if(hit_bottom){
				if(samus.face==LEFT){samus.state=LEFT_REG;samus.flip=0;}
				if(samus.face==RIGHT)samus.state=RIGHT_REG;samus.flip=0;}
			}
			samus.x_pos+=samus.x_change;
			samus.y_pos+=samus.y_change;
			//the movement/key reading routines
			if(!samus.y_change&&samus.x_change>0)samus.x_change-=1;//this is so that if you are moving horizontally, you decelerate.  this will be undone later if you press the direction
			if(!samus.y_change&&samus.x_change<0)samus.x_change+=1;
			if(_rowread(0x01)&0x02/*left*/&&samus.x_change>-5){samus.x_change-=1;samus.face=LEFT;}
			if(_rowread(0x01)&0x08/*right*/&&samus.x_change<5){samus.x_change+=1;samus.face=RIGHT;}
			if(_rowread(0x01)&0x10/*2nd*/)pushShot();//shooting or bombing
			if(_rowread(0x01)&0x40/*diamond*/&&samus.y_change==0){//jumping
				if(samus.jumping<5)samus.jumping+=1;
				if(samus.face==LEFT&&!samus.x_change)samus.state=LEFT_JUMP;
				if(samus.face==RIGHT&&!samus.x_change)samus.state=RIGHT_JUMP;
				if(samus.x_change){				
					if(!samus.flip){
						if(samus.face==LEFT)samus.state=LEFT_FLIP_1;
						if(samus.face==RIGHT)samus.state=RIGHT_FLIP_1;
					}
				}
				samus.y_change=JUMP_POWER;
			}
			//the morphing stuff
		}		
	//this ends the overall}
	GraySpriteDeInit();
	SetIntVec(AUTO_INT_1,interrupt1);
}