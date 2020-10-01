#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"


#define SAMPLE_PERIOD 318


void setupTimer(uint32_t period);
void setupDAC();
void setupGPIO();
void setupNVIC();
void MakeSound(int freq, int length);
void Sleep();


int main(void)
{
	/* interrupt løsning går inn i sleep mode med lavere strøm enn vanlig(ca 1.4uA). trykk på knapp fungerer, holde inne knapp fungerer ikke, avbryte sang fungerer ikke*/
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);
	setupNVIC();
	Sleep();

	while (1){
		__asm("WFI");
	}
}

void setupNVIC()
{
	*GPIO_EXTIPSELL = 0x22222222;
	*GPIO_EXTIFALL = 0xff;
	*GPIO_EXTIRISE = 0xff;
	*GPIO_IEN = 0xff;
	
	*ISER0 = 0x802;
}

void Sleep()
{
	*SCR = 6; 
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
