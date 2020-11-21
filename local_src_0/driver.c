/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>  //to allocate and freeing devise numbers and use file opperations
#include <linux/kdev_t.h>
#include <linux/cdev.h>  //make the kernel a char devise
#include <linux/device.h>
#include "efm32gg.h"
#include <linux/uaccess.h> //to use function copy_to_user -allows user access
#include <stdint.h>

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
#include <linux/moduleparam.h>
//#include <linux/siginfo.h>

//#include <linux/iqr.h>
#include <asm/signal.h>

#include <linux/types.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/mm.h>

#include <linux/signal.h>  		// Signal usage
#include <linux/sched.h>  		// Used for current PID

static int __init template_init(void);
static void __exit template_cleanup(void);


/*
static int	gamepad_open (struct inode *, struct file *);
static int	gamepad_release(struct inode *, struct file *);
static ssize_t	gamepad_read(struct file *, char *, size_t, loff_t *);
static ssize_t	gamepad_write(struct file *, const char *, size_t, loff_t *);
void 		transfer_interrupt(unsigned long);
static 		irqreturn_t KERNEL_IRQ_HANDLER(int, void*);
*/

static ssize_t my_read (struct file *file_point, char __user *buff, size_t count, loff_t *off_poin);
static int my_release (struct inode *inode, struct file *filp);
static int my_open (struct inode *inode, struct file *filp);
static ssize_t my_write (struct file *filp, const char __user *buff, size_t count, loff_t *offp);
static irqreturn_t interrupt_handler(int irq, void* dev_id, struct pt_regs* regs);
// void 		transfer_interrupt(unsigned long);

/*Define driver constants*/
#define GPIO_IRQ_EVEN 17
#define GPIO_IRQ_ODD 18
#define DRIVER_NAME "gamepad_group_2"  //måtte lage et navn 


dev_t device_num;
struct cdev my_cdev;  //device setup
struct class *cl;
struct device *d1;
struct task_struct *task; 
//static struct device* d1;
/*static int majorNumber;			// Assigned major number
struct task_struct *task;*/  		// Takes pid number of child process
struct siginfo out_signal = {};

struct resource *req_GPIO;
//void __iomem *virt_GPIO_C;
//void __iomem *virt_GPIO_intr;
int device_num_status;
int char_dev_status;
uint32_t buttons;
int char_dev_status


static struct file_operations my_fo = {
	.owner = THIS_MODULE,
 	.read = my_read,
 	.write = my_write,
 	.open = my_open,
 	.release = my_release,
};

DECLARE_TASKLET(button_data, iqr_queue, 0)

static int __init template_init(void)
{
	printk("Hello World local, here is your module speaking\n");
	
	device_num_status = alloc_chrdev_region(&device_num, 0, 1, DRIVER_NAME); 
	if (device_num_status < 0){
		printk("feil ved driver nummer \n");
		return -1
	}
	
	cdev_init(&my_cdev, &my_fo);
	my_cdev.owner = THIS_MODULE;
	
	char_dev_status = cdev_add(&my_cdev, device_num, 1);
	if (char_dev_status < 0){
		printk("feil ved char driver \n");
	}
	cl = class_create(THIS_MODULE, DRIVER_NAME);
	d1 = device_create(cl, NULL, device_num, NULL, DRIVER_NAME);
	
	out_signal.si_signo = SIGUSR1;
	out_signal.si_code = SI_KERNEL;
	out_signal.si_int = SIGUSR1; 
	return 0;
}

static irqreturn_t interrupt_handler(int irq, void* dev_id, struct pt_regs* regs)
{
        uint32_t source = ioread32(GPIO_IF);
        iowrite32(source, GPIO_IFC);
        tasklet_schedule(&button_data);
        return IRQ_HANDLED;
}

static irq_queue()
{
	send_sig_info(SIGUSR1, &out_signal, task);
}

static void __exit template_cleanup(void)
{
	printk("Short life for a small module...\n");

    free_irq(GPIO_IRQ_EVEN, &device_num);
    free_irq(GPIO_IRQ_ODD, &device_num);
	
	release_mem_region(GPIO_EXTIPSELL, 0x20);
	release_mem_region(GPIO_PC_BASE, 0x24);

    device_destroy(cl, device_num);
    class_destroy(cl);
    cdev_del(&my_cdev);
    /* Dealloc the device numbers */
    unregister_chrdev_region(device_num, 1);
}

