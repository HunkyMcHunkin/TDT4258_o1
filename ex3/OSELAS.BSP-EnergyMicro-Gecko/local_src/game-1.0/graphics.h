#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include "font.h"

#define Black		0x0000
#define White 		0xFFFF
#define LightGrey       0xC618
#define Red 		0xF000
#define Blue 		0X000F


int init_framebuffer();
void deinit_framebuffer();
void refresh_fb();
void draw_grid();
void fill_block(int maxR, int minR, int maxC, int minC, int player);
int get_row(int pixel);
int get_col(int pixel, int row);
