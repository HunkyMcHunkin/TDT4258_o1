/*
 * This is a demo Linux kernel module.
 */
/*
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>  //to allocate and freeing devise numbers and use file opperations
#include <linux/kdev_t.h>
#include <linux/cdev.h>  //make the kernel a char devise
#include <linux/device.h>
#include "efm32gg.h"
#include <linux/uaccess.h> //to use function copy_to_user -allows user access

#include <linux/fb.h>  //graphic

#include <linux/ioport.h> //allocation of io memory
#include <asm/system.h> // adds barriers
#include <linux/stat.h> //who can access the rep. of module param. in sysfs, define you own param.
#include <linux/errno.h> //for error handling, use goto
//#include <linus/sched.h>  //pointer to struct task_struct
#include <asm/current.h> //can access current process
#include <asm/io.h> //to access io ports
#include <linux/interrupt.h>
#include <asm/irq.h>

//#include <linux/iqr.h>
#include <asm/signal.h>

#include <linux/moduleparam.h>
*/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioport.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/moduleparam.h>
#include <linux/kdev_t.h>
#include <linux/ioport.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/signal.h>
#include <asm/siginfo.h>
#include <linux/interrupt.h>
#include "efm32gg.h"

#define GPIO_EVEN_IRQ_LINE 17
#define GPIO_ODD_IRQ_LINE 18



static ssize_t my_read (struct file *file_point, char __user *buff, size_t count, loff_t *off_poin);
static int my_release (struct inode *inode, struct file *filp);
static int my_open (struct inode *inode, struct file *filp);
static ssize_t my_write (struct file *filp, const char __user *buff, size_t count, loff_t *offp);
static irqreturn_t interrupt_handler(int irq, void* dev_id, struct pt_regs* regs);
static int gamepad_fasync(int fd, struct file* filp, int mode);


#define GPIO_IRQ_EVEN 17
#define GPIO_IRQ_ODD 18
# define DRIVER_NAME "gamepad_group_2"  //måtte lage et navn 
dev_t device_num;
struct cdev my_cdev;  //device setup
struct class *cl;
struct resource *req_GPIO;
struct fasync_struct *async_queue;

static struct file_operations my_fo = {
	.owner = THIS_MODULE,
 	.read = my_read,
 	.write = my_write,
 	.open = my_open,
 	.release = my_release,
 	.fasync = gamepad_fasync,
};
/*
static irqreturn_t interrupt_handler(int irq, void* dev_id, struct pt_regs* regs)
{
        uint32_t source = ioread32(GPIO_IF);
        iowrite32(ioread32(GPIO_IF), GPIO_IFC);
            if (async_queue) { //FUNKER IKKE
        kill_fasync(&async_queue, SIGIO, POLL_IN);
    }
    return IRQ_HANDLED;
}*/

irqreturn_t gpio_interrupt_handler(int irq, void* dev_id, struct pt_regs* regs)
{
    //printk(KERN_ALERT "Handling GPIO interrupt\n");
    printk(KERN_ALERT "Handling GPIO interrupt: Got %d\n", ioread32(GPIO_IF));
    iowrite32(ioread32(GPIO_IF), GPIO_IFC);
    if (async_queue) {
        kill_fasync(&async_queue, SIGIO, POLL_IN);
    }
    return IRQ_HANDLED;
}

static int gamepad_fasync(int fd, struct file* filp, int mode) 
{
    return fasync_helper(fd, filp, mode, &async_queue);
}

/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */
  

static int __init template_init(void)
{
	printk("Hello World local, here is your module speaking\n");
	
	// Finds an available device number and saves it in device_num(returns 0 if it is successfull). "minor nummeret" is assigned to defalt = 0, starts count on 1, name = driver name
	int device_num_status = alloc_chrdev_region(&device_num, 0, 1, DRIVER_NAME); 
	if (device_num_status < 0){
		printk("feil ved driver nummer \n");
	}
	
	// The char device is set up.
	cdev_init(&my_cdev, &my_fo);
	my_cdev.owner = THIS_MODULE;
	
	// Tells the kernel that this device exists.
	int char_dev_status = cdev_add(&my_cdev, device_num, 1);
	if (char_dev_status < 0){
		printk("feil ved char driver \n");
	}
	
	// Gives us access to write to the char device.
	cl = class_create(THIS_MODULE, DRIVER_NAME);
	device_create(cl, NULL, device_num, NULL, DRIVER_NAME);
	
	// Tries to allocate memory to GPIO_PC_MODEL and tells the kernel if this fails.
	req_GPIO = request_mem_region(GPIO_PC_BASE, 0x24, DRIVER_NAME);
    if (req_GPIO == NULL){
        printk("Could not allocate memory region for GPIO_PC");
        return -1;
    }
	// Tries to allocate memory for interrupt and tells the kernel if this fails.
    req_GPIO = request_mem_region(GPIO_EXTIPSELL, 0x24 , DRIVER_NAME);
    if (req_GPIO == NULL){
        printk("Could not allocate memory region for interrupt handling");
        return -1;
    }	
		
	request_irq(GPIO_IRQ_EVEN, (irq_handler_t) interrupt_handler, 0, DRIVER_NAME, &my_cdev);
	request_irq(GPIO_IRQ_ODD,  (irq_handler_t) interrupt_handler, 0, DRIVER_NAME, &my_cdev);
	
	// Set up the GPIO buttons
        iowrite32(0x33333333, GPIO_PC_MODEL);
        iowrite32(0xff, GPIO_PC_DOUT);
	                    
        //Set up the interrupt handler
        iowrite32(0x22222222, GPIO_EXTIPSELL);
        iowrite32(0xff, GPIO_EXTIFALL);
        iowrite32(0xff, GPIO_EXTIRISE);
        iowrite32(0xff, GPIO_IEN);



	printk("fulført med set opp!");
	return 0;
}



/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */


static void __exit template_cleanup(void)
{
	free_irq(GPIO_EVEN_IRQ_LINE, &my_cdev); //FINNES IKKE
    free_irq(GPIO_ODD_IRQ_LINE, &my_cdev); //FINNES IKKE

	release_mem_region(GPIO_EXTIPSELL, 0x24);
	release_mem_region(GPIO_PC_BASE, 0x24);
	
	class_destroy(cl);
	cdev_del(&my_cdev);
	
	unregister_chrdev_region(device_num, 1);
	
	printk("Short life for a small module...\n");
	
	device_destroy(cl, device_num);
	
	
}

static ssize_t my_read (struct file *file_point, char __user *buff, size_t count, loff_t *off_poin){
	printk("started reading buttons");
	uint32_t buttons = ioread32(GPIO_PC_DIN);  //leser knapper
	copy_to_user(buff, &buttons, 1);  //to-from-long end
	return 0;
} 
static int my_open (struct inode *inode, struct file *filp){
	printk("sucessfull open kernel");
	return 1;
}

static int my_release (struct inode *inode, struct file *filp){
	return 0;
}

static ssize_t my_write (struct file *filp, const char __user *buff, size_t count, loff_t *offp){
	return 1;
}

//fc-en sette flagg

module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");
