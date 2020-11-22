
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 1
#define _GNU_SOURCE 1
#endif
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <math.h>


#include "display.h"
#include <time.h>

static FILE* device;
static struct Paddle paddle_left;
static struct Paddle paddle_right;
//static struct Ball ball;
uint8_t last_input;
int last_player;
 bool toggle(bool in);
 void button_handler(int signo);


#define BUTTON1 0xFE
#define BUTTON2 0xFD
#define BUTTON3 0xFB
#define BUTTON4 0xF7
#define BUTTON5 0xEF
#define BUTTON6 0xDF
#define BUTTON7 0xBF
#define BUTTON8 0x7F


void delay(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Storing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not achieved 
    while (clock() < start_time + milli_seconds) 
        ; 
}

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
        flush_display_buffer();
    }

	exit(EXIT_SUCCESS);
}


int init_gamepad(){
  device = fopen("/dev/gamepad", "rb");
  if (!device) {
    printf("[GAMEPAD] ERROR: Couldn't open gamepad device...\n"); 
  return EXIT_FAILURE;
  }
  if (signal(SIGIO, &button_handler) == SIG_ERR)
  {
    printf("[GPIO] ERROR: Signal handler error...\n");
    return EXIT_FAILURE; 
  }
  if (fcntl(fileno(device), F_SETOWN, getpid()) == -1)
  {
    return EXIT_FAILURE;
  }
  long oflags = fcntl(fileno(device), F_GETFL);
  if (fcntl(fileno(device), F_SETFL, oflags | FASYNC) == -1)
  {
    printf("[FLAG] ERROR: Couldn't set FASYNC flag...\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}


 bool toggle(bool in){
 if(in){
 return false;
 }else{
 return true;
 }
 }

void button_handler(int signo)
{
	printf("button_handler");
      printf("player = %d\n", last_player);
    
    int button = fgetc(device);
    
    //printf("input %d\n", input);
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
    //delayMilliSeconds(100);
  }
}
