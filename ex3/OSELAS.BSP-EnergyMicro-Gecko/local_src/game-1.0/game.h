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
int map_buttons(int)
void buttonpress_handler(int);
int main();

