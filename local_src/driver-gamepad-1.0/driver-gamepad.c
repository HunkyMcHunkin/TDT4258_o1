/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>  //to allocate and freeing devise numbers and use file opperations
#include <linux/kdev_t.h>
#include <linux/cdev.h>  //make the kernel a char devise
#include <linux/device.h>
#include <linux/fb.h>  //graphic

#include <linux/ioport.h> //allocation of io memory
#include <asm/system.h> // adds barriers
#include <linux/stat.h> //who can access the rep. of module param. in sysfs, define you own param.
#include <linux/errno.h> //for error handling, use goto
#include <linus/sched.h>  //pointer to struct task_struct
#include <asm/current.h> //can access current process
#include <asm/io.h> //to access io ports




# define DRIVER_NAME "group_2"  //måtte lage et navn 
dev_t device_num;
struct cdev my_cdev;  //device setup
struct class *cl;
struct resource *req_GPIO;

static struct file_operations my_fo = {
	.owner = THIS_MODULE,
 	//.read = my_read,
 	//.write = my_write,
 	//.ioctl = my_ioctl,
 	//.open = my_open,
 	//.release = my_release,
};


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
	int char_dev_status = cdev_add(&my_cdev, devise_num, 1);
	if (char_dev_status < 0){
		printk("feil ved char driver \n");
	}
	
	// Gives us access to write to the char device.
	cl = class_create(THIS_MODULE, DRIVER_NAME);
	device_create(cl, NULL, device_num, NULL, DRIVER_NAME);
	
	// Tries to allocate memory to GPIO_PC_MODEL and tells the kernel if this fails.
	req_GPIO = request_mem_region(GPIO_PC_BASE, 0x24, DRIVER_NAME)
    	if (req_GPIO == NULL)
             printk("Could not allocate memory region for GPIO_PC")
             return -1
        
        iowrite32(0x33333333, GPIO_PC_MODEL)
        iowrite32(0xff, GPIO_PC_DOUT)
	
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
	release_mem_region(GPIO_PC_BASE, 0x24)
	printk("Short life for a small module...\n");
}





module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

