#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

//#include "graphics.h"


#define Black		0x0000
#define White 		0xFFFF
#define LightGrey       0xC618
#define Red 		0xF000
#define Blue 		0X000F

static void init_game();
static void update_game();
int get_row( int pixel);
int get_col(int pixel, int row);
void fill_block(int maxR, int minR, int maxC, int minC, int player);

struct fb_copyarea rect;
int fbfd; // Framebuffer
int gamepad_driver; // Gamepad

int square_dim[50]; // Dimension of the squares vec
int width = 50; // Dimension of squares
int red_square; // The position of the red square
int blue_square; // The position of the blue square
int step_size; // the step size of the squares
uint16_t *pixels;
int pixel;
int finished_setup; // Variable marking when the setup of the game is finished
int hop;




static void init_game(){
	
	width = 75;
	red_square = 0;
	blue_square = 160;
	step_size= 80;
	int i;
	
	// Making the background white
	for (i = 0; i < 320*240; i++){
		pixels[i] = 0xffff;
	}
	
	//set up grid
	for (i=0; i<320*240; i++){
		int row = get_row(i);
		int col = get_col(i, row);
		if((col >= 75 && col < 80) || (col >= 155 && col < 160) || (col >= 235 && col < 240)) {
			pixels[i]=0x0000;
		}
		if((row >= 75 && row <80) || (row >= 155 && row <160)){
			pixels[i]=0x0000;
		}
		if(col>=240){
			pixels[i]= 0xffff;
		}
	}
	
	//fill square
	fill_block(75, 80, 75, 0, 1);
	fill_block(235, 160, 235, 160, 0);
	
	rect.dx = 0;
	rect.dy = 0;
	rect.width = 320;
	rect.height = 240;
	ioctl(fbfd, 0x4680, &rect);
	
	
	printf("init finished \n");
}

int get_row (int pixel)
{
  return (int) pixel / 320;
}

int get_col (int pixel, int row)
{
  return (int) ((int)pixel - (row * 320));
}

void fill_block(int maxR, int minR, int maxC, int minC, int player)
{
  int i;
  for (i = 0; i < 240*320; i++)
  {
  	int row = get_row(i);
  	int col = get_col(i, row);
    if (((col < maxC) && (col >= minC)) && ((row < maxR) && (row >= minR)))
    {
      if (player)
      {
        pixels[i] = Red;
      }
      else
      {
        pixels[i] = Blue;
      }
    }
  }
}


static void button_interrupt_handler(int unused)
{
	printf("button pressed \n");
	int buff = 0;
	// read interrupt message only if interrupts are enabled and game is not over
	if (finished_setup){
		read(gamepad_driver, &buff, sizeof(int));
	}
	
	// check if button 2 is pressed
	if ((~buff & 0b10) == 0b10){
		if (red_square - step_size > 0){
			red_square -= step_size;
		}		
	}
	
	// check if button 4 is pressed
	if ((~buff & 0b1000) == 0b1000){
		if (red_square + step_size > 0){
			red_square += step_size;
		}		
	}
	
	// check if button 6 is pressed
	if((~buff & 0b100000) == 0b100000){
	 	if (blue_square - step_size > 0){
			red_square -= step_size;
		}	
	}
	
	// check if button 8 is pressed
	if((~buff & 0b10000000) == 0b10000000){
		if (blue_square + step_size > 0){
			red_square += step_size;
		}		
	}
}






int main(int argc, char *argv[])
{
	// Set up the drivers
	struct sigaction button_action = {}; 
	button_action.sa_handler = &button_interrupt_handler; 
	sigaction(SIGUSR1, &button_action, NULL);
	printf("starting modprobe\n");
	char command1[50];
	strcpy(command1, "modprobe driver-gamepad");
	system(command1);
	
	printf("opening driver\n");
	gamepad_driver = open("/dev/gamepad_group_2", O_RDWR, (mode_t)0600);
	printf("open finished\n");
	finished_setup = 1;
	
	// Set up the pixel mapping
	fbfd = open("/dev/fb0", O_RDWR, (mode_t)0600);
	pixels = (uint16_t*)mmap(NULL, 320*240*2, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	
	//Initialize the game
	init_game();
	//init_framebuffer();
	//draw_grid();
	
	while (1) {
		int count = 0;
		while(1){
			count = count + 1;
			if(count > 80){
				break;
			}
		}
		update_game();
	}
	exit(EXIT_SUCCESS);
}

static void update_game(){
	
	fill_block(75, 0, red_square+width, red_square, 1);
	fill_block(75, 0, blue_square+width, blue_square, 1);
	
}
