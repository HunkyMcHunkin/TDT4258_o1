        .syntax unified
	
	      .include "efm32gg.s"

   /////////////////////////////////////////////////////////////////////////////
	//
  // Constants
  // Here we define constants
	//
	/////////////////////////////////////////////////////////////////////////////
	 
    .section .constants

    OUTPUT_MODE = 0x55555555
    INPUT_MODE = 0x33333333
    DRIVE_STRENGTH_LOW = 0x3
    INTERNAL_PULLUP = 0xff


	/////////////////////////////////////////////////////////////////////////////
	//
  // Exception vector table
  // This table contains addresses for all exception handlers
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .section .vectors
	
	      .long   stack_top               /* Top of Stack                 */
	      .long   _reset                  /* Reset Handler                */
	      .long   dummy_handler           /* NMI Handler                  */
	      .long   dummy_handler           /* Hard Fault Handler           */
	      .long   dummy_handler           /* MPU Fault Handler            */
	      .long   dummy_handler           /* Bus Fault Handler            */
	      .long   dummy_handler           /* Usage Fault Handler          */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* SVCall Handler               */
	      .long   dummy_handler           /* Debug Monitor Handler        */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* PendSV Handler               */
	      .long   dummy_handler           /* SysTick Handler              */

	      /* External Interrupts */
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO even handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO odd handler */
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
        bl GPIO_CLOCK_ENABLE

        // Load the base adresses for PA and PC into registers
        ldr r1, = GPIO_PA_BASE
        ldr r2, = GPIO_PC_BASE

        //LED
        // Set high drive strength
        mov r4, #DRIVE_STRENGTH_LOW
        str r4, [r1, #GPIO_CTRL]

        // Set pins 8-15 to output
        mov r3, #OUTPUT_MODE
        str r3, [r1, #GPIO_MODEH]
        
        //BUTTONS
	// Set pins 0-7 to input
        mov r3, #INPUT_MODE
        str r3, [r2, #GPIO_MODEL]

        // Enable internal pull-up
        mov r3 #INTERNAL_PULLUP
        str r3, [r2, #GPIO_DOUT]
	
	// Polling: Load value from input port to register, shift it into right position and store the value in output port 
		Loop:
			ldr r5, [r2, #GPIO_DIN]
			lsl r5, r5, #8
        	str r5, [r1, #GPIO_DOUT]
     		b Loop

	      b .  // do nothing
	
	/////////////////////////////////////////////////////////////////////////////
	//
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
GPIO_CLOCK_ENABLE:
        // Set up clock
        ldr r1, = CMU_BASE
        ldr r2, [r1, #CMU_HFPERCLKEN0]
        mov r3, #1
        lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
        orr r2, r2, r3
        str r2, [r1, #CMU_HFPERCLKEN0]
        mov r15, r14

        .thumb_func
gpio_handler:  

	      b .  // do nothing
	
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
dummy_handler:  
        b .  // do nothing
