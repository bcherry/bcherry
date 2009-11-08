// C Header File
// Created 8/10/2003; 7:03:46 PM

//custom gray sprite functions

void *light_buff=NULL, *dark_buff=NULL;
void *light_vscreen=NULL, *dark_vscreen=NULL;

typedef struct {
	int upleft_x, upleft_y, downright_x, downright_y;
} SCREEN_RECT;

SCREEN_RECT sprt_bound;

inline void m_Sprite8(short x1, short y, short h, unsigned char *sprite, void *light_buff, void *dark_buff, short mode)
{
  int x=x1;
  if(x1<sprt_bound.upleft_x) x=sprt_bound.upleft_x;
  long addr_light=(long)light_buff+30*y+((x>>3)&0xfffe), addr_dark=(long)dark_buff+30*y+((x>>3)&(0xfffe)), d1_light, d1_dark;
  unsigned short cnt=24-(x&15),data_and,light_data,dark_data;
  short tempy=y,temp=h;
	for(;h--;)
	  {
	    dark_data=*(sprite+temp);
	    light_data=*sprite;
	    if(x1<sprt_bound.upleft_x) {dark_data<<=abs(sprt_bound.upleft_x-x1); light_data<<=abs(sprt_bound.upleft_x-x1);}
	    if(x1>sprt_bound.downright_x-8) {light_data>>=x-(sprt_bound.downright_x-8); dark_data>>=x-(sprt_bound.downright_x-8);}
	    if(tempy<sprt_bound.upleft_y||tempy>sprt_bound.downright_y) {light_data=0; dark_data=0;}
			data_and=light_data | dark_data;
			*(long*)addr_light&=~((long)(data_and&0xff)<<cnt);
			*(long*)addr_dark&=~((long)(data_and&0xff)<<cnt);
	    d1_light=(long)light_data<<cnt; d1_dark=(long)dark_data<<cnt;
	    if(mode==SPRT_XOR) {*(long*)addr_light^=d1_light, *(long*)addr_dark^=d1_dark;}
	    else {*(long*)addr_light|=d1_light, *(long*)addr_dark|=d1_dark;}
	    addr_light+=30,addr_dark+=30,*sprite++; tempy++;
	  }
}

inline void m_Sprite16(short x1, short y, short h, unsigned short *sprite, void *light_buff, void *dark_buff, short mode)
{
  int x=x1;
  if(x1<sprt_bound.upleft_x) x=sprt_bound.upleft_x;
  long addr_light=(long)light_buff+30*y+((x>>3)&0xfffe), addr_dark=(long)dark_buff+30*y+((x>>3)&(0xfffe)), d1_light, d1_dark;
  unsigned short cnt=16-(x&15),data_and,light_data,dark_data;
  short tempy=y,temp=h;
	for(;h--;)
	  {
	    dark_data=*(sprite+temp);
	    light_data=*sprite;
	    if(x1<sprt_bound.upleft_x) {dark_data<<=abs(sprt_bound.upleft_x-x1); light_data<<=abs(sprt_bound.upleft_x-x1);} //clip the left side of the sprite because it is overlapping the left side of the screen
	    if(x1>sprt_bound.downright_x-16) {light_data>>=x-(sprt_bound.downright_x-16); dark_data>>=x-(sprt_bound.downright_x-16);} //clip the right side of the sprite because it is overlapping the right side of the screen
	    if(tempy<sprt_bound.upleft_y||tempy>sprt_bound.downright_y) {light_data=0; dark_data=0;}
	    data_and = light_data | dark_data;
	    *(long*)addr_light&=~((long)data_and<<cnt);
			*(long*)addr_dark&=~((long)data_and<<cnt);
	    d1_light=(long)light_data<<cnt; d1_dark=(long)dark_data<<cnt;
	    if(mode==SPRT_XOR) {*(long*)addr_light^=d1_light, *(long*)addr_dark^=d1_dark;}
	    else {*(long*)addr_light|=d1_light, *(long*)addr_dark|=d1_dark;}
	  	addr_light+=30,addr_dark+=30,*sprite++; tempy++;
	  }
}

