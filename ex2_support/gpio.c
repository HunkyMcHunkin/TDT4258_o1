#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"

//------------ GPIO -------------//
//Here we have functions related to the GPIO pins. This includes setup functions for GPIO pins, and set and clear functions for LEDs.

 /*
    name: setup_GPIO
    purpose: sets up GPIO mode and interrupts by configure registers. 
    argument(s): none
    return value: none
  */
void setup_GPIO()
{
	//enable high frequency peripheral clock for the timer
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;

	//set lowest drive strength
	*GPIO_PA_CTRL = 1;

	//set pins A8-15 as output
	*GPIO_PA_MODEH = 0x55555555;

	//turn all LEDs off
	*GPIO_PA_DOUT = 0xffff;

	//set buttons as output
	*GPIO_PC_MODEL = 0x33333333;

	//enable internal pull up
	*GPIO_PC_DOUT = 0xff;
}

/*
	name: setLEDs_waveFormat
	purpose: Lighting LEDs to indicate which wave format is being used.
	argument(s):
		waveFormat:
			range: SQUARE, SAWTOOTH, TRIANGLE, SINUS
			purpose: define which wave format is being used
	return value: none
*/

void setLEDs_waveFormat(int waveFormat)
{
	// choosing which LEDs to light
	switch (waveFormat) {
	case 3:
		*GPIO_PA_DOUT = (0x00ff);
		break;
	case 2:
		*GPIO_PA_DOUT = (0x03ff);
		break;
	case 1:
		*GPIO_PA_DOUT = (0x0fff);
		break;
	case 0:
		*GPIO_PA_DOUT = (0x3fff);
		break;
	case 4:
		*GPIO_PA_DOUT = (0x3fff);
		break;
	}
}

/*
	name: turnOffLEDs
	purpose: Turn off all the LEDs.
	argument(s): none
	return value: none
*/
void turnOffLEDs()
{
	*GPIO_PA_DOUT = 0xffff;
}
