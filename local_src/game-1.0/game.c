#include <stdio.h>
#include <stdlib.h>



struct fb_copyarea rect;  //draw on screen
uint16_t *pixels;

int main(int argc, char *argv[])
{
	printf("Hello World try, I'm game!\n");
	
	//set up frame buffer-optaderer hele skjermen når ioctl blir kaldt
	int fbfd = fopen("/dev/fb0", 2);
	
	rect.dx = 0;
	rect.dy = 0;
	rect.width = 320;
	rect.height = 240;
	
	
	 //320*240*(16/8) //antall pixsler ganger antall bytes per pixel
	pixels = mmap(NULL, 320*240*2, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0); //array med pixels
	//
	for (int i=0; i<500; i++){
		pixels(i) = 0xffff; //tegner en hvit strek
	}
	ioctl(fbfd, 0x4680, &rect);
	
	return 0;
}
	

	exit(EXIT_SUCCESS);
}
