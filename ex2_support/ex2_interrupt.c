#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"

 /*
name: main
purpose: program the microcontroller runs. Sets up nessesary settings, sleeps if it is not interrupted by interrupt signals.
argument(s): none
return value: none
*/
int main2(void)
{
	int *wave;
	*wave = 0:
	//setup configurations
	setup_disableRam();
	setup_GPIO();
	setup_DAC();
	setup_timer(SAMPLE_PERIOD);
	setup_NVIC();
	setup_sleep();
	
	startUpMelody(*wave); //TODO

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
void setup_NVIC()
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
	//set external interrupt mode on pin 0 to 7 in port B.
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
purpose: configure registers so low power mode is SLEEPDEEP, and when done handeling interrupt it is set in low power mode
argument(s): none
return value: none
*/
void setup_sleep()
{
	//enable SLEEPONEXIT 
	*SCR |= (1 << 1);
	
	//enable SLEEPDEEP
	*SCR |= (1 << 2);
}

 /*
name: setup_disableRam
purpose: configure registers to disable RAM blocks
argument(s): none
return value: none
*/
void setup_disableRam()
{
	//power down RAM blocks 1-3
	*EMU_MEMCTRL = 7;
}
