#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"


void MakeSound(int freq, int length);
void MakeSong(int *frecVec, int lengthFrecVec, int lengthnote);
void MakeSoundT(int freq, int length);
void MakeSongT(int *frecVec, int lengthFrecVec, int lengthnote);
void MakeSoundS(int freq, int length);
void MakeSongS(int *frecVec, int lengthFrecVec, int lengthnote);
void stopDAC();
/*
 * function to set up GPIO mode and interrupts
 */
void setupGPIO()
{

	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;	/* enable GPIO clock */
	*GPIO_PA_CTRL = 3;	/* set low drive strength */
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
		MakeSoundT(30, 100);
		MakeSoundT(60, 100);
		MakeSoundT(100, 100);
		MakeSoundT(500, 100);
		MakeSoundT(200, 100);
		MakeSoundT(50, 100);
	} else if (buttonX==0xfb) {
		MakeSound(200, 100);
	} else if (buttonX==0xf7) {
		int song[] = {165, 175, 196, 220, 267, 262, 294, 330, 349, 392, 440, 440, 392, 349, 330, 294, 262, 267, 220, 196, 175, 165, 146};
		MakeSong(song, 22, 100);
	} else if (buttonX==0xef) {
		int song[] = {165, 175, 196, 220, 267, 262, 294, 330, 349, 392, 440, 440, 392, 349, 330, 294, 262, 267, 220, 196, 175, 165, 146};
		MakeSongS(song, 22, 100);
	} else if (buttonX==0xdf) {
		MakeSoundS(200, 100);
	} else if (buttonX==0xbf) {
		MakeSoundT(200, 100);
	} else if (buttonX==0x7f) {
		int song[] = {165, 175, 196, 220, 267, 262, 294, 330, 349, 392, 440, 440, 392, 349, 330, 294, 262, 267, 220, 196, 175, 165, 146};
		MakeSongT(song, 22, 100);
	}
	stopDAC();
}
