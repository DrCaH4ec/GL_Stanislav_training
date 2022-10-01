
// Made by CYB3RSP1D3R

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>


MODULE_DESCRIPTION("Hello kernel");
MODULE_AUTHOR("CYB3RSP1D3R");
MODULE_VERSION("1.0");
MODULE_LICENSE("Dual MIT/GPL");

static char *name = "username"; // variable for the name with default value
static int continious = 0; // variable which indicates about continious print
module_param(name, charp, 0);
MODULE_PARM_DESC(name, "Variable for passing the name via command line");
module_param(continious, int, 0);
MODULE_PARM_DESC(continious, 
                 "If 'continious' is not 0, then module will continiously print hello message");

static void print_hello(unsigned long data);
DECLARE_TASKLET(hello_tasklet, print_hello, 0);

static unsigned long hello_counter = 1;


static void print_hello(unsigned long data)
{
	printk(KERN_INFO "Hello %s, %lu times\n", name, ++hello_counter);
}


static int __init init_mod(void)
{
    if (0 != continious) {
        tasklet_hi_schedule(&hello_tasklet);
    	printk(KERN_INFO "Hello, %s, for the first time!\n", name);
    } else {
        printk(KERN_INFO "Hello, %s!\n", name);
    }
	return 0;
}
 

static void __exit cleanup_mod(void)
{
    if (0 != continious) {
	    tasklet_kill(&hello_tasklet);
    }
	printk(KERN_INFO "Goodbye, %s?\n", name);
}
 

module_init(init_mod);
module_exit(cleanup_mod);