static ssize_t my_read (struct file *file_point, char __user *buff, size_t count, loff_t *off_poin){
	printk("started reading buttons");
	buttons = ioread32(GPIO_PC_DIN);  //leser knapper
	copy_to_user(buff, &buttons, 1);  //to-from-long end
	return 0;
} 
static int my_open (struct inode *inode, struct file *filp){
	printk("sucessfull open kernel");
	
	task = current;
	
	// Tries to allocate memory to GPIO_PC_MODEL and tells the kernel if this fails.
	req_GPIO = request_mem_region(GPIO_PC_BASE, 0x24, DRIVER_NAME);
    if (req_GPIO == NULL){
        printk("Could not allocate memory region for GPIO_PC");
        return -1;
    }
    	// Tries to allocate memory for interrupt and tells the kernel if this fails.
    req_GPIO = request_mem_region(GPIO_EXTIPSELL, 0x20 , DRIVER_NAME);
    if (req_GPIO == NULL){
        printk("Could not allocate memory region for interrupt handling");
        return -1;
    }	
    
    	request_irq(GPIO_IRQ_EVEN, interrupt_handler, 0, DRIVER_NAME, &my_cdev);
		request_irq(GPIO_IRQ_ODD,  interrupt_handler, 0, DRIVER_NAME, &my_cdev);
	
		// Set up the GPIO buttons
        iowrite32(0x33333333, GPIO_PC_MODEL);
        iowrite32(0xff, GPIO_PC_DOUT);
        
        //Set up the interrupt handler
        iowrite32(0x22222222, GPIO_EXTIPSELL);
        iowrite32(0xff, GPIO_EXTIFALL);
        iowrite32(0xff, GPIO_EXTIRISE);
        iowrite32(0xff, GPIO_IEN);
		printk("finished set up!");
	return 1;
}

static int my_release (struct inode *inode, struct file *filp){
	return 0;
}

static ssize_t my_write (struct file *filp, const char __user *buff, size_t count, loff_t *offp){
	return 1;
}


module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");
























#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>  //to allocate and freeing devise numbers and use file opperations
#include <linux/kdev_t.h>
#include <linux/cdev.h>  //make the kernel a char devise
#include <linux/device.h>
#include "efm32gg.h"
#include <linux/uaccess.h> //to use function copy_to_user -allows user access
//#include <linux/fb.h>  //graphic
#include <linux/ioport.h> //allocation of io memory
#include <asm/system.h> // adds barriers
//#include <linux/stat.h> //who can access the rep. of module param. in sysfs, define you own param.
#include <linux/errno.h> //for error handling, use goto
#include <asm/current.h> //can access current process
#include <asm/io.h> //to access io ports
#include <linux/interrupt.h>
#include <linux/moduleparam.h>
#include <asm/signal.h>
#include <linux/types.h>
#include <asm/uaccess.h>
#include <linux/signal.h>  
#include <linux/sched.h>

static int __init template_init(void);
static void __exit template_cleanup(void);

void irq_queue(unsigned long);
static irqreturn_t irq_handler(int irq, void* dev_id, struct pt_regs* regs);


static ssize_t my_read (struct file *file_point, char __user *buff, size_t count, loff_t *off_poin);
static int my_release (struct inode *inode, struct file *filp);
static int my_open (struct inode *inode, struct file *filp);
static ssize_t my_write (struct file *filp, const char __user *buff, size_t count, loff_t *offp);

/*Define driver constants*/
#define GPIO_IRQ_EVEN 17
#define GPIO_IRQ_ODD 18
#define DRIVER_NAME "gamepad_group_2"  //måtte lage et navn 


dev_t device_num;
struct cdev my_cdev;  //device setup
struct class *cl;
struct device *d1;
struct task_struct *task; 
struct siginfo out_signal = {};

int device_num_status;
int char_dev_status;
uint32_t buttons;
int char_dev_status;
int irq_var;
void *req_GPIO;

static struct file_operations my_fo = {
	.owner = THIS_MODULE,
 	.read = my_read,
 	.write = my_write,
 	.open = my_open,
 	.release = my_release,
};

