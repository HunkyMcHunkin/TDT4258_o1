#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"


void MakeSound(int freq, int length);
void MakeSong(int *frecVec, int lengthFrecVec, int lengthnote);
/*
 * function to set up GPIO mode and interrupts
 */
void setupGPIO()
{

	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;	/* enable GPIO clock */
	*GPIO_PA_CTRL = 2;	/* set high drive strength */
	*GPIO_PA_MODEH = 0x55555555;	/* set pins A8-15 as output */
	*GPIO_PA_DOUT = 0xffff;	     /* turn all LEDs off*/
	
	*GPIO_PC_MODEL = 0x33333333; /*set buttons as output*/
	*GPIO_PC_DOUT = 0xff; /*enable internal pull up*/
}


void buttonPressed(int buttonX)
{
	if (buttonX==0xfe) {
		MakeSound(30, 100);
		MakeSound(60, 100);
		MakeSound(100, 100);
		MakeSound(500, 100);
		MakeSound(200, 100);
		MakeSound(50, 100);
	} else if (buttonX==0xfd) {
		MakeSound(2000, 100);
		MakeSound(1500, 100);
		MakeSound(300, 100);
		MakeSound(70, 100);
		MakeSound(500, 100);
		MakeSound(1000, 100);
	} else if (buttonX==0xfb) {
		MakeSound(440, 100);
	} else if (buttonX==0xf7) {
		MakeSound(1500, 100);
	} else if (buttonX==0xef) {
		MakeSound(5000, 100);
	} else if (buttonX==0xdf) {
		MakeSound(10000, 100);
	} else if (buttonX==0xbf) {
		int song[] = {30, 60, 100, 500, 200, 50, 30, 60, 100};
		MakeSong(song, 8, 70);
	} else if (buttonX==0x7f) {
		MakeSound(15000, 100);
	}
}
