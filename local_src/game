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

static void init_game();
static void update_game();

struct fb_copyarea rect;
int fbfd; // Framebuffer
int gamepad_driver; // Gamepad

int square_dim[50]; // Dimension of the squares vec
int width = 50; // Dimension of squares
int red_square; // The position of the red square
int blue_square; // The position of the blue square
int step_size; // the step size of the squares
uint16_t *pixels;
int finished_setup; // Variable marking when the setup of the game is finished

static void init_game(){
	width = 50;
	red_square = 50 + (320 * 60);
	blue_square = 270 + (320 * 60);
	step_size= 15;
	int i;
	int j;
	for(i = 0; i < (sizeof(square_dim) / sizeof(*square_dim)); i++){
		square_dim[i] = 320 * i;
	}
	
	// Making the background white
	for (i = 0; i < 320*240; i++){
		pixels[i] = 0xffff;
	}
	
	rect.dx = 0;
	rect.dy = 0;
	rect.width = 320;
	rect.height = 240;
	ioctl(fbfd, 0x4680, &rect);

}

static void button_interrupt_handler(int unused)
{
	int button = 0;
	// read interrupt message only if interrupts are enabled and game is not over
	if (finished_setup){
		read(gamepad_driver, &button, sizeof(int));
	}
	
	// check if button 2 is pressed
	if ((~button & 0b10) == 0b10){
		// check if the new movement goes out of bound
		if (red_square - 320 * step_size > 0){
			red_square -= 320 * step_size;
		}	
	}
	
	if ((~button & 0b100) == 0b100){
		// check if the new movement goes out of bound
		if (red_square - 320 * step_size > 0){
			red_square -= 320 * step_size;
		}	
	}
	
	// check if button 4 is pressed
	if ((~button & 0b1000) == 0b1000){
		// check if new movement goes out of bound
		if (red_square + 320 * (sizeof(square_dim) / sizeof(*square_dim)) + (320 * step_size) <= 76800 + 320 * step_size){
			red_square += 320 * step_size;
		}	
	
	}
	
	// check if button 6 is pressed
	if((~button & 0b100000) == 0b100000){
	 	// check if new movement goes out of bound
		if (blue_square - 320 * step_size > 0){
			blue_square -= 320 * step_size;
		}	
	}
	
	// check if button 8 is pressed
	if((~button & 0b10000000) == 0b10000000){
		// check if new movement goes out of bound
		if (blue_square + 320 * (sizeof(square_dim) / sizeof(*square_dim)) + (320 * step_size) <= 76800 + 320 * step_size){
			blue_square += 320 * step_size;
		}	
	}
}






int main(int argc, char *argv[])
{
	// Set up the drivers
	struct sigaction button_action = {}; 
	button_action.sa_handler = &button_interrupt_handler; 
	sigaction(SIGUSR1, &button_action, NULL);
	char command1[50];
	strcpy(command1, "modprobe driver-gamepad");
	system(command1);
	
	gamepad_driver = open("/dev/gamepad_group_2", O_RDWR, (mode_t)0600);
	finished_setup = 1;
	
	// Set up the pixel mapping
	fbfd = open("/dev/fb0", O_RDWR, (mode_t)0600);
	pixels = (uint16_t*)mmap(NULL, 320*240*2, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	
	// Initialize the game
	init_game();
	
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
	int i;  // C doesnt like it in the loop declaration =(
	int j;
	
	// Turn screen white
	for (i = 0; i < 320*240; i++){
		pixels[i] = 0xffff;
	}

	// Color and shape the squares
	for(i = 0; i < width; i++){
		for(j = 0; j < (sizeof(square_dim) / sizeof(*square_dim)); j++){
				pixels[red_square + i + square_dim[j]] = 0xf000;
				pixels[blue_square + i + square_dim[j]] = 0x000f;
			//}
		}
	}
	// Drawing the red square
	rect.dx = red_square % 320;
	rect.dy = 0;
	rect.width = width;
	rect.height = 240;
	ioctl(fbfd, 0x4680, &rect);
	
	// Drawing the blue square
	rect.dx = blue_square % 320;
	rect.dy = 0;
	rect.width = width;
	rect.height = 240;
	ioctl(fbfd, 0x4680, &rect);
}