DECLARE_TASKLET(button_action, irq_queue, 0);

static int __init template_init(void)
{
	printk("Hello World local, here is your module speaking\n");
	
	device_num_status = alloc_chrdev_region(&device_num, 0, 1, DRIVER_NAME); 
	if (device_num_status < 0){
		printk("feil ved driver nummer \n");
		return -1;
	}
	
	cdev_init(&my_cdev, &my_fo);
	my_cdev.owner = THIS_MODULE;
	
	char_dev_status = cdev_add(&my_cdev, device_num, 1);
	if (char_dev_status < 0){
		printk("feil ved char driver \n");
	}
	cl = class_create(THIS_MODULE, DRIVER_NAME);
	d1 = device_create(cl, NULL, device_num, NULL, DRIVER_NAME);


    
	out_signal.si_signo = SIGUSR1;
	out_signal.si_code = SI_KERNEL;
	//out_signal.si_int = SIGUSR1; 
	return 0;
}

static irqreturn_t irq_handler(int irq, void* dev_id, struct pt_regs* regs)
{
        uint32_t source = ioread32(GPIO_IF);
        iowrite32(source, GPIO_IFC);
        tasklet_schedule(&button_action);
        return IRQ_HANDLED;
}

void irq_queue(unsigned long arg)
{
	send_sig_info(SIGUSR1, &out_signal, task);
}

static void __exit template_cleanup(void)
{
	printk("Short life for a small module...\n");


    device_destroy(cl, device_num 0);
    class_destroy(cl);
    cdev_del(&my_cdev);
    /* Dealloc the device numbers */
    unregister_chrdev_region(device_num, 1);
}

static ssize_t my_read (struct file *file_point, char __user *buff, size_t count, loff_t *off_poin){
	printk("started reading buttons");
	buttons = ioread32(GPIO_PC_DIN);  //leser knapper
	copy_to_user(buff, &buttons, sizeof(buttons));  //to-from-long end
	return 0;
	} 
	
static int my_open (struct inode *inode, struct file *filp){
	printk("sucessfull open kernel");
	
	task = current;
				
	// Tries to allocate memory to GPIO_PC_MODEL and tells the kernel if this 		fails.
		req_GPIO = request_mem_region(GPIO_PC_BASE, 0x24, DRIVER_NAME);
    if (req_GPIO == NULL){
        printk("Could not allocate memory region for GPIO_PC");
        return -1;
    }
    	// Tries to allocate memory for interrupt and tells the kernel if this 			fails.
    	req_GPIO = request_mem_region(GPIO_EXTIPSELL, 0x20 , DRIVER_NAME);
    if (req_GPIO == NULL){
        printk("Could not allocate memory region for interrupt handling");
        return -1;
        }
    printk("successfull allocated memory");
    irq_var = request_irq(GPIO_IRQ_EVEN, irq_handler, 0, DRIVER_NAME, device_num);
    if (irq_var) {
	printk("Problem with requesting interrupt");
	}
	irq_var = request_irq(GPIO_IRQ_ODD,  irq_handler, 0, DRIVER_NAME, device_num);
	if (irq_var) {
	printk("Problem with requesting interrupt");
	}
	 printk("sucessfull requested interrupts");
	
	// Set up the GPIO buttons
    iowrite32(0x33333333, GPIO_PC_MODEL);
    iowrite32(0xff, GPIO_PC_DOUT);
    printk("sucessfull set up of gpio buttons");
    //Set up the interrupt handler
    iowrite32(0x22222222, GPIO_EXTIPSELL);
    iowrite32(0xff, GPIO_EXTIFALL);
    iowrite32(0xff, GPIO_EXTIRISE);
    iowrite32(0xff, GPIO_IEN);
    printk("finished set up!");
	return 1;
}

static int my_release (struct inode *inode, struct file *filp){

	free_irq(GPIO_IRQ_EVEN, &device_num);
    free_irq(GPIO_IRQ_ODD, &device_num);
    release_mem_region(GPIO_EXTIPSELL, 0x20);
	release_mem_region(GPIO_PC_BASE, 0x24);

	return 0;
}

static ssize_t my_write (struct file *filp, const char __user *buff, size_t count, loff_t *offp){
	return 1;
}

module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

