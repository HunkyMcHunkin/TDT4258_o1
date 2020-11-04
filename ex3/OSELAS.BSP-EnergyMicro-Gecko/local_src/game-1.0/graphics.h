#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 1
#define _GNU_SOURCE 1
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>

#define Black		0x0000
#define White 		0xFFFF
#define DarkGrey	0x7BEF
#define LightGrey	0xC618
#define Green		0x07E0
#define Red 		0xF800

#define BACKGROUND_COLOR DarkGrey

static uint16_t colors[17] = {
	Black,
	White,	DarkGrey,
	LightGrey,	Green
};

static char gameover[16] = {
	' ', ' ', ' ', ' ',
	'G', 'A', 'M', 'E',
	'O', 'V', 'E', 'R',
	' ', ' ', ' ', ' '
};

int init_framebuffer();
void deinit_framebuffer();
void refresh_fb();
void draw_tile(int pos, int val);
void draw_game_over();
void draw_high_score(int high_score);
void draw_score(int score);
void clear_score(void);
