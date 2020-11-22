/*
The game module
*/

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 1
#define _GNU_SOURCE 1
#endif

//INCLUDES
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>


//DEFINES
#define SCREEN_WIDTH 	320
#define SCREEN_HEIGHT 	240
#define SCR_BPP 2  //16 bits per pixel in bytes
#define BUTTON1 0xFE
#define BUTTON2 0xFD
#define BUTTON3 0xFB
#define BUTTON4 0xF7
#define BUTTON5 0xEF
#define BUTTON6 0xDF
#define BUTTON7 0xBF
#define BUTTON8 0x7F
#define Black		0x0000
#define White 		0xFFFF
#define LightGrey   0xC618
#define Red 		0xF000
#define Blue		0X000F

//FUCTION DEFINITIONS
bool toggle(bool in);
void button_handler(int signo);
void init_display();
void update_screen();
int get_row (int pixel);
int get_col (int pixel, int row);
void fill_block(int maxR, int minR, int maxC, int minC, int player);

//VARIABLES
static FILE* device_file;
struct fb_copyarea frame;
uint16_t *frame_buffer;
int fbfd = 0;

uint8_t last_input;
int last_player;


//Get row the pixel is located in
int get_row (int pixel)
{
  return (int) pixel / 320;
}

//Get colon the pixel is located in
int get_col (int pixel, int row)
{
  return (int) ((int)pixel - (row * 320));
}

//Draw a block with
void fill_block(int maxR, int minR, int maxC, int minC, int player)
{
  int i;
  printf("player = %d\n", player);
  for (i = 0; i < 240*320; i++)
  {
  	int row = get_row(i);
  	int col = get_col(i, row);
    if (((col < maxC) && (col >= minC)) && ((row < maxR) && (row >= minR)))
    {
      if (player)
      {
        frame_buffer[i] = Red;
      }
      else
      {
        frame_buffer[i] = Blue;
      }
    }
  }
}

//Set the display at the start of the game
void init_display(){

	frame.dx = 0;
	frame.dy = 0;
	frame.width = SCREEN_WIDTH;
	frame.height = SCREEN_HEIGHT;
	fbfd = open("/dev/fb0", O_RDWR);
	
	if(fbfd == -1) {
		printf("Error: unable to open frame_buffer, exiting...\n)");
		exit(EXIT_FAILURE);

	}
	
	int screensize = SCREEN_WIDTH * SCREEN_HEIGHT * SCR_BPP; 	
	frame_buffer = (uint16_t*) mmap(NULL, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	
	
	if(frame_buffer == MAP_FAILED) {
		printf("Error: unable to map frame_buffer, exiting...\n");
		close(fbfd);
		exit(EXIT_FAILURE);
	}
	
	int i;
	
	// Making the background white
	for (i = 0; i < 320*240; i++){
		frame_buffer[i] = 0xffff;
	}
	
	
	//set up grid
	for (i=0; i<320*240; i++){
		int row = get_row(i);
		int col = get_col(i, row);
		if((col >= 75 && col < 80) || (col >= 155 && col < 160) || (col >= 235 && col < 240)) {
			frame_buffer[i]=0x0000;
		}
		if((row >= 75 && row <80) || (row >= 155 && row <160)){
			frame_buffer[i]=0x0000;
		}
		if(col>=240){
			frame_buffer[i]= 0xffff;
		}
	}
}


//Update the screen
void update_screen() {
    ioctl(fbfd, 0x4680, &frame);
}

//Set an delay in seconds
void delay(int number_of_seconds) 
{ 

    int milli_seconds = 1000 * number_of_seconds; 
  
    //Storing start time 
    clock_t start_time = clock(); 
  
    //Looping while required time is not achieved 
    while (clock() < start_time + milli_seconds) 
        ; 
}

//Function to run at all times
int main(int argc, char *argv[])
{
    init_display();
	printf("Hello World, I'm game!\n");
	last_input = 0;
	last_player = true;
    int status = init_gamepad();
    if (status != EXIT_SUCCESS) {
        exit(status);
    }

    while (true) {
        update_screen();
    }
	exit(EXIT_SUCCESS);
}

//Do all the setups so we can play the game
int init_gamepad(){
	printf("Open init gamepad");
  device_file = fopen("/dev/gamepad", "rb");
  
  if (!device_file) {
    printf("[GAMEPAD] ERROR: Couldn't open gamepad device_file...\n"); 
  return EXIT_FAILURE;
  }

  if (signal(SIGIO, &button_handler) == SIG_ERR)
  {
    printf("[GPIO] ERROR: Signal handler error...\n");
    return EXIT_FAILURE; 
  }

  if (fcntl(fileno(device_file), F_SETOWN, getpid()) == -1)
  {
    return EXIT_FAILURE;
  }

  long oflags = fcntl(fileno(device_file), F_GETFL);
  if (fcntl(fileno(device_file), F_SETFL, oflags | FASYNC) == -1)
  {
    printf("[FLAG] ERROR: Couldn't set FASYNC flag...\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

//Switches value of the boolean input
 bool toggle(bool in){
	 if(in){
		 return false;
	 }else{
		 return true;
	 }
 }

//Handles buttonpress
void button_handler(int signo)
{
	printf("button_handler");
      printf("player = %d\n", last_player);
    
    int button = fgetc(device_file);
    
     switch (button)
  {
    case BUTTON1:
      //11
      if (last_input == BUTTON1) 
      {
        fill_block(75,0,75,0,last_player);
        last_input = 0;
        last_player = toggle(last_player);
        break;
      }
      //21
      else if (last_input == BUTTON2)
      {
        fill_block(155,80,75,0,last_player);
        last_input = 0;
        last_player = toggle(last_player);
        break;
      }
      //31
      else if (last_input == BUTTON3)
      {
        fill_block(240,160,75,0,last_player);
        last_input = 0;
        last_player = toggle(last_player);
        break;
      }
      last_input = button;      
      break;
      
    case BUTTON2:
      //12
      if (last_input == BUTTON1)
      {
        fill_block(75,0,155,80,last_player);
        last_input = 0;
        last_player = toggle(last_player);
        break;
      }
      //22
      else if (last_input == BUTTON2)
      {
        fill_block(155,80,155,80,last_player);
        last_input = 0;
        last_player = toggle(last_player);
        break;
      }
      //32
      else if (last_input == BUTTON3)
      {
        fill_block(240,160,155,80,last_player);
        last_input = 0;
        last_player = toggle(last_player);
        break;
      }
      last_input = button;
      break;

    case BUTTON3:
      //13
      if (last_input == BUTTON1)
      {
        fill_block(75,0,235,160,last_player);
        last_input = 0;
        last_player = toggle(last_player);
        break;
      }
      //23
      else if (last_input == BUTTON2)
      {
        fill_block(155,80,235,160,last_player);
        last_input = 0;
        last_player = toggle(last_player);
        break;
      }
      //33
      else if (last_input == BUTTON3)
      {
        fill_block(240,160,235,160,last_player);
        last_input = 0;
        last_player = toggle(last_player);
        break;
      }
      last_input = button;
      printf("player = %d\n", last_player);
      break;
    delay(0.2);
  }
}
