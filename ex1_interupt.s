          	.syntax unified
    
      	.include "efm32gg.s"

    /////////////////////////////////////////////////////////////////////////////
    //
  // Exception vector table
  // This table contains addresses for all exception handlers
    //
    /////////////////////////////////////////////////////////////////////////////
    
    	.section .vectors
    
      	.long   stack_top           	/* Top of Stack             	*/
      	.long   _reset              	/* Reset Handler            	*/
      	.long   dummy_handler       	/* NMI Handler              	*/
      	.long   dummy_handler       	/* Hard Fault Handler       	*/
      	.long   dummy_handler       	/* MPU Fault Handler        	*/
      	.long   dummy_handler       	/* Bus Fault Handler        	*/
      	.long   dummy_handler       	/* Usage Fault Handler      	*/
      	.long   dummy_handler       	/* Reserved                 	*/
      	.long   dummy_handler       	/* Reserved                 	*/
      	.long   dummy_handler       	/* Reserved                 	*/
      	.long   dummy_handler       	/* Reserved                 	*/
      	.long   dummy_handler       	/* SVCall Handler           	*/
      	.long   dummy_handler       	/* Debug Monitor Handler    	*/
      	.long   dummy_handler       	/* Reserved                 	*/
      	.long   dummy_handler       	/* PendSV Handler           	*/
      	.long   dummy_handler       	/* SysTick Handler          	*/

      	/* External Interrupts */
      	.long   dummy_handler
      	.long   gpio_handler        	/* GPIO even handler */
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   gpio_handler        	/* GPIO odd handler */
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler
      	.long   dummy_handler

      	.section .text

    /////////////////////////////////////////////////////////////////////////////
    //
    // Reset handler
  // The CPU will start executing here after a reset
    //
    /////////////////////////////////////////////////////////////////////////////

      	.globl  _reset
      	.type   _reset, %function
    	.thumb_func
