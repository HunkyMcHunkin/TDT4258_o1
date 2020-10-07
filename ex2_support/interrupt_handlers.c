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
purpose: handles interrupt provoked by the even GPIO pins, and run a functionality accorrding to which pin that is/was pressed.
argument(s): none
return value: none
 */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler(int *wave)
{
	//clear interrupt
	*GPIO_IFC = *GPIO_IF;
	
	//run functionality specified by pushed button
	startDAC(); //*** set up eller start?
	buttonPressed(*GPIO_PC_DIN, wave); //*** Navn
	stopDAC();
}


/*
name: GPIO_EVEN_IRQHandler
purpose: handles interrupt provoked by the odd GPIO pins, and run a functionality accorrding to which pin that is/was pressed.
argument(s): none
return value: none
 */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler(int *wave)
{
	//clrear interrupt
	*GPIO_IFC = *GPIO_IF;
	
	//run functionality specified by pushed button
	startDAC(); //*** set up eller start?
	buttonPressed(*GPIO_PC_DIN, wave);
	stopDAC();
}
