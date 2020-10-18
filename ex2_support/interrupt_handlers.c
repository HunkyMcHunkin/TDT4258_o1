#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"

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
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler(int *wave)
{
	int buttonIn = *GPIO_PC_DIN;
	//clear interrupt
	*GPIO_IFC = *GPIO_IF;
	//prepering DAC in case a song will be played
	setup_DAC();
	startDAC();
	//run functionality specified by pushed button
	*GPIO_PA_DOUT = 0xAAff;
	delay_C(100);
	
	buttonPressed(buttonIn, &wave);
	//prepering to go back to sleep
	stopDAC();
	//turnOffLEDs();
}

/*
	name: GPIO_EVEN_IRQHandler
	purpose: handles interrupt provoked by the odd GPIO pins, and run a functionality accorrding to which pin that is pressed.
	argument(s): none
	return value: none
 */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler(int *wave)
{
	int buttonIn = *GPIO_PC_DIN;
	//clrear interrupt
	*GPIO_IFC = *GPIO_IF;

	//prepering DAC in case a song will be played
	setup_DAC();
	startDAC();

	*GPIO_PA_DOUT = 0xAAff;
	delay_C(100);
	//run functionality specified by pushed button
	buttonPressed(buttonIn, &wave);

	//prepering to go back to sleep
	stopDAC();
	//turnOffLEDs();
}
