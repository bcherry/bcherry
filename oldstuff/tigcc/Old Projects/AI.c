#define OPTIMIZE_ROM_CALLS
#define SAVE_SCREEN
#include <tigcclib.h>

#define lcd_width 160
#define lcd_height 100

short _ti89;
short _ti92plus;

//================================GAME GLOBALS============================
int player_x;
int player_bullet_x[] = {0,0,0,0,0};
int player_bullet_y[] = {0,0,0,0,0};
int player_particles_x[] = {-1,-1,-1,-1,-1,-1};
int player_particles_y[] = {-1,-1,-1,-1,-1,-1};
int player_explosion = 0;
int player_invincible = 0;
int player_invincible_time = 150;
int player_shield_on = 0;
int player_shield = 200;
int laser_x = 0;
int laser_y = -1;
int laser_length = -2;
int laser_press = 0;
int laser_level = 15;
int cluster_x = -1;
int cluster_y = -1;
int cluster_at_y = -1;
int cluster_radius = 0;
int cluster_press = 0;
int cluster_level = 20;
int heat_x = -1;
int heat_y = -1;
int heat_target = 0;
int heat_level = 3;
int heat_accuracy = 1;
int disruption_press = 0;
int item_disruption = 5;
int item_heat = 5;
int item_cluster = 5;
int item_laser = 5;
int item_show = 1;
int enemy1_particles_x[30];
int enemy1_particles_y[30];
const int enemy_particles_Vx[] = {-3, -2, 0, 3, 2, 3};
const int enemy_particles_Vy[] = {-2, 2, -3, 2, 3, -2};
int enemy_x[] = {20,40,60,80,100};
int enemy_y[] = {3,3,3,3,3};
int enemy_Vx[] = {-2,1,-1,2,1};
int enemy_Vy[] = {-1,1,-1,1,1};
int enemy_bullet_x[] = {0,0,0,0,0};
int enemy_bullet_y[] = {111,111,111,111,111};
int enemy_explosion[] = {0,0,0,0,0};
int enemy_alive[] = {0,0,0,0,0};
int enemy_frame;
int enemy_ship = 1;
int enemy_explosion_show = 1;
int enemy_low = 1; //Variable to see how low the enemies can go
int enemy_bullet_accuracy = 0;
int enemy_bullet_speed = 1;
int upgrade_x = -1;
int upgrade_y = 111;
int game_speed = 6800;
int game_fire;
int game_level = 1;
int game_lives = 3;
int game_quick_exit = 0;
int name1[12];
int high_name[] = {74, 97, 115, 111, 110, 32, 72, 111, 32, 32, 32, 32,
					    74, 97, 115, 111, 110, 32, 72, 111, 32, 32, 32, 32,
						 74, 97, 115, 111, 110, 32, 72, 111, 32, 32, 32, 32,
						 74, 97, 115, 111, 110, 32, 72, 111, 32, 32, 32, 32,
						 74, 97, 115, 111, 110, 32, 72, 111, 32, 32, 32, 32};
int high_score[] = {0, 0, 0, 0, 0};
int repeat1 = 0;
int repeat2 = 0;
char *virtual = 0;

//================================PICTURES================================
static unsigned short player_ship[] = 
{0x40, 0xE0, 0xE0, 0x130, 0x110, 0x318, 0x144D, 0x1EAF, 0x16AD, 0x16ED, 0x171D, 0x1F1F, 0xDF6};
static unsigned char player_bullet[] =
{0x2, 0x7, 0x7, 0x2};
static unsigned char enemy_bullet[] =
{0x2, 0x7, 0x2};
static unsigned short heart[] =
{0xEE, 0x1FD, 0x1FD, 0x1FB, 0xF6, 0x7C, 0x38, 0x10};
static unsigned long shield[] =
{0x1100000, 0x1F00000, 0x1D1FFFF, 0x1D10001, 0x1F1FFFF, 0xE00000};
static unsigned short upgrade[] = 
{0x38, 0x74, 0xF2, 0x1FF, 0xFE, 0x7C, 0x38};
static unsigned char particle[] =
{0xE, 0x1F, 0x1B, 0x1F, 0xE};
static unsigned char enemy_particle[] = 
{0x7};
static unsigned char cluster[] =
{0x1C, 0x3A, 0x79, 0x7F, 0x7F, 0x3E, 0x1C};
static unsigned char heat[] =
{0x2, 0x5, 0x5, 0x5, 0x7};
static unsigned char disruption[] =
{0x19, 0x3F, 0x66};
static unsigned long enemy1[4][17] = 
{{0x0, 0x0, 0x0, 0x0, 0x0, 0x701C, 0xE83A, 0x1E7F9, 0x1FFFF, 0x1FFFF, 0xF83E, 0x701C, 0x0, 0x0, 0x0, 0x0, 0x0},
 {0x0, 0x3800, 0x7400, 0xF200, 0xFE00, 0xFE00, 0x7E00, 0x3F00, 0x380, 0x1F8, 0xF4, 0xF2, 0xFE, 0xFE, 0x7C, 0x38, 0x0},
 {0x380, 0x740, 0xF20, 0xFE0, 0xFE0, 0x7C0, 0x380, 0x380, 0x380, 0x380, 0x380, 0x740, 0xF20, 0xFE0, 0xFE0, 0x7C0, 0x380},
 {0x0, 0x38, 0x74, 0xF2, 0xFE, 0xFE, 0xFC, 0x1F8, 0x380, 0x3F00, 0x7600, 0xF200, 0xFE00, 0xFE00, 0x7C00, 0x3800, 0x0}};
static unsigned long enemy2[2][17] = 
{{0x0, 0x3838, 0x7474, 0xF2F2, 0xFEFE, 0xFEFE, 0x7EFC, 0x3FF8, 0x380, 0x3FF8, 0x76F4, 0xF2F2, 0xFEFE, 0xFEFE, 0x7C7C, 0x3838, 0x0},
 {0x380, 0x740, 0xF20, 0xFE0, 0xFE0, 0x77DC, 0xEBBA, 0x1E7F9, 0x1FFFF, 0x1FFFF, 0xFBBE, 0x775C, 0xF20, 0xFE0, 0xFE0, 0x7C0, 0x380}};
