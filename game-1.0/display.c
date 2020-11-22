#include "display.h"

struct fb_copyarea frame;
uint16_t *framebuffer;
int fbfd = 0;


#define Black		0x0000
#define White 		0xFFFF
#define LightGrey       0xC618
#define Red 		0xF000
#define Blue 0X000F


Color color(uint8_t r, uint8_t g, uint8_t b) {
	Color col;
	if(r >= 0 && r < 32 && g >= 0 && g < 64 && b >= 0 && b < 32){
		col.R = r;
		col.G = g;
		col.B = b;
	} else {
		col.R = 0;
		col.G = 0;
		col.B = 0;
		printf("Invalid color value supplied: (R: %d, G: %d, B: %d), max (31, 63, 31)", r, g, b);
	}
	return col;
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
  printf("player = %d\n", player);
  for (i = 0; i < 240*320; i++)
  {
  	int row = get_row(i);
  	int col = get_col(i, row);
    if (((col < maxC) && (col >= minC)) && ((row < maxR) && (row >= minR)))
    {
      if (player)
      {
        framebuffer[i] = Red;
      }
      else
      {
        framebuffer[i] = Blue;
      }
    }
  }
}


void init_display(){
    // Init display driver
	frame.dx = 0;
	frame.dy = 0;
	frame.width = SCREEN_WIDTH;
	frame.height = SCREEN_HEIGHT;
	fbfd = open("/dev/fb0", O_RDWR);
	if(fbfd == -1) {
		// Failed to open file descriptor
		printf("Error: unable to open framebuffer, exiting...\n)");
		exit(EXIT_FAILURE);
	}
	
	int screensize = SCREEN_WIDTH * SCREEN_HEIGHT * SCR_BPP; 	
	framebuffer = (uint16_t*) mmap(NULL, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if(framebuffer == MAP_FAILED) { // Error, return
		printf("Error: unable to map framebuffer, exiting...\n");
		close(fbfd);
		exit(EXIT_FAILURE);
	}
	
	int i;
	
	// Making the background white
	for (i = 0; i < 320*240; i++){
		framebuffer[i] = 0xffff;
	}
	
	//set up grid
	for (i=0; i<320*240; i++){
		int row = get_row(i);
		int col = get_col(i, row);
		if((col >= 75 && col < 80) || (col >= 155 && col < 160) || (col >= 235 && col < 240)) {
			framebuffer[i]=0x0000;
		}
		if((row >= 75 && row <80) || (row >= 155 && row <160)){
			framebuffer[i]=0x0000;
		}
		if(col>=240){
			framebuffer[i]= 0xffff;
		}
}
}

/*void set_screen_black(){
	memset(framebuffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT * SCR_BPP); // Fill screen with black
}*/


void flush_display_buffer() {
    ioctl(fbfd, 0x4680, &frame);
}