_reset:
   	 bl GPIO_clock_enable
   	 
   	 ldr r0, = GPIO_BASE             	/* Load the base adress for GPIO into register */
    	ldr r1, = GPIO_PA_BASE          	/* Load the base adresses for PA and PC into registers */
    	ldr r2, = GPIO_PC_BASE
   	 
   	 bl LED_set_up
   	 bl button_set_up
   	 bl enable_interrupt
   	 bl enable_sleep
   	 //bl power_down_RAM_blocks
   	 
   	 mov r5,  #0b0000000001111111
   	 lsl r5, r5, #8
    	str r5, [r1, #GPIO_DOUT]
   	 
   	 loop:
   		 wfi
   	 b loop                       	/* Go to sleep */
   	 
    	b .  // do nothing


GPIO_clock_enable:
    	ldr r1, = CMU_BASE                     	/* Load CMU base address */
    	ldr r2, [r1, #CMU_HFPERCLKEN0]         	/* Load value of CMU_HFPERCLKEN0 */
    	mov r3, #1                             	/* Set bit for GPIO clk */
    	lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
    	orr r2, r2, r3
    	str r2, [r1, #CMU_HFPERCLKEN0]         	/* Store new value of CMU_HFPERCLKEN0 */
    	mov r15, r14                           	/* Move the value of lr into pc */

LED_set_up:
    	mov r4, #0x3                          	/* Set drive strength high(20mA) when 2, low when 3 (2mA) */
    	str r4, [r1, #GPIO_CTRL]
    	ldr r3, = 0x55555555                   	/* Set pins 8-15 to output */
    	str r3, [r1, #GPIO_MODEH]
    	mov r15, r14                           	/* Move the value of lr into pc */

button_set_up:
    	ldr r3, = 0x33333333                   	/* Set pin 0-7 to input */
    	str r3, [r2, #GPIO_MODEL]
    	ldr r3, = 0xff                         	/* Enable internal pull-up */
    	str r3, [r2, #GPIO_DOUT]
    	mov r15, r14                           	/* Move the value of lr into pc */
    
enable_interrupt:
   	 ldr r6, = 0x22222222                   	/* Set interrupt for port C */
   	 str r6, [r0, #GPIO_EXTIPSELL]
   	 ldr r6, = 0xff
   	 str r6, [r0, #GPIO_EXTIFALL]           	/* Set interrupt on 1->0 transition */
   	 str r6, [r0, #GPIO_EXTIRISE]           	/* Set interrupt on 0->1 transition */
   	 str r6, [r0, #GPIO_IEN]                	/* Enable interrupt generation */
   	 
   	 ldr r6, = 0x802                        	/* Enable interrupt handler */
   	 ldr r7, = ISER0
   	 str r6, [r7]
    	mov r15, r14                           	/* Move the value of lr into pc */
    
enable_sleep:
   	 mov r10, #6                        	/* Enable SLEEPDEEP and SLEEPONEXIT */
   	 ldr r11, = SCR
   	 str r10, [r11]
    	mov r15, r14                           	/* Move the value of lr into pc */
   	 
power_down_RAM_blocks:
   	 ldr r9, = EMU_BASE
   	 mov r10, #7
   	 str r10, [r9, #EMU_MEMCTRL]
	     	mov r15, r14    


    /////////////////////////////////////////////////////////////////////////////
    //
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
    //
    /////////////////////////////////////////////////////////////////////////////

    	.thumb_func
gpio_handler:  
   	 
   	 // Determine the source of the interrupt
   	 ldr r6, [r0, #GPIO_IF]

   	 // Clear the interrupt
   	 str r6, [r0, #GPIO_IFC]
   	 //ldr r7, [r2, #GPIO_DIN]
   	 //lsl r7, r7, #8
   	 //str r7, [r1, #GPIO_DOUT]
   	 
   	 
   	 //define what happens when you press the specific buttons
   	 ldr r5, [r2, GPIO_DIN]
   	 cmp r5, #0b0000000011111110
   	 beq knapp1
   	 cmp r5, #0b0000000011111101
   	 beq knapp2
   	 cmp r5, #0b0000000011111011
   	 beq knapp3
   	 cmp r5, #0b0000000011110111
   	 beq knapp4
   	 cmp r5, #0b0000000011101111
   	 beq shift_left
   	 cmp r5, #0b0000000011011111
   	 beq blink
   	 cmp r5, #0b0000000010111111
   	 beq shift_right
   	 cmp r5, #0b0000000001111111
   	 beq knapp8
   	 bx r14
   	 

   	 b .  // do nothing
   	 
knapp1:
   	 ldr r5, [r1, #GPIO_DOUT]
   	 and r7, r5, #0b0000000100000000    	 //check the only bit we are intrested in
   	 cmp r7, #0b0000000000000000   			 //check if led is on
   	 beq turn_off
   	 and r5, r5, #0b1111111000000000   		 //turn led on
   	 str r5, [r1, #GPIO_DOUT]
   	 bx r14
knapp2:
   	 ldr r5, [r1, #GPIO_DOUT]
   	 and r7, r5, #0b0000001000000000
   	 cmp r7, #0b0000000000000000
   	 beq turn_off
   	 and r5, r5, #0b1111110100000000
   	 str r5, [r1, #GPIO_DOUT]
   	 bx r14
knapp3:
   	 ldr r5, [r1, #GPIO_DOUT]
   	 and r7, r5, #0b0000010000000000
   	 cmp r7, #0b0000000000000000
   	 beq turn_off
   	 and r5, r5, #0b1111101100000000
   	 str r5, [r1, #GPIO_DOUT]
   	 bx r14
knapp4:
   	 ldr r5, [r1, #GPIO_DOUT]
   	 and r7, r5, #0b0000100000000000
   	 cmp r7, #0b0000000000000000
   	 beq turn_off
   	 and r5, r5, #0b1111011100000000
   	 str r5, [r1, #GPIO_DOUT]
   	 bx r14
shift_right:
   	 ldr r5, [r1, #GPIO_DOUT]
   	 and r7, r5, #0b1000000000000000
   	 cmp r7, #0b0000000000000000   		 //check if end led is on
   	 beq shift_end_r
   	 lsl r5, r5, #1   					 //move the led one to the right
   	 orr r5, r5, #0b0000000100000000   	 //correcting added zero from lsl
   	 str r5, [r1, #GPIO_DOUT]
   	 bx r14
shift_left:
   	 ldr r5, [r1, #GPIO_DOUT]
   	 and r7, r5, #0b0000000100000000
   	 cmp r7, #0b0000000000000000   		 //check if end LED is on
   	 beq shift_end_l
   	 lsr r5, r5, #1   					 //move the led one to the left
   	 orr r5, r5, #0b1000000000000000   	 //correcting added zero from lsr
   	 str r5, [r1, #GPIO_DOUT]
   	 bx r14
shift_end_r:
   	 lsl r5, r5, #1
   	 str r5, [r1, #GPIO_DOUT]
   	 bx r14
shift_end_l:
   	 lsr r5, r5, #1
   	 str r5, [r1, #GPIO_DOUT]
   	 bx r14
turn_off:
   	 mov r5, #0b1111111100000000
   	 str r5, [r1, #GPIO_DOUT]
   	 bx r14
blink:
    mov r7, #0b0101010100000000   	 
    str r7, [r1, #GPIO_DOUT]
    push {r14}   						 //store value of program counter outside of blink
    bl wait
    mov r7, #0b1010101000000000
    str r7, [r1, #GPIO_DOUT]
    bl wait
    ldr r5, [r2, GPIO_DIN]
    cmp r5, #0b0000000011011111
	beq blink
	pop {r15}   						 //get the stored value of pc back
    bx r14
wait:
    mov r7, #1000
    mov r5, #1000
    mul r7, r7, r5   					 //define wait, need large number for human eye to see
wait_loop:
    subs r7, #1
    bne wait_loop   					 //count down to 0
    bx r14

    

   	 
    /////////////////////////////////////////////////////////////////////////////
    
    	.thumb_func
dummy_handler:  
b . // do nothing
