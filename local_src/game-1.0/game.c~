#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

//sett 


struct fb_copyarea rect;  //draw on screen
uint16_t *pixels;
int button;
FILE* device;


int init_gamepad()
{
    device = fopen("/dev/gamepad_group_2", "rb");
    if (device==NULL) {
        printf("Unable to open driver device\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


int main(int argc, char *argv[])
{
	printf("Hello World try, I'm game!\n");
	init_gamepad();
	//åpner driveren vår bare lesing
	//int gamepad_fd = open("/dev/gamepad_group_2", O_RDONLY);
	/*if (gamepad_fd == 1){
		printf("open driver success");
	}
	if (gamepad_fd == NULL){
		printf("open driver failed");
	}*/
	printf("open done\n");
	//int buttonspressed = (int)fgetc(device);  //henter knapptilstander
	printf("button compleat\n");
	
	
	//set up frame buffer-optaderer hele skjermen når ioctl blir kaldt
	int fbfd = open("/dev/fb0", 2);
	
	//forteller hvor stor del av skjermen vi skal oppdatere -> har valgt å oppdatare hele skjermen
	rect.dx = 0;
	rect.dy = 0;
	rect.width = 320;
	rect.height = 240;
	
	//uint8_t buttons = (uint8_t)getc(gamepad_fd);  //henter knapptilstander
	
	while(1){
	//printf(buttonspressed);
	
	//grafik
	 //320*240*(16/8) //antall pixsler ganger antall bytes per pixel
	pixels = mmap(NULL, 320*240*2, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0); //array med pixels
	//
	int i;
	for (i=0; i<638; i++){
		pixels[i] = 0xffff; //tegner en hvit strek øverst
	}
	for (i=640; i<1278; i++){
		pixels[i] = 0x000f; //tegner en blå strek linje 2
	}
	for (i=1280; i<10000; i++){
		pixels[i] = 0x0ff0;
	}
/*	if(buttonspressed == 0xFD){
		for (i=1500; i<10000; i++){
			pixels[i] = 0xffff;
		}
	}
*/	
	ioctl(fbfd, 0x4680, &rect);
	
	}
	
	return 0;
}
	