static unsigned long enemy3[] = 
{0x7C0, 0x1FF0, 0x3FB8, 0x7F4C, 0xFF26, 0xFF96, 0x1FF8B, 0x1FFFF, 0x1FFFF, 0x1FFFF, 0x1DFFF, 0xDFFE, 0xEFFE, 0x77FC, 0x3FF8, 0x1FF0, 0x7C0};
static unsigned long enemy4[4][17] =
{{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xE38E, 0x1D75D, 0x1FFFF, 0x1F7DF, 0xE38E, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
 {0x0, 0x0, 0x38, 0x74, 0x7C, 0x7C, 0x3F8, 0x740, 0x7C0, 0x7C0, 0x3F80, 0x7400, 0x7C00, 0x7C00, 0x3800, 0x0, 0x0},
 {0x380, 0x740, 0x7C0, 0x7C0, 0x380, 0x100, 0x380, 0x740, 0x7C0, 0x7C0, 0x380, 0x100, 0x380, 0x740, 0x7C0, 0x7C0, 0x380},
 {0x0, 0x0, 0x3800, 0x7400, 0x7C00, 0x7C00, 0x3F80, 0x740, 0x7C0, 0x7C0, 0x3F8, 0x74, 0x7C, 0x7C, 0x38, 0x0, 0x0}};
static unsigned long enemy5[3][17] =
{{0x380, 0x740, 0xF20, 0xFE0, 0xFE0, 0x7C0, 0x380, 0x0, 0x0, 0x701C, 0xE83A, 0x1E479, 0x1FC7F, 0x1FC7F, 0xF83E, 0x701C, 0x0},
 {0x380, 0x740, 0xF20, 0xFE0, 0xFE0, 0x7C0, 0x380, 0x0, 0x0, 0x1C70, 0x3AE8, 0x79E4, 0x7FFC, 0x7FFC, 0x3EF8, 0x1C70, 0x0},
 {0x380, 0x740, 0xF20, 0xFE0, 0xFE0, 0x7C0, 0x380, 0x0, 0x0, 0x380, 0x740, 0xF20, 0xFE0, 0xFE0, 0x7C0, 0x380, 0x0}};
static unsigned long enemy6[2][14] = 
{{0x40008, 0x60018, 0x50028, 0x28C50, 0x15EA0, 0xDEC0, 0x7380, 0x1F3E0, 0x3F3F0, 0x77FB8, 0x67F98, 0x2EDD0, 0xC0C0, 0x4080},
 {0x0, 0x0, 0x0, 0xC00, 0x5E80, 0x1BEE0, 0x6F3F8, 0x9F3F4, 0xBF3FC, 0x77FB8, 0x67F98, 0x2EDD0, 0xC0C0, 0x4080}};
static unsigned long enemy7[2][17] =
{{0x0, 0x0, 0x380, 0x740, 0x1CF27, 0x1AFEB, 0x11FF1, 0x87C2, 0x5394, 0x3F78, 0x7C0, 0x540, 0x380, 0x280, 0x280, 0x0, 0x0},
 {0x0, 0x0, 0x380, 0x1750, 0x3F38, 0x2FE8, 0x2FE8, 0x17D0, 0xBA0, 0x740, 0x7C0, 0x540, 0x380, 0x440, 0x820, 0x0, 0x0}};

//==========================PROTOTYPES (in order of appearance)=====================
int pause(void);
void init(void);
void stop(void);
void menu(int , int , int , int , int);
int choose(int , int , int , int , int , int, int);
void InputStr(int , int , int);
void high(void);
void quit(void);
int main_menu(void);
void intro(void);
void changeship(void);
void reset(void);
void start(void);
void drawship(int , int , int);
void enemyshoot(void);
void upgrade_test(int);
void upgrade_get(int);
void paint(void);
void end(void);

//===================================MAIN LOOP======================================
void _main(void)
{
	//==============================DECLARE LOCALS/INITIALIZE=========================
	int x;
	int y;
	int z;
	//repeats control how fast an operation repeats
	int repeat1 = 0;
	int repeat2 = 0;
	int repeat3 = 0;
	int repeat4 = 0;
	//Cant shoot controls if you can shoot again (0 for can, 1 for cant)
	int cant_shoot = 0;
	virtual = malloc(3840);
	if(game_quick_exit != 1)
		intro();
	init();
	if(game_quick_exit != 1)
		start();
	game_quick_exit = 0;
	for(;;)
	{
		//==============================MOVE ENEMY SHIPS============================
		for(x = 0; x < 5; x++)
		{
			repeat3++;
			if(repeat3 == 2000) changeship();
			if(repeat3 == 500 || repeat3 == 1000 || repeat3 == 1500 || repeat3 == 0) enemyshoot();
			if(repeat3 > 2000) repeat3 = 0;
			enemy_x[x] += enemy_Vx[x];
			enemy_y[x] += enemy_Vy[x];
			if(enemy_x[x] >= lcd_width - 32) {enemy_Vx[x] *= -1; enemy_x[x] = lcd_width - 32;}
			if(enemy_x[x] <= 0) {enemy_Vx[x] *= -1; enemy_x[x] = 0;}
			enemy_low = game_level;
			if(game_level >= 50) enemy_low = game_level - 25;
			if(game_level >= 80) enemy_low = game_level - 55;
			if(enemy_low > 60) enemy_low = 60;
			if(enemy_y[x] >= lcd_height-77+enemy_low && enemy_Vy[x] > 0) enemy_Vy[x] *= -1;
			if(enemy_y[x] <= 0) {enemy_Vy[x] *= -1; enemy_y[x] = 0;}
		}
		//==============================MOVE BULLETS================================
		
		//Enemy bullets
		if(game_level == 10) enemy_bullet_accuracy = 1;
		if(game_level == 50) enemy_bullet_speed = 2;
		if(game_level == 80) enemy_bullet_accuracy = 2;
		for(x = 0; x < 5; x++)
		{	
			if(enemy_bullet_y[x] < lcd_height)
			{
				enemy_bullet_y[x] += enemy_bullet_speed;
				if(enemy_bullet_x[x] < player_x) enemy_bullet_x[x] += enemy_bullet_accuracy;
				if(enemy_bullet_x[x] > player_x) enemy_bullet_x[x] -= enemy_bullet_accuracy;
			}
		}
		//Move single fire bullets
		if(game_fire == 1) player_bullet_y[0] -= 2;
		//Move triple fire bullets
		if(game_fire == 3)
		{
			for(x = 0; x < 3; x++) player_bullet_y[x] -= 2;
			repeat2++;
			if(repeat2 == 5) player_bullet_x[0] -= 1;
			if(repeat2 == 5) player_bullet_x[2] += 1;
			if(repeat2 > 5) repeat2 = 0;
		}
		//Move spreader fire bullets
		if(game_fire == 5)
		{
			for(x = 0; x < 5; x++) player_bullet_y[x] -= 2;
			repeat1++;
			repeat2++;
			if(repeat1 == 1) player_bullet_x[0] -= 1;
			if(repeat2 == 4) player_bullet_x[1] -= 1;
			if(repeat2 == 4) player_bullet_x[3] += 1;
			if(repeat1 == 1) player_bullet_x[4] += 1;
			if(repeat1 > 1) repeat1 = 0;
			if(repeat2 > 4) repeat2 = 0;
		}
		//If bullets go out of boundary, delete them.
		for(x = 0; x < 5; x++)
		{
			if(player_bullet_x[x] < 0 || player_bullet_x[x] > lcd_width) player_bullet_x[x] = 0;
			if(player_bullet_y[x] < 0)
			{
				player_bullet_y[x] = 0;
				if(x == 0 && game_fire == 1) cant_shoot = 0;
				if(x == 2 && game_fire == 3) cant_shoot = 0;
				if(x == 4 && game_fire == 5) cant_shoot = 0;
			}
		}
		
		//=========================IF ENEMY BULLET HIT YOU========================
		for(x = 0; x < 5; x++)
		{
			if(player_shield_on == 0 && player_invincible == 0 && player_explosion == 0 && enemy_bullet_x[x] >= player_x - 3 && enemy_bullet_x[x] <= player_x + 11 && enemy_bullet_y[x] >= lcd_height-13 && enemy_bullet_y[x] < lcd_height)		
			{	
				player_explosion = 1;
				player_invincible_time = 150;
				for(z = 0; z < 6; z++)
				{
					player_particles_x[z] = player_x + 13;
					player_particles_y[z] = lcd_height - 13 + 17/2;
				}
			}
		}
		
		//==========================IF YOU GOT UPGRADE===========================
		if(upgrade_x + 11 >= player_x && upgrade_x < player_x + 9 && upgrade_y + 7 > lcd_height - 13 && upgrade_y <= lcd_height)
		{
			upgrade_y = lcd_height + 1;
			upgrade_get(random(9)+1);
		}
		//=========================Test if Cluster bomb hit enemy===============
		for(x = 0; x < 5; x++)
		{
			if(enemy_alive[x] == 1 && cluster_radius > 0 && cluster_x - cluster_radius <= enemy_x[x]+17 && cluster_x + cluster_radius >= enemy_x[x] && cluster_at_y - cluster_radius <= enemy_y[x]+17 && cluster_at_y + cluster_radius >= enemy_y[x])
			{
				enemy_explosion[x] = 1;
				enemy_Vx[x] = 0;
				enemy_Vy[x] = 0;
				enemy_alive[x] = 0;
				upgrade_test(x);
			}
		}

		//=========================TEST IF HEAT SEEKER HIT ENEMY=================
		for(x = 0; x < 5; x++)
		{
			if(enemy_alive[x] && heat_y >= 0 && heat_x >= enemy_x[x] && heat_x <= enemy_x[x] + 24 && heat_y <= enemy_y[x] + 17 && heat_y >= enemy_y[x])
			{
				enemy_explosion[x] = 1;
				enemy_Vx[x] = 0;
				enemy_Vy[x] = 0;
				enemy_alive[x] = 0;
				heat_y = -1;
				upgrade_test(x);
			}
		}
		
		//=========================TEST IF LASER HIT ENEMY=======================
		for(x = 0; x < 5; x++)
		{
			if(enemy_alive[x] && laser_x-13 >= enemy_x[x] && laser_x <= enemy_x[x] + 32 && ((enemy_y[x] >= laser_y && enemy_y[x] <= laser_y + laser_length) || (enemy_y[x]+17 >= laser_y && enemy_y[x]+17 <= laser_y + laser_length)))
			{
				enemy_explosion[x] = 1;
				enemy_Vx[x] = 0;
				enemy_Vy[x] = 0;
				enemy_alive[x] = 0;
				upgrade_test(x);
			}
		}
		
		//=========================TEST IF BULLET HIT ENEMY======================
		for(x = 0; x < game_fire; x++)
		{
			for(y = 0; y < 5; y++)
			{
				if((player_bullet_x[x] != 0 && player_bullet_y[x] != 0 && enemy_alive[y] == 1 && player_bullet_x[x] >= enemy_x[y] && player_bullet_x[x] <= enemy_x[y] + 24 && player_bullet_y[x] <= enemy_y[y] + 17 && player_bullet_y[x] >= enemy_y[y]))
				{
					enemy_explosion[y] = 1;
					enemy_Vx[y] = 0;
					enemy_Vy[y] = 0;
					enemy_alive[y] = 0;
					player_bullet_x[x] = 0;
					player_bullet_y[x] = 0;
					upgrade_test(y);
				}	
			}
		}
		
		//========================If all enemies die, get new ones================
		y = 0;
		for(x = 0; x < 5; x++)
		{
			if(enemy_alive[x] != 0) y = 1;
		}
		if(y == 0)
		{
			repeat4++;
			if(repeat4 == 100)
			{
				for(x = 0; x < 5; x++) enemy_alive[x] = 1;
				enemy_ship = random(7)+1;
				game_level++;
				changeship();
			}
			if(repeat4 > 100) repeat4 = 0;
		}
		
		//==============================GET INPUT================================
			
		if(_rowread(0xFE)&0x10 && player_shield_on == 0 && cant_shoot == 0 && player_explosion == 0) //2nd - Shoot regular bullets
		{
			cant_shoot = 1;
			//Shoot if you have single fire
			if(game_fire == 1)
			{
				player_bullet_x[0] = player_x + 3;
				player_bullet_y[0] = lcd_height - 13;
			}
			//Shoot if you have triple fire
			if(game_fire == 3)
			{
				player_bullet_x[0] = player_x;
				player_bullet_x[1] = player_x + 3;
				player_bullet_x[2] = player_x + 6;
				player_bullet_y[0] = lcd_height - 13;
				player_bullet_y[1] = lcd_height - 17;
				player_bullet_y[2] = lcd_height - 13;
			}
			//Shoot if you have spreader fire
			if(game_fire == 5)
			{
				player_bullet_x[0] = player_x - 3;
				player_bullet_x[1] = player_x;
				player_bullet_x[2] = player_x + 3;
				player_bullet_x[3] = player_x + 6;
				player_bullet_x[4] = player_x + 9;
				player_bullet_y[0] = lcd_height - 13;
				player_bullet_y[1] = lcd_height - 16;
				player_bullet_y[2] = lcd_height - 19;
				player_bullet_y[3] = lcd_height - 16;
				player_bullet_y[4] = lcd_height - 13;
			}
		}
		if(_rowread(0xFE)&0x2 && player_explosion == 0) //Left - move left
		{
			player_x -= 2;
			if(player_x < 0) player_x = 0;
		}
		if(_rowread(0xFE)&0x8 && player_explosion == 0) //Right - move right
		{
			player_x += 2;
			if(player_x > lcd_width - 16) player_x = lcd_width - 16;
		}
		player_shield_on = 0;
		if(_rowread(0xFE)&0x1 && player_explosion == 0 && player_shield > 0) //Up - activate shields
		{
			player_shield_on = 1;
			player_shield--;
			if(player_shield < 0) player_shield = 0;
		}
		if(_rowread(0xFE)&0x4 && player_explosion == 0) //Down - teleport
		{
			if(player_invincible_time == 150)
			{
				player_invincible = 1;
				player_invincible_time = 15;
				if(player_x <= lcd_width/2 - 13/2)
					player_x = lcd_width - 16;
				else
					player_x = 0;
			}
		}
		if(_rowread(0xDF)&0x80 && player_explosion == 0) //F1 - Cluster Bomb
		{
			if(cluster_y < 0 && cluster_press == 0 && item_cluster > 0 && cluster_radius == 0 && player_shield_on == 0)
			{
				cluster_press = 1;
				cluster_x = player_x + 5;
				cluster_y = lcd_height - 20;
				item_cluster--;
			}
			else
				if(cluster_press == 2)
				{
					cluster_press = 3;
					cluster_radius = 1;
				}
		}
		if(!(_rowread(0xDF)&0x80))
		{
			if(cluster_press == 1) cluster_press = 2;
			if(cluster_press == 3) cluster_press = 0;
		}
		if(_rowread(0xEF)&0x80 && heat_y < 0 && item_heat > 0 && player_explosion == 0 && player_shield_on == 0) //F2 - Heat Seeker
		{
			heat_x = player_x + 5;
			heat_y = lcd_height - 20;
			for(x = 0; x < 5; x++)
			{
				if(enemy_alive[x] == 1) heat_target = x;
			}
			item_heat--;
		}
		if(_rowread(0xFB)&0x80 && player_explosion == 0 && item_disruption > 0 && disruption_press == 0) //F4 - Disruption field
		{
			disruption_press = 1;
			z = 0;
			y = 0;
			for(x = 0; x < 5; x++)
			{
				if(enemy_alive[x] == 1 && enemy_Vx[x] == 0 && enemy_Vy[x] == 0) z = 1;
				if(enemy_alive[x] == 1) y = 1;
			}
			if(z == 0 && y == 1)
			{
				for(x = 0; x < 4; x++)
				{
					for(z = 0; z < 100; z++)
						DrawLine(0, z, 159, z, A_XOR);
					memcpy (LCD_MEM, virtual, LCD_SIZE);
				}
				for(x = 0; x < 5; x++)
				{
					if(enemy_ship < 6)
					{
						enemy_Vx[x] = 0;
						enemy_Vy[x] = 0;
					}
				}
				item_disruption--;
			}
		}
		if(!(_rowread(0xFB)&0x80))
			disruption_press = 0;
		if(_rowread(0xFD)&0x80)	//F5 - Stats menu
		{
			menu(52,20,56,58,0);
			//Draw Items to screen
			Sprite8(59, 28, 7, cluster, virtual, A_NORMAL);
			Sprite8(57, 41, 5, heat, virtual, A_NORMAL);
			DrawLine(63, 53, 63, 57, A_NORMAL);
			Sprite32(54, 64, 6, shield, virtual, A_NORMAL);
			DrawLine(69, 67, 69+player_shield/(40/3), 67, A_NORMAL);
			//Draw Info
			printf_xy(70, 29, "level: %d", cluster_level/5 - 3);
			if(heat_accuracy == 1)
				printf_xy(70, 41, "level: %d", 4-heat_level);
			if(heat_level == 1 && heat_accuracy == 2)
				printf_xy(70, 41, "level: 4");
			if(heat_level == 1 && heat_accuracy == 3)
				printf_xy(70, 41, "level: 5");	
			printf_xy(70, 53, "level: %d", (laser_level-15)/10+1);
			DrawStr(69, 65, "         ", A_REPLACE);
			printf_xy(70, 65, "%d/200", player_shield);
			memcpy (LCD_MEM, virtual, LCD_SIZE);
			pause();
			init();
		}
		if(_rowread(0xDF)&0x40) //APPS - Quick exit
		{
			game_quick_exit = 1;
			stop();
			end();
		}
		if(_rowread(0xF7)&0x80 && player_explosion == 0 && laser_press == 0 && laser_y < 0 && item_laser > 0) //F3 - Lasers
		{
			item_laser--;
			laser_press = 1;
			laser_y = lcd_height-13-1;
			laser_x = player_x + 9;
			laser_length = laser_level;
		}
		else
			laser_press = 0;
		if(_rowread(0xBF)&0x1) //Esc - quit game
		{
			stop();
			if(main_menu() == -1)
			{
				high();
				quit();
			}
			pause();
			init();
		}
		//============================PAINT===================================
		paint();
		for(x = 0; x < game_speed; x++);
	}
}

//=============================PAUSE FUNCTION======================
int pause()
{
	int x = 0;
	while(!(_rowread(0xBF)&0x1) && !(_rowread(0xFD)&0x1));
	if(_rowread(0xBF)&0x1) x = 1; 
	while(_rowread(0xBF)&0x1 || _rowread(0xFD)&0x1);
	return x;	//Returns 0 if you hit enter, 1 if you hit ESC
}

//===============================INITIALIZE===============================
void init(void)
{
	PortSet(virtual, 239, 127);
	FontSetSys(F_4x6);
	OSSetSR (0x0700);
}

//==============================EXIT OPERATIONS===========================
void stop(void)
{
	FontSetSys(F_6x8);
	OSSetSR (0x0000);
	PortRestore();
	GrayMode(GRAY_OFF);
}

//==============================BRINGS UP THE MENU==========================
void menu(int x, int y, int w, int h, int m)
{
	int z;
	//Center the screen
	if(m == 1)
	{
		x = lcd_width/2 - w/2;
		y = lcd_height/2 - h/2;
	}
	//Clear the area
	for(z = 0; z < h; z++)
		DrawLine(x, y+z, x+w, y+z, A_REVERSE);
	//Draw outside layer
	DrawLine(x+1, y, x+w-2, y, A_NORMAL);
	DrawLine(x+1, y+h-1, x+w-1, y+h-1, A_NORMAL);
	DrawLine(x, y+1, x, y+h-2, A_NORMAL);
	DrawLine(x+w-1, y+1, x+w-1, y+h-2, A_NORMAL);
	//Draw outside shadow
	DrawLine(x+2, y+h, x+w-1, y+h, A_NORMAL);
	DrawLine(x+w, y+2, x+w, y+h-1, A_NORMAL);
	//Draw inside layer
	DrawLine(x+2, y+2, x+w-3, y+2, A_NORMAL);
	DrawLine(x+2, y+h-3, x+w-3, y+h-3, A_NORMAL);
	DrawLine(x+2, y+2, x+2, y+h-3, A_NORMAL);
	DrawLine(x+w-3, y+2, x+w-3, y+h-3, A_NORMAL);
	//Draw inside shadow
	DrawLine(x+2, y+3, x+w-3, y+3, A_NORMAL);
	DrawLine(x+3, y+2, x+3, y+h-3, A_NORMAL);
}

//===============================CHOOSE MENU================================
int choose(int x, int y, int w, int h, int spacing, int number, int current)
{
	int count;	//Used for FOR loops
	int z;		//Used for getting input
	stop();
	for(;;)
	{
		pushkey(0);
		z = 0;
		for(count = 0; count < h; count++)
			DrawLine(x, y+count+(current-1)*spacing, x+w, y+count+(current-1)*spacing, A_XOR);
		while(z != 264 && z != 13 && z != 337 && z != 340)
			z = ngetchx();
		if(z == 264) return 0;
		if(z == 13) 
		{
			for(count = 0; count < h; count++)
				DrawLine(x, y+count+(current-1)*spacing, x+w, y+count+(current-1)*spacing, A_XOR);
			return current;
		}
		for(count = 0; count < h; count++)
			DrawLine(x, y+count+(current-1)*spacing, x+w, y+count+(current-1)*spacing, A_XOR);
		if(z == 337)
		{
			current -= 1;
			if(current < 1) current = number;
		}
		if(z == 340)
		{
			current += 1;
			if(current > number) current = 1;
		}
	}
	init();
}

//================================InputStr================================
void InputStr(int x, int y, int max)
{
	int i = 0;
	int z = 0;
	int c;
	for(c = 0; c < max; c++) name1[c] = 32;
	stop();
	printf_xy(x, y, "_");
   for(;;)
   {
   	z = ngetchx();
   	if(z == 264) continue;
   	if(z == 13) break;
   	if(z == 257 && i > 0) 
   	{
   		i--;
   		name1[i] = 32;
   		printf_xy(x + i*6, y, "_ ");
   	}
   	if(z != 257 && i < max)
   	{
   		name1[i] = z;
   		printf_xy(x + i*6, y, "%c_ ", name1[i]);
   		i++;
   	}
   }
   init();
}

//=================================HIGH SCORE=================================
void high(void)
{	
	int x;
	int y;
	int place = 5;
	stop();
	//Check if you are in high score
	for(x = 0; x < 5; x++)
	{
		if(game_level >= high_score[x])
		{
			place = x;
			break;
		}
	}
	//If so, Put you in high score
	if(place < 5)
	{
		clrscr();
		FontSetSys(F_6x8);
		DrawStr(33, 10, "Congratulations!", A_NORMAL); 
		FontSetSys(F_4x6);
		DrawStr(23, 22, "You made it into the hall of fame!", A_NORMAL);
		for(y = 0; y < 4-place; y++)	//Shift all names down
		{
			for(x = 0; x < 12; x++)
				high_name[12*4+x - 12*y] = high_name[12*3+x - 12*y];
			high_score[4-y] = high_score[3-y];
		}
		InputStr(40, 45, 12);
		for(x = 0; x < 12; x++)
		{
			high_name[place*12+x] = name1[x];
		}
		high_score[place] = game_level;
	}
	init();
	if(game_lives >= 0) pause();
	stop();
	//Display High Scores
	clrscr();
	FontSetSys(F_4x6);
	DrawStr(60, 91, "By: Jason Ho", A_NORMAL);
	for(y = 0; y < 5; y++)
	{
		for(x = 0; x < 12; x++)
		{
			printf_xy(15+x*6, 36+y*10, "%c", high_name[x+12*y]);
		}
		printf_xy(120, 36+y*10, "%d", high_score[y]);
	}
	FontSetSys(F_8x10);
	DrawStr(32, 3, "HALL OF FAME", A_NORMAL);
	FontSetSys(F_6x8);
	DrawStr(15, 24, "Pilot", A_NORMAL);
	DrawStr(109, 24, "Level", A_NORMAL);
	DrawLine(15, 32, 138, 32, A_NORMAL);
	init();
	pause();
}

//==========================QUIT AND DISPLAY GAME OVER===========================

void quit(void)
{
	int z = 0;
	stop();
	GrayMode(GRAY_ON);
	SetPlane(LIGHT_PLANE);
	clrscr();
	FontSetSys(F_8x10);
	DrawStr(48, 38, "GAME OVER", A_NORMAL);
	FontSetSys(F_4x6);
	DrawStr(62, 50, "By: Jason Ho", A_NORMAL);
	SetPlane(DARK_PLANE);
	clrscr();
	FontSetSys(F_8x10);
	DrawStr(46, 36, "GAME OVER", A_NORMAL);
	FontSetSys(F_4x6);
	DrawStr(62, 50, "By: Jason Ho", A_NORMAL);
	pushkey(0);
	while(z != 13 && z != 264) z = ngetchx();
	stop();
	end();
}

//===============================THE MAIN MENU===============================
int main_menu(void)
{
	int y = 1;
	char temp[3840];
	back:
	menu(0,0,62,60,1);
	FontSetSys(F_4x6);
	DrawStr(62, 28, "PLAY GAME", A_NORMAL);
	DrawStr(67, 38, "OPTIONS", A_NORMAL);
	DrawStr(62, 48, "HIGH SCORE", A_NORMAL);
	DrawStr(72, 58, "HELP", A_NORMAL);
	DrawStr(63, 68, "QUIT GAME", A_NORMAL);
	y = choose(56,26,48,9,10,5,y);
	switch(y)
	{
		case 0:
			return 0;
		case 1:
			break;
		case 2:
			LCD_save(temp);
			y = 1;
		option:
			menu(0,0,82,47,1);
			FontSetSys(F_4x6);
			DrawStr(70, 35, "Speed", A_NORMAL);
			DrawStr(53, 44, "Enemy Explosion", A_NORMAL);
			DrawStr(47, 53, "Item/Shield Display", A_NORMAL);
			DrawStr(66, 62, "Contrast", A_NORMAL);
			y = choose(45,33,70,9,9,4,y);
			if(y == 1)
			{
				for(;;)
				{
					menu(50,20,39,35,1);
					FontSetSys(F_4x6);
					printf_xy(77, 39, "%d", 99-game_speed/200);
					DrawStr(70, 48, "Faster", A_NORMAL);
					DrawStr(68, 57, "Slower", A_NORMAL);
					y = choose(67,47,27,7,9,2,y);
					if(y == 0) break;
					if(y == 1)
					{
						game_speed -= 200;
						if(game_speed < 0) game_speed = 19800;
					}
					if(y == 2)
					{
						game_speed += 200;
						if(game_speed > 19800) game_speed = 0;
					}
				}
				y = 1;
				goto option;
			}
			if(y == 2)
			{
				menu(50,20,32,29,1);
				FontSetSys(F_4x6);
				DrawClipEllipse(74, 46, 3, 3, &(SCR_RECT){{0, 0, 159, 99}}, A_NORMAL);
				DrawClipEllipse(74, 55, 3, 3, &(SCR_RECT){{0, 0, 159, 99}}, A_NORMAL);
				DrawStr(80, 44, "On", A_NORMAL);
				DrawStr(80, 53, "Off", A_NORMAL);
				y = choose(73,45,2,3,9,2,2-enemy_explosion_show);
				if(y != 0) enemy_explosion_show = 2-y;
				y = 2;
				goto option;
			}
			if(y == 3)
			{
				menu(50,20,32,29,1);
				FontSetSys(F_4x6);
				DrawClipEllipse(74, 46, 3, 3, &(SCR_RECT){{0, 0, 159, 99}}, A_NORMAL);
				DrawClipEllipse(74, 55, 3, 3, &(SCR_RECT){{0, 0, 159, 99}}, A_NORMAL);
				DrawStr(80, 44, "On", A_NORMAL);
				DrawStr(80, 53, "Off", A_NORMAL);
				y = choose(73,45,2,3,9,2,2-item_show);
				if(y != 0) item_show = 2-y;
				y = 3;
				goto option;
			}
			if(y == 4)
			{
				y = 1;
				for(;;)
				{
					menu(50,20,32,29,1);
					FontSetSys(F_4x6);
					DrawStr(77, 44, "Up", A_NORMAL);
					DrawStr(71, 53, "Down", A_NORMAL);
					y = choose(70,43,20,7,9,2,y);
					if(y == 0) break;
					if(y == 1) OSContrastUp();
					if(y == 2) OSContrastDn();
				}
				y = 4;
				goto option;
			}
			y = 2;
			LCD_restore(temp);
			goto back;
		case 3:
			LCD_save(temp);
			init();
			y = game_level;
			game_level = -1;
			high();
			game_level = y;
			y = 3;
			LCD_restore(temp);
			goto back;
		case 4:
			LCD_save(temp);
			menu(11,2,138,95,0);
			FontSetSys(F_4x6);
			DrawStr(21, 9, "[2nd] - Fire Bullets", A_NORMAL);
			DrawStr(21, 16, "[LEFT] - Move Left", A_NORMAL);
			DrawStr(21, 23, "[RIGHT] - Move Right", A_NORMAL);
			DrawStr(21, 30, "[UP] - Activate Shields", A_NORMAL);
			DrawStr(21, 37, "[DOWN] - Teleport to the other side", A_NORMAL);
			DrawStr(21, 44, "[F1] - Cluster Bomb", A_NORMAL);
			DrawStr(21, 51, "[F2] - Heat Seeker", A_NORMAL);
			DrawStr(21, 58, "[F3] - Lasers", A_NORMAL);
			DrawStr(21, 65, "[F4] - Disruption Field", A_NORMAL);
			DrawStr(21, 72, "[F5] - Check Item Levels", A_NORMAL);
			DrawStr(21, 79, "[HOME] - Quick Exit", A_NORMAL);
			DrawStr(21, 86, "[ESC] - Main Menu/Quit Game", A_NORMAL);
			while(y != 13 && y != 264) y = ngetchx();
			y = 4;
			LCD_restore(temp);
			goto back;
		case 5:
			return(-1);
	}
	return(1);
}

//===============================INTRODUCTION================================
void intro(void)
{
	int z = 0;
	int y = 0;
	GrayMode(GRAY_ON);
	for(z = 0; z < lcd_height; z++)
	{
		SetPlane(LIGHT_PLANE);
		clrscr();
		FontSetSys(F_8x10);
		DrawStr(24, z, "Alien Invasion", A_NORMAL);
		FontSetSys(F_4x6);
		DrawStr(62, z+12, "By: Ben Cherry", A_NORMAL);
		SetPlane(DARK_PLANE);
		clrscr();
		FontSetSys(F_8x10);
		DrawStr(22, 90-z, "Alien Invasion", A_NORMAL);
		FontSetSys(F_4x6);
		DrawStr(62, 81-z, "By: Ben Cherry", A_NORMAL);
		DrawStr(143, 93, "v1.5", A_NORMAL);
		if(z == 46)
		{
			while(y != 13 && y != 264)
				y = ngetchx();
			if(y == 264) 
			{
				stop();
				end();
			}
			if(y == 13)
				if(main_menu() <= 0) end();
			GrayMode(GRAY_ON);
		}
	}
	GrayMode(GRAY_OFF);
}

//===============================CHANGE SHIP DIRECTIONS==========================
void changeship(void)
{
	int x;
	for(x = 0; x < 5; x++)
	{
		if(enemy_alive[x] == 1)
		{
			enemy_Vx[x] = random(4)-2;
			enemy_Vy[x] = random(4)-2;
			if(enemy_Vx[x] == 0 && enemy_Vy[x] == 0) enemy_Vx[x] = 2; 
		}
	}
}

//==============================RESET UPGRADES============================
void reset(void)
{
	game_fire = 1;
	player_shield = 200;
	item_disruption = 5;
	item_heat = 5;
	item_cluster = 5;
	item_laser = 5;
	laser_level = 15;
	cluster_level = 20;
	heat_level = 3;
	heat_accuracy = 1;
}

//==========================START NEW GAME/SET VARIABLES==================
void start(void)
{
	int x;
	player_x = lcd_width/2 - 13/2;
	player_explosion = 0;
	game_level = 0;
	game_lives = 3;
	cluster_y = -1;
	cluster_radius = 0;
	cluster_press = 0;
	heat_y = -1;
	laser_y = -1;
	laser_press = 0;
	laser_length = -2;
	upgrade_y = lcd_height + 1;
	player_invincible = 0;
	repeat1 = 0;
	repeat2 = 0;
	enemy_bullet_accuracy = 0;
	enemy_bullet_speed = 1;
	player_invincible_time = 150;
	for(x = 0; x < 5; x++)
	{
		enemy_alive[x] = 0;
		player_bullet_y[x] = 0;
		enemy_bullet_y[x] = lcd_height + 1;
		enemy_explosion[x] = 0;
		enemy_y[x] = 3;
	}
	for(x = 0; x < 30; x++)
	{
		enemy1_particles_y[x] = LCD_HEIGHT + 1;
		enemy1_particles_x[x] = -1;
	}
	reset();
	changeship();
}

//=============================DRAW SHIPS================================
void drawship(int ship, int frame, int x)
{
	if(enemy_alive[x] == 1)
	{
		if(ship == 1) Sprite32(enemy_x[x], enemy_y[x], 17, enemy1[frame-1], virtual, A_NORMAL);
		if(ship == 2 && (frame == 1 || frame == 3)) Sprite32(enemy_x[x], enemy_y[x], 17, enemy2[0], virtual, A_NORMAL);
		if(ship == 2 && (frame == 2 || frame == 4)) Sprite32(enemy_x[x], enemy_y[x], 17, enemy2[1], virtual, A_NORMAL);
		if(ship == 3) Sprite32(enemy_x[x], enemy_y[x], 17, enemy3, virtual, A_NORMAL);
		if(ship == 4) Sprite32(enemy_x[x], enemy_y[x], 17, enemy4[frame-1], virtual, A_NORMAL);
		if(ship == 5 && frame == 1) Sprite32(enemy_x[x], enemy_y[x], 17, enemy5[0], virtual, A_NORMAL);
		if(ship == 5 && (frame == 2 || frame == 4)) Sprite32(enemy_x[x], enemy_y[x], 17, enemy5[1], virtual, A_NORMAL);
		if(ship == 5 && frame == 3) Sprite32(enemy_x[x], enemy_y[x], 17, enemy5[2], virtual, A_NORMAL);
		if(ship == 6 && repeat1%2 == 0) Sprite32(enemy_x[x], enemy_y[x], 14, enemy6[0], virtual, A_NORMAL);
		if(ship == 6 && (repeat1-1)%2 == 0) Sprite32(enemy_x[x], enemy_y[x], 14, enemy6[1], virtual, A_NORMAL);
		if(ship == 7 && repeat1%2 == 0) Sprite32(enemy_x[x], enemy_y[x], 17, enemy7[0], virtual, A_NORMAL);
		if(ship == 7 && (repeat1-1)%2 == 0) Sprite32(enemy_x[x], enemy_y[x], 17, enemy7[1], virtual, A_NORMAL);
	}
}

//===============================Let the enemy shoot================================
void enemyshoot(void)
{
	int x;
	for(x = 0; x < 5; x++)
	{
		if(enemy_alive[x] == 1)
		{
			enemy_bullet_x[x] = enemy_x[x] + 17/2;
			enemy_bullet_y[x] = enemy_y[x] + 17;
			if(enemy_bullet_x[x] > lcd_width - 3) enemy_bullet_x[x] = lcd_width - 3;
			if(enemy_bullet_y[x] > lcd_height - 3) enemy_bullet_y[x] = lcd_height - 3;
		}
	}
}

//===============================DROP UPGRADE================================
void upgrade_test(int enemy)
{
	int y = 0;
	int x;
	for(x = 0; x < 5; x++)
	{
		if(enemy_alive[x] != 0) y = 1;
	}
	if(y == 0 && upgrade_y > lcd_height)
	{
		upgrade_x = enemy_x[enemy]+13/2;
		upgrade_y = enemy_y[enemy]+13;
	}
}

//===============================CHOOSE UPGRADE===============================
void upgrade_get(int x)
{
	//If you got upgrade multi-fire
	if(x == 1)
	{
		if(game_fire == 5) x = random(8) + 2;
		if(game_fire == 3)
		{
			if(game_level >= 20)
				game_fire = 5;
			else
			{
				x = random(8) + 2;
			}
		}
		if(game_fire == 1)
			game_fire = 3;
	}
	//If you got upgrade recharge shields
	if(x == 2)
		player_shield = 200;
	if(x == 3)
		item_disruption += 2 + (game_level/50);
	if(x == 4)
		item_heat += 2 + (game_level/50);
	if(x == 5)
		item_cluster += 2 + (game_level/50);
	if(x == 6)
		item_laser += 2 + (game_level/50);
	if(x == 7)
		cluster_level += 5 + 5*(int)(game_level/50);
	if(x == 8)
		laser_level += 10 + 10*(int)(game_level/50);
	//===Control the Max level for laser and cluster
	if(cluster_level > 115)
	{
		cluster_level = 115;
		player_shield = 200;
	}
	if(laser_level > 205)
	{
		laser_level = 205;
		player_shield = 200;
	}
	//====
	if(x == 9)
	{	
		heat_level--;
		if(heat_level == 0 && heat_accuracy == 1)
		{
			heat_level = 1;
			heat_accuracy = 2;
		}
		if(heat_level == 0 && heat_accuracy == 2)
		{
			heat_level = 1;
			heat_accuracy = 3;
		}
		if(heat_level == 0 && heat_accuracy == 3)
		{
			heat_level = 1;
			player_shield = 200;
		}
	}
}

//==============================PAINT THE SCREEN==========================
void paint(void)
{
	int x;
	int y;
	clrscr();
	printf_xy(0, 1, "Level: %d", game_level);
	Sprite16(130, 0, 8, heart, virtual, A_NORMAL);
	if(item_show == 1)
	{
		Sprite8(43, 0, 7, cluster, virtual, A_NORMAL);
		printf_xy(52, 1, "%d", item_cluster);
		Sprite8(64, 1, 5, heat, virtual, A_NORMAL);
		printf_xy(73, 1, "%d", item_heat);
		DrawLine(88, 1, 88, 5, A_NORMAL);
		printf_xy(93, 1, "%d", item_laser);
		Sprite8(109, 2, 3, disruption, virtual, A_NORMAL);
		printf_xy(117, 1, "%d", item_disruption);
		Sprite32(125, 94, 6, shield, virtual, A_NORMAL);
		DrawLine(140, 97, 140+player_shield/(40/3), 97, A_NORMAL);
	}
	if(player_shield_on == 1 && repeat1%3 == 0) DrawClipEllipse(player_x + 9, lcd_height - 5, 10, 10, &(SCR_RECT){{0, 0, 159, 99}}, A_NORMAL);
	printf_xy(148, 1, "x%d", game_lives);
	if(upgrade_y <= lcd_height)
	{
		Sprite16(upgrade_x, upgrade_y, 7, upgrade, virtual, A_NORMAL);
		upgrade_y++;
	}
	if(player_invincible > 0)
	{
		repeat2++;
		if(repeat2 > 3) repeat2 = 1;
		player_invincible++;
		if(player_invincible > player_invincible_time) 
		{
			player_invincible = 0;
			player_invincible_time = 150;
			repeat2 = 0;
		}
	}
	if(player_explosion == 0 && repeat2%2 == 0) Sprite16(player_x, lcd_height-13, 13, player_ship, virtual, A_NORMAL);
	repeat1++;
	//Test to see which enemy picture to draw
	if(repeat1 < 7) enemy_frame = 1;
	if(repeat1 >= 7 && repeat1 < 14) enemy_frame = 2;
	if(repeat1 >= 14 && repeat1 < 21) enemy_frame = 3;
	if(repeat1 >= 21 && repeat1 <= 28) enemy_frame = 4;
	if(repeat1 > 28) repeat1 = 0;
	//Draw enemy explosion particles
	y = 0;
	for(x = 0; x < 30; x++)
	{
		if(enemy1_particles_x[x] >= 0 && enemy1_particles_y[x] <= lcd_height && enemy1_particles_x[x] <= lcd_width && enemy1_particles_y[x] >= 0)
		{
			if(enemy_explosion_show == 1) Sprite8(enemy1_particles_x[x], enemy1_particles_y[x], 1, enemy_particle, virtual, A_NORMAL);
			enemy1_particles_x[x] += enemy_particles_Vx[y];
			enemy1_particles_y[x] += enemy_particles_Vy[y];
		}
		y++;
		if(y == 6) y = 0;
	}
	//Draw player explosion
	if(player_explosion > 0)
	{
		if(player_explosion < 35) DrawClipEllipse(player_x + 13, lcd_height - 13 + 17/2, player_explosion + 5, player_explosion + 5, &(SCR_RECT){{0, 0, 159, 99}}, A_NORMAL);
		player_explosion++;
		if(player_explosion > 100)
		{	
			game_lives -= 1;
			if(game_lives < 0)
			{
				high();
				quit();
			}
			reset();
			player_x = lcd_width/2 - 13/2;
			player_invincible = 1;
			player_explosion = 0;
		}
	}
	if(player_explosion > 0)
	{
		for(x = 0; x < 6; x++)
		{
			if(player_particles_y[x] >= 0 && player_particles_x[x] >= 0 && player_particles_x[x] <= lcd_width)
				Sprite8(player_particles_x[x], player_particles_y[x], 5, particle, virtual, A_NORMAL);
		}
		player_particles_x[0] -= 2;
		player_particles_x[1] -= 1;
		player_particles_x[2] -= 0;
		player_particles_x[3] += 1;
		player_particles_x[4] += 2;
		player_particles_x[5] -= 3;
		player_particles_y[0] -= 1;
		player_particles_y[1] -= 1;
		player_particles_y[2] -= 2;
		player_particles_y[3] -= 2;
		player_particles_y[4] -= 1;
		player_particles_y[5] -= 1;
	}
	//Draw enemy explosion
	for(x = 0; x < 5; x++)
	{
		if(enemy_explosion[x] == 1) //Initiate Enemy particles
		{
			for(y = 6*x; y < 6*x + 6; y++)
			{
				enemy1_particles_x[y] = enemy_x[x] + 17/2;
				enemy1_particles_y[y] = enemy_y[x] + 17/2;
			}
		}
		if(enemy_explosion[x] != 0)
		{
			DrawClipEllipse(enemy_x[x] + 20, enemy_y[x] + 17/2, enemy_explosion[x]+5, enemy_explosion[x]+5, &(SCR_RECT){{0, 0, 159, 99}}, A_NORMAL);
			enemy_explosion[x]++;
			if(enemy_explosion[x] > 15) enemy_explosion[x] = 0;
		}
	}
	//Draw enemy picture
	for(x = 0; x < 5; x++) drawship(enemy_ship, enemy_frame, x);
	//Draw your bullets
	for(x = 0; x < game_fire; x++)
	{
		if(player_bullet_x[x] != 0 && player_bullet_y[x] != 0)
			Sprite8(player_bullet_x[x], player_bullet_y[x], 4, player_bullet, virtual, A_NORMAL);
	}
	//Draw enemy bullets
	for(x = 0; x < 5; x++)
	{
		if(enemy_bullet_y[x] < lcd_height)
			Sprite8(enemy_bullet_x[x], enemy_bullet_y[x], 3, enemy_bullet, virtual, A_NORMAL);
	}
	//Draw special upgrades
	if(cluster_y >= 0) //Cluster Bomb
	{
		Sprite8(cluster_x, cluster_y, 7, cluster, virtual, A_NORMAL);
		cluster_y -= 2;
		if(cluster_y < 0) cluster_press = 0;
	}
	if(cluster_radius > 0)
	{
		if(cluster_radius == 1)
		{
			cluster_at_y = cluster_y;
			cluster_y = -1;
		}
		DrawClipEllipse(cluster_x + 4, cluster_at_y, cluster_radius, cluster_radius, &(SCR_RECT){{0, 0, 159, 99}}, A_NORMAL);
		cluster_radius++;
		if(cluster_radius > cluster_level)
		{ 
			cluster_radius = 0;
			cluster_press = 0;
		}
	}
	if(heat_y >= 0) //Heat Seaker
	{
		Sprite8(heat_x, heat_y, 5, heat, virtual, A_NORMAL);
		heat_y -= 2;
		if(heat_x > enemy_x[heat_target] + 17/2 && repeat1%heat_level == 0) heat_x -= heat_accuracy;
		if(heat_x < enemy_x[heat_target] + 17/2 && repeat1%heat_level == 0) heat_x += heat_accuracy;
	}
	if(laser_y >= 0) //Lasers
	{
		if(laser_y == 0)
		{
			if(laser_length >= 0)
			{
				if(laser_length <= lcd_height - 13)
					DrawLine(laser_x, laser_y + laser_length, laser_x, laser_y, A_NORMAL);
				else
					DrawLine(laser_x, lcd_height - 13, laser_x, laser_y, A_NORMAL);
			}
			else
				laser_y = -1;
			laser_length -= 2;
		}
		if(lcd_height - laser_y - 13 <= laser_length)
			DrawLine(laser_x, lcd_height-13, laser_x, laser_y, A_NORMAL);
		if(lcd_height - laser_y - 13 > laser_length && laser_y > 0)
			DrawLine(laser_x, laser_y + laser_length, laser_x, laser_y, A_NORMAL);
		if(laser_y != 0) laser_y -= 2;
	}
	memcpy (LCD_MEM, virtual, LCD_SIZE);
}

void end(void)
{
	free(virtual);
	exit(0);
}
