#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "graphics.h"

int init();
void deinit();
int init_gamepad();
void deinit_gamepad();
int check_win();
int map_buttons(int input);
void buttonpress_handler(int signal);
int main();

void delay(int seconds);
//void setup_timer(uint16_t period);
//void delayMicroSeconds(uint32_t microSeconds);
//void delayMilliSeconds(uint32_t milliSeconds);

