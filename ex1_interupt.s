        .syntax unified
	
	      .include "efm32gg.s"

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
        mov r4, #2
        str r4, [r1, #GPIO_CTRL]

        // Set pins 8-15 to output
        ldr r3, = 0x55555555
        str r3, [r1, #GPIO_MODEH]

        //BUTTONS

    // Set pins 0-7 to input
        ldr r3, = 0x33333333
        str r3, [r2, #GPIO_MODEL]

        // Enable internal pull-up
        ldr r3, = 0xff
        str r3, [r2, #GPIO_DOUT]
    
    // Load the output for PA and input for PC into registers
        ldr r3, [r1, #GPIO_DOUT]
        ldr r4, [r2, #GPIO_DIN]



	// Interrupt
	ldr r5, = GPIO_BASE

	// 
	ldr r6, = 0x22222222
	str r6, [r5, #GPIO_EXTIPSELL]

	// Set interrupt on 1->0 transition, set interrupt on 0->1 transition and enable interrupt generation
	ldr r6, = 0xff
	str r6, [r5, #GPIO_EXTIFALL]
	str r6, [r5, #GPIO_EXTIRISE]
	str r6, [r5, #GPIO_IEN]

	ldr r6, = 0x802
	ldr r7, = ISER0
	str r6, [r7]

GPIO_CLOCK_ENABLE:
		    // Enable GPIO clock
	ldr r1, = CMU_BASE
	ldr r2, [r1, #CMU_HFPERCLKEN0]
	mov r3, #1
	lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
	orr r2, r3, r2
	str r2, [r1, #CMU_HFPERCLKEN0]
	mov r15, r14

	b .  // do nothing

	/////////////////////////////////////////////////////////////////////////////
	//
	// GPIO handler
	// The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////

	.thumb_func
	gpio_handler:
	ldr r5, = GPIO_BASE

	// Determine the source of the interrupt
	ldr r6, [r5, #GPIO_IF]

	// Clear the interrupt
	str r6, [r5, #GPIO_IFC]

	ldr r1, = GPIO_PA_BASE
	ldr r2, = GPIO_PC_BASE
	ldr r3, [r1, #GPIO_DOUT]
	ldr r4, [r2, #GPIO_DIN]

	ldr r5, [r4]
	lsl r5, r5, #8
	str r5, [r3]


	b .  // do nothing

	/////////////////////////////////////////////////////////////////////////////

	.thumb_func
	dummy_handler:
	b .  // do nothing











