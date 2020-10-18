#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"



/*
	name: main
	purpose: program the microcontroller start running. Sets up nessesary settings, and sleeps if it is not interrupted by interrupt signals.
	argument(s): none
	return value: none
*/
int main(void)
{
	int wavevalue = 0;
	int *wave = &wavevalue;
	*wave = 2;


	//setup configurations
	//disableRam (); TODO - sjekk om denne funker!
	setup_GPIO();		//seupGPIO
	//setup_interruptGPIO();	//setupNVIC
	//setup_interrupt();	//setupNVIC
	setup_NVIC();

	setup_DAC();
	setup_timer(SAMPLE_PERIOD);

	setup_sleep();

	startDAC();
	startUpSong(*wave);
	//stopDAC();
	*GPIO_PA_DOUT = 0xEEff;
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
	//set external interrupt mode on pin 0 to 7 in port B.
	*GPIO_EXTIPSELL = 0x22222222;

	//enable falling edge trigger on pin 0 to 15
	*GPIO_EXTIFALL = 0xff;

	//enable rising edge trigger on pin 0 to 15     
	*GPIO_EXTIRISE = 0xff;

	//enable interrupt on pin 0 to 15
	*GPIO_IEN = 0xff;

}

//Kan nå erstattes av de to setup-funksjonene over TODO!!
void setup_NVIC()
{
	*GPIO_EXTIPSELL = 0x22222222;
	*GPIO_EXTIFALL = 0xff;
	*GPIO_EXTIRISE = 0xff;
	*GPIO_IEN = 0xff;

	*ISER0 = 0x802;
}

 /*
    name: setup_sleep
    purpose: configure registers so low power mode is SLEEPDEEP, and when done handling interrupt it is set in low power mode
    argument(s): none
    return value: none
  */
void setup_sleep()
{
	//enable SLEEPONEXIT TODO SJEKK OM DISSE FUNKER!
	//*SCR |= (1 << 1);

	//enable SLEEPDEEP
	//*SCR |= (1 << 2);
	
	*SCR = 6;
}

 /*
    name: disableRam
    purpose: configure registers to disable RAM blocks
    argument(s): none
    return value: none
  */
void disableRam()
{
	//power down RAM blocks 1-3
	*EMU_MEMCTRL = 7;
}

/*
 * if other interrupt handlers are needed, use the following names:
 * NMI_Handler HardFault_Handler MemManage_Handler BusFault_Handler
 * UsageFault_Handler Reserved7_Handler Reserved8_Handler
 * Reserved9_Handler Reserved10_Handler SVC_Handler DebugMon_Handler
 * Reserved13_Handler PendSV_Handler SysTick_Handler DMA_IRQHandler
 * GPIO_EVEN_IRQHandler TIMER0_IRQHandler USART0_RX_IRQHandler
 * USART0_TX_IRQHandler USB_IRQHandler ACMP0_IRQHandler ADC0_IRQHandler
 * DAC0_IRQHandler I2C0_IRQHandler I2C1_IRQHandler GPIO_ODD_IRQHandler
 * TIMER1_IRQHandler TIMER2_IRQHandler TIMER3_IRQHandler
 * USART1_RX_IRQHandler USART1_TX_IRQHandler LESENSE_IRQHandler
 * USART2_RX_IRQHandler USART2_TX_IRQHandler UART0_RX_IRQHandler
 * UART0_TX_IRQHandler UART1_RX_IRQHandler UART1_TX_IRQHandler
 * LEUART0_IRQHandler LEUART1_IRQHandler LETIMER0_IRQHandler
 * PCNT0_IRQHandler PCNT1_IRQHandler PCNT2_IRQHandler RTC_IRQHandler
 * BURTC_IRQHandler CMU_IRQHandler VCMP_IRQHandler LCD_IRQHandler
 * MSC_IRQHandler AES_IRQHandler EBI_IRQHandler EMU_IRQHandler 
 */
