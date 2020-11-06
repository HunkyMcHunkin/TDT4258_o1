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




# define DRIVER_NAME "group_2"  //måtte lage et navn 
dev_t devise_num;
struct cdev my_cdev;  //devise setup
struct class *cl;

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
	
	//finner en ledig devise nummer og lagrer det i devise_num(returnerer 0 hvis sucsessfull). "minor nummeret" er satt til defalt = 0, starter count på 1, navn = driver navn
	int devise_num_status = alloc_chrdev_region(&devise_num, 0, 1, DRIVER_NAME); 
	if (devise_num_status < 0){
		printk("feil ved driver nummer \n");
	}
	
	//setter opp char device
	cdev_init(&my_cdev, &my_fo);
	my_cdev.owner = THIS_MODULE;
	
	
	
	//forteller kernel at denne devicen eksisterer
	int char_dev_status = cdev_add(&my_cdev, devise_num, 1);
	if (char_dev_status < 0){
		printk("feil ved char driver \n");
	}
	
	//gir oss tilgang til å skrive til char devisen 
	cl = class_create(THIS_MODULE, DRIVER_NAME);
	device_create(cl, NULL, devise_num, NULL, DRIVER_NAME);
	

	
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
	 printk("Short life for a small module...\n");
}





module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

