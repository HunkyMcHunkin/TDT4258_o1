#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"


/*
 * function to set up GPIO mode and interrupts
 */
void setupGPIO()
{

	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;	/* enable GPIO clock */
	*GPIO_PA_CTRL = 1;	/* set lowest drive strength */
	*GPIO_PA_MODEH = 0x55555555;	/* set pins A8-15 as output */
	*GPIO_PA_DOUT = 0xffff;	     /* turn all LEDs off*/
	
	*GPIO_PC_MODEL = 0x33333333; /*set buttons as output*/
	*GPIO_PC_DOUT = 0xff; /*enable internal pull up*/
}


void buttonPressed(int buttonX)
{
	if (buttonX==0xfe) {  
		updatewave();
	} else if (buttonX==0xfd) {
		int song[] = {A, A, A, A, 1, 1, A, G, E, Cl, 1, Cl, A, A, G, F, G};   //pokemon
		ChooseWave(song, 17, 100);
	} else if (buttonX==0xfb) {
		int song[] = {E, E, C, C, D, D, C, 1, H, H, A, A, G, G, F};    //twinkle twinkle little star
		ChooseWave(song, 15, 200);
	} else if (buttonX==0xf7) {
		int song[] = {C, D, C, H, A, H, C, 1, G, A, H, 1, A, H, C, 1, C, D, C, H, A, H, C, 1, G, 1, C, 1, A, F};  //london bridge
		ChooseWave(song, 30, 70);
	} else if (buttonX==0xef) {
		int song[] = {D, F, 1, 1, F, E, 1, 1, G, A, H, A, G, F, 1, D, E, G, D, D, E, F, G, C, H, 1, G, A, 1, D, E, F, G, F, E};
		ChooseWave(song, 35, 150);
	} else if (buttonX==0xdf) {
		int song[] = {E, G, Dl, 1, E, G, Dl, 1, E, G, D, C, G};  //zelda
		ChooseWave(song, 13, 200);
	} else if (buttonX==0xbf) {
		int song[] = {E, G, Cl, 1, E, G, Cl, 1, F, A, C, A, A, G, 1, E, G, Cl, 1, E, G, Cl, 1, Dl, E, F, Hl, Dl, Cl, Cl};  //mikkel rev
		ChooseWave(song, 30, 150);
	} else if (buttonX==0x7f) {
		int song[] = {E, F, G, A, H, C, D, Eh, Fh, Gh, Ah, Ah, Gh, Fh, Eh, D, C, H, A, G, F, E};  //full step
		MakeSongT(song, 21, 100);
	}
}

