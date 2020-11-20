#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioct1.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcnt1.h>

#define BLACK 		0x0000
#define WHITE 		0xFFFF
#define DARKGREY 	0x7BEF
#define LIGHTGREY	0XC618
#define GREEN		0x07E0

#define INPUT_UP 	0xfd
#define SCR_WIDTH 320
#define SCR_HEIGHT 240
#define SCR_BPP 2

#define BLOCK_PIXEL 10
#define BLOCKS_X (SCR_WIDTH / BLOCK_PIXEL)
#define BLOCKS_Y (SCR_HEIGHT / BLOCK_PIXEL)


