#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h";

FILE *gamepad_driver;
FILE *framebuffer_driver;

int playerX = 20;
int playerY = 80;
int cactusY = 80;


void draw_cactus(int posX, int posY);
void draw_player(int posX, int posY)
{
  
}
void draw_ground();
void draw_high_score(int high_score);
void draw_score(int score);
void clear_score(void);
void handler (int sigio);
int init_framebuffer();
void deinit_framebuffer();
void refresh_fb();




int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n");

	exit(EXIT_SUCCESS);
}
