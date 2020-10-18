#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"

//------------ interrupt_handlers -------------//
//Here we have functions that handles different kind of interrupts 

/*
	name: TIMER1_IRQHandler
	purpose: handles interrupt provoked by the timer
	argument(s): none
	return value: none
*/

void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	//clear interrupt
	*TIMER1_IFC = 1;

}

/*
	name: GPIO_EVEN_IRQHandler
	purpose: handles interrupt provoked by the even GPIO pins, and run a functionality accorrding to which pin that is pressed.
	argument(s): none
	return value: none
 */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	int buttonStatus = *GPIO_PC_DIN;
	
	//clear interrupt
	*GPIO_IFC = *GPIO_IF;
	
	setup_DAC();
	startDAC();
	buttonPressed(buttonStatus);
	stopDAC();
}

/*
	name: GPIO_EVEN_IRQHandler
	purpose: handles interrupt provoked by the odd GPIO pins, and run a functionality accorrding to which pin that is pressed.
	argument(s): none
	return value: none
 */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	int buttonStatus = *GPIO_PC_DIN;
	
	//clear interrupt
	*GPIO_IFC = *GPIO_IF;
	
	setup_DAC();
	startDAC();
	buttonPressed(buttonStatus);
	stopDAC();
}
