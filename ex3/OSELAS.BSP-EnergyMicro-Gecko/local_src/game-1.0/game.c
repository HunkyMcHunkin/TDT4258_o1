#include "game.h"
#include <time.h>

FILE* device;
bool running;

uint8_t last_input;
bool last_player;


void new_game(){
  
}

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

int init(){
  if (init_gamepad() == EXIT_FAILURE)
  {
    printf("[GAMEPAD] ERROR: Couldn't initialize gamepad...\n");
    return EXIT_FAILURE;
  }
  if (init_framebuffer() == EXIT_FAILURE)
  {
    printf("[FRAMEBUFFER] ERROR: Couldn't initialize framebuffer...\n");
    return EXIT_FAILURE;
  }
  new_game();
  return EXIT_SUCCESS;
}

void deinit(){
  deinit_gamepad();
  deinit_framebuffer();
}

int init_gamepad(){
  device = fopen("/dev/gamepad", "rb");
  if (!device) {
    printf("[GAMEPAD] ERROR: Couldn't open gamepad device...\n"); 
  return EXIT_FAILURE;
  }/*
  if (signal(SIGIO, &buttonpress_handler) == SIG_ERR)
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
  }*/
  return EXIT_SUCCESS;
}

void deinit_gamepad(){
  fclose(device);
}

int check_win(){/*
  if (square[1] == square[2] && square[2] == square[3])
  {
    return 1;
  }else if (square[4] == square[5] && square[5] == square[6])
  {
    return 1;
  }else if (square[7] == square[8] && square[8] == square[9])
  {
    return 1;
  }else if (square[1] == square[4] && square[4] == square[7])
  {
    return 1;
  }else if (square[2] == square[5] && square[5] == square[8])
  {
    return 1;
  }else if (square[3] == square[6] && square[6] == square[9])
  {
    return 1;
  }else if (square[1] == square[5] && square[5] == square[9])
  {
    return 1;
  }else if (square[3] == square[5] && square[5] == square[7])
  {
    return 1;
  }else if (square[1] != '1' && square[2] != '2' && square[3] != '3' && square[4] != '4' && square[5] != '5' && square[6] != '6' && square[7] != '7' && square[8] != '8' && square[9] != '9')
  {
    return 0;
  }else
  {
    return -1;
  }

  */
  return 0;
}

/*

		|		|	
	11	|	12	|	13
		|		|	
		|		|
--------------------------------------------------
		|		|
	21	|	22	|	23
		|		|
		|		|
--------------------------------------------------
		|		|		
	31	|	32	|	33	
		|		|	
		|		|
--------------------------------------------------


*/

void buttonpress_handler(int signal){
  int input = map_buttons((int)getc(device));
  switch (input)
  {
    case 1:
      //11
      if (last_input == 1) 
      {
        fill_block(75,0,75,0,last_player);
        last_input = 0;
        last_player = ~last_player;
        break;
      }
      //21
      else if (last_input == 2)
      {
        fill_block(155,80,75,0,last_player);
        last_input = 0;
        last_player = ~last_player;
        break;
      }
      //31
      else if (last_input == 3)
      {
        fill_block(240,160,75,0,last_player);
        last_input = 0;
        last_player = ~last_player;
        break;
      }
      last_input = input;      
      break;
    case 2:
      //12
      if (last_input == 1)
      {
        fill_block(75,0,155,80,last_player);
        last_input = 0;
        last_player = ~last_player; 
        break;
      }
      //22
      else if (last_input == 2)
      {
        fill_block(155,80,155,80,last_player);
        last_input = 0;
        last_player = ~last_player;
        break;
      }
      //32
      else if (last_input == 3)
      {
        fill_block(240,160,155,80,last_player);
        last_input = 0;
        last_player = ~last_player;
        break;
      }
      last_input = input;
      break;
    case 3:
      //13
      if (last_input == 1)
      {
        fill_block(75,0,235,160,last_player);
        last_input = 0;
        break;
      }
      //23
      else if (last_input == 2)
      {
        fill_block(155,80,235,160,last_player);
        last_input = 0;
        last_player = ~last_player;
        break;
      }
      //33
      else if (last_input == 3)
      {
        fill_block(240,160,235,160,last_player);
        last_input = 0;
        last_player = ~last_player;
        break;
      }
      last_input = input;
      break;
      delay(1);
      //delayMilliSeconds(100);
  }
}

int map_buttons(int input){
  switch (input)
  {
    case 0xFD:
      return 1;
    case 0xF7:
      return 2;
    case 0xDF:
      return 3;
  }
  return 0;
}

int main()	{

  //setup_timer(SAMPLE_PERIOD);
  
  if (init() == EXIT_FAILURE)
  {
    printf("[INIT] ERROR: Couldn't init...\n");
    return EXIT_FAILURE;
  }
  
  last_input = 0;
  last_player = true;
  running = true;
  
  while (running)
  {
    if (check_win())
    {
      printf("Finito\n");
      running = false;
    }
    refresh_fb();
    pause();
  }
  deinit();
  return EXIT_SUCCESS;
}
