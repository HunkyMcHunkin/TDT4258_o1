#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"

//------------ ex2 -------------//
//Here we have the main function the interrupt solution uses


/*
	name: main
	purpose: program the microcontroller start running. Sets up nessesary settings, and sleeps if it is not interrupted by interrupt signals.
	argument(s): none
	return value: none
*/
int main(void)
{


	//setup configurations
	setup_GPIO();
	
	//setup NVIC
	setup_interruptGPIO();
	setup_interrupt();

	setup_DAC();
	setup_timer(SAMPLE_PERIOD);
	setup_sleep();

	startDAC();
	startUpSong();	
	stopDAC();
	
	//sleeps while waiting for interrupts
	while (1) {
		__asm("WFI");
	}
}

 /*
    name: setup_interrupt
    purpose: enable interrupt
    argument(s): none
    return value: none
  */

void setup_interrupt()
{

	//enable interrupt
	*ISER0 = 0x802;
}

 /*
    name: setup_GPIOinterrupt
    purpose: enable GPIO interrupt, and set interrupt detection on rising and falling edge
    argument(s): none
    return value: none
  */
void setup_interruptGPIO()
{
	//set external interrupt mode on pin 0 to 7 in port C.
	*GPIO_EXTIPSELL = 0x22222222;

	//enable falling edge trigger on pin 0 to 15
	*GPIO_EXTIFALL = 0xff;

	//enable rising edge trigger on pin 0 to 15     
	*GPIO_EXTIRISE = 0xff;

	//enable interrupt on pin 0 to 15
	*GPIO_IEN = 0xff;

}


/*
    name: setup_sleep
    purpose: configure registers so low power mode is SLEEPDEEP, and when not handling interrupt it is set in low power mode
    argument(s): none
    return value: none
*/
void setup_sleep()
{
	//enable SLEEPONEXIT and SLEEPDEEP
	*SCR = 6;
}