inline void m_Sprite32(short x1, short y, short h, unsigned long *sprite, void *this_light_buff, void *this_dark_buff, short mode)
{
  /*int x = x1;
  long addr_light=(long)light_buff+30*y+((x>>3)&0xfffe), addr_dark=(long)dark_buff+30*y+((x>>3)&(0xfffe));
  unsigned long light_data,dark_data,d1_light, d1_dark, d2_light, d2_dark,data_and;
  short cnt=(x&15),ccnt=32-cnt;
  short tempy=y,temp=h;
  for(;h--;)
    {
      dark_data=*(sprite+temp);
      light_data=*sprite;
      if(x1<sprt_bound.upleft_x) {dark_data<<=abs(sprt_bound.upleft_x-x1); light_data<<=abs(sprt_bound.upleft_x-x1);} //clip the left side of the sprite because it is overlapping the left side of the screen
	    if(x1>sprt_bound.downright_x-32) {light_data>>=x-(sprt_bound.downright_x-32); dark_data>>=x-(sprt_bound.downright_x-32);} //clip the right side of the sprite because it is overlapping the right side of the screen
      if(tempy<sprt_bound.upleft_y||tempy>sprt_bound.downright_y) {light_data=0; dark_data=0;}
      data_and=light_data|dark_data;
      *(long*)addr_light&=~(data_and>>cnt); *(long*)(addr_light+4)&=~(data_and<<ccnt);
      *(long*)addr_dark&=~(data_and>>cnt); *(long*)(addr_dark+4)&=~(data_and<<ccnt);
      d1_light=light_data>>cnt; d2_light=light_data<<ccnt;
      d1_dark=dark_data>>cnt; d2_dark=dark_data<<ccnt;
      if(mode==SPRT_XOR) {*(long*)addr_light^=d1_light; *(long*)(addr_light+4)^=d2_light; *(long*)addr_dark^=d1_dark; *(long*)(addr_dark+4)^=d2_dark;}
      else {*(long*)addr_light|=d1_light; *(long*)(addr_light+4)|=d2_light; *(long*)addr_dark|=d1_dark; *(long*)(addr_dark+4)|=d2_dark;}
      addr_light+=30; addr_dark+=30; *sprite++; tempy++;
    }*/
    unsigned short dark_data_1, dark_data_2, light_data_1, light_data_2, temph = h;
    unsigned short left_side[2], right_side[2];
    for(;h--;) {
			dark_data_1 = (unsigned short)(*(sprite+temph)>>16);
			dark_data_2 = (unsigned short)(*(sprite+temph));
			light_data_1 = (unsigned short)(*(sprite)>>16);
			light_data_2 = (unsigned short)(*sprite);
			left_side[0] = light_data_1;
			left_side[1] = dark_data_1;
			right_side[0] = light_data_2;
			right_side[1] = dark_data_2;
    	m_Sprite16(x1, y, 1, left_side, this_light_buff, this_dark_buff, mode);
    	m_Sprite16(x1+16, y, 1, right_side, this_light_buff, this_dark_buff, mode);
    	*sprite++; y++;
    }
}

//does some initilization to create buffers and return FALSE in case of error
int GraySpriteInit(unsigned long size_buff) {
	light_vscreen = malloc(size_buff);
	if(!light_vscreen) return -1;
	dark_vscreen = malloc(size_buff);
	if(!dark_vscreen) {free(light_vscreen); return -1;}
	light_buff = malloc(size_buff);
	if(!light_buff) {free(light_vscreen); free(dark_vscreen); return -1;}
	dark_buff = malloc(size_buff);
	if(!dark_buff) {free(light_vscreen); free(dark_vscreen); free(light_buff); return -1;}
	memset(light_vscreen, 0x00, size_buff);
	memset(dark_vscreen, 0x00, size_buff);
	memset(light_buff, 0x00, size_buff);
	memset(dark_buff, 0x00, size_buff);
	GrayOn();
	if(!IsGrayMode()) {free(light_vscreen); free(dark_vscreen); free(light_buff); free(dark_buff); GrayOff(); return -1;}
	sprt_bound.upleft_x = 0; sprt_bound.upleft_y = 0;
	sprt_bound.downright_x = LCD_WIDTH; sprt_bound.downright_y = LCD_HEIGHT;
	return 1;
}

//cleans up/deinitilizes gray sprites
int GraySpriteDeInit(void) {
	free(light_vscreen);
	free(dark_vscreen);
	free(light_buff);
	free(dark_buff);
	GrayOff();
	return 1;
}

//draws a clipped line-clipped to screen boundries.  Faster than DrawClipLine()
void CDrawLine(short x1, short y1, short x2, short y2, void *buff, short mode)
{
  short x = x1, y = y1;
  short dx = abs (x2 - x1), dy = abs (y2 - y1);
  short ystep = (y1 < y2) ? 1 : -1, pystep = 30 * ystep;
  short mov = dx ? 0 : -1;
  unsigned char *ptr = (char*)buff + 30 * y + (x >> 3);
  short mask = 1 << (~x & 7);
  if (x1 < x2)
    while (x != x2 || y != y2)
      {
        if(mode==A_OR&&x>=10&&y>=10&&x<=LCD_WIDTH-10&&y<=LCD_HEIGHT-10) *ptr |= mask;
        if(mode==A_XOR&&x>=10&&y>=10&&x<=LCD_WIDTH-10&&y<=LCD_HEIGHT-10) *ptr ^= mask;
        if(mode==A_AND&&x>=10&&y>=10&&x<=LCD_WIDTH-10&&y<=LCD_HEIGHT-10) *ptr &= mask;
        if (mov < 0) y += ystep, ptr += pystep, mov += dx;
        else
          {
            mov -= dy;
            if (++x & 7) mask >>= 1;
            else ptr++, mask = 0x80;
          }
      }
  else
    while (x != x2 || y != y2)
      {
        if(mode==A_OR&&x>=10&&y>=10&&x<=LCD_WIDTH-10&&y<=LCD_HEIGHT-10) *ptr |= mask;
        if(mode==A_XOR&&x>=10&&y>=10&&x<=LCD_WIDTH-10&&y<=LCD_HEIGHT-10) *ptr ^= mask;
        if(mode==A_AND&&x>=10&&y>=10&&x<=LCD_WIDTH-10&&y<=LCD_HEIGHT-10) *ptr &= mask;
        if (mov < 0) y += ystep, ptr += pystep, mov += dx;
        else
          {
            mov -= dy;
            if (x-- & 7) mask <<= 1;
            else ptr--, mask = 1;
          }
      }
}

