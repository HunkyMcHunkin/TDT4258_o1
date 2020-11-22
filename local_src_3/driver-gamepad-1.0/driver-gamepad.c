/*
The kernel module for gamepad
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/cdev.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/signal.h>
#include <asm/siginfo.h>
#include <linux/interrupt.h>
#include <linux/moduleparam.h>
#include <linux/kdev_t.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioport.h>

#include "efm32gg.h"

//DEFINES
#define GPIO_IRQ_EVEN 17
#define GPIO_IRQ_ODD 18
#define DRIVER_NAME "gamepad"
#define DEVICE_NAME "gamepad"
#define DEVICE_NUMBER 1


//FUCTION DEFINITIONS
static int __init gp_device_init(void);
static void __exit gp_device_exit(void);
static int my_open(struct inode *, struct file *);
static int my_release(struct inode *, struct file *);
static ssize_t my_read(struct file *, char *__user, size_t, loff_t *);
static ssize_t my_write(struct file *, char *__user, size_t, loff_t *);
static irqreturn_t interrupt_handler(int, void *, struct pt_regs *);
static int my_fasync(int, struct file *, int mode);

//VARIABLES
static dev_t device_number;
struct cdev pg_cdev;
struct fasync_struct *queue;
struct class *cl;

static struct file_operations gamepad_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_release,
	.read = my_read,
	.write = my_write,
	.fasync = my_fasync,
};

//MODULE SPECIFICATIONS
MODULE_DESCRIPTION("Description gamepad kernel module");
MODULE_LICENSE("GPL");
module_init(gp_device_init);
module_exit(gp_device_exit);


//interrupt handler
irqreturn_t interrupt_handler(int irq, void* dev_id, struct pt_regs* regs)
{
    printk(KERN_ALERT "interrupt handler\n");
    iowrite32(ioread32(GPIO_IF), GPIO_IFC);
    if (queue) {
        kill_fasync(&queue, SIGIO, POLL_IN);
    }
    return IRQ_HANDLED;
}

//Gets information
static int my_fasync(int fd, struct file* filp, int mode) 
{
    return fasync_helper(fd, filp, mode, &queue);
}


//Set module into kernel space
static int __init gp_device_init(void)
{
	printk("Hello World, gamepad module here\n");
	
	if (alloc_chrdev_region(&device_number, 0, DEVICE_NUMBER, DRIVER_NAME) < 0)
	{
		printk(KERN_ALERT "Device memory not allocated\n");
		return -1;
	}


	if (request_mem_region(GPIO_PC_BASE, 0x020, DEVICE_NAME) == NULL){
		printk(KERN_ALERT "GPIO_PC_BASE memory region unaccessable");
		release_mem_region(GPIO_PC_BASE, 0x020);

		return -1;
	}

	if (request_mem_region(GPIO_EXTIPSELL, 0x020, DEVICE_NAME) == NULL)
	{
		printk(KERN_ALERT "GPIO_EXTIPSELL memory region unaccessable");
		release_mem_region(GPIO_PC_BASE, 0x020);
		release_mem_region(GPIO_EXTIPSELL, 0x020);

		return -1;
	}

	//Button set up and interrupt set up for the gamepad
	iowrite32(0x33333333, GPIO_PC_MODEL);
	iowrite32(0xFF, GPIO_PC_DOUT);
	iowrite32(0x22222222, GPIO_EXTIPSELL);
	iowrite32(0xFF, GPIO_EXTIFALL);
	iowrite32(0xFF, GPIO_IEN);


	//Interrupt set up for the kernal
	request_irq(GPIO_IRQ_EVEN, (irq_handler_t)interrupt_handler, 0, DRIVER_NAME, &pg_cdev);
    request_irq(GPIO_IRQ_ODD, (irq_handler_t)interrupt_handler, 0, DRIVER_NAME, &pg_cdev);
	
	//Device set up
    cdev_init(&pg_cdev, &gamepad_fops);
    pg_cdev.owner = THIS_MODULE;
    cdev_add(&pg_cdev, device_number, DEVICE_NUMBER);
    cl = class_create(THIS_MODULE, DRIVER_NAME);
    device_create(cl, NULL, device_number, NULL, DRIVER_NAME);


	return 0;
}


//Remove module from kernal space
static void __exit gp_device_exit(void)
{
	printk("Exit gamepad driver\n");
	
	iowrite32(0x00, GPIO_IEN);
	
	free_irq(GPIO_IRQ_EVEN, &pg_cdev);
    free_irq(GPIO_IRQ_ODD, &pg_cdev);

	release_mem_region(GPIO_PC_BASE, 0x020);
	release_mem_region(GPIO_EXTIPSELL, 0x020);

	class_destroy(cl);
	cdev_del(&pg_cdev);

	unregister_chrdev_region(device_number, DEVICE_NUMBER);
}

//run when calles open in userspace
static int my_open(struct inode* inode, struct file* filp)
{
	printk(KERN_INFO "Open gamepad driver\n");
	return 0;
}

//run when calles release in userspace
static int my_release(struct inode* inode, struct file* filp)
{
	printk(KERN_INFO "Releasing gamepad driver\n");
	return 0;
}

//run when calles read in userspace
static ssize_t my_read(struct file* filp, char* __user buff, size_t count, loff_t* offp)
{
    uint32_t data = ioread32(GPIO_PC_DIN);
    copy_to_user(buff, &data, 1);
    return 1;
}

//run when calles write in userspace
static ssize_t my_write(struct file* filp, char* __user buff, size_t count, loff_t* offp)
{
    return 1;
}

