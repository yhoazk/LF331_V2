#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/moduleparam.h>

// execute it as
// sudo insmod ./interrupt.ko interrutp_name="some_name" int irq=1;

// for testing check irq numbers in /proc/interrupts
//
//remove with
// sudo rmmod interrupt

static int irq = 1;
static char* interrutp_name;

module_param(interrutp_name,charp,0);
MODULE_PARM_DESC(interrutp_name, "Name for the interrutp");
module_param(irq, int, 0);
MODULE_PARM_DESC(irq, "The IRQ of the network interface");

struct info_to_pass {
    int* some_int;
    char* some_string;
} dev_info;

static irqreturn_t my_interrupt_handler(int irq, void *dev_id) {
    static int my_count = 0;
    // info is what we passed as &dev_info to request_irq
    // this variable is just an argument carrier from module_init
    // to IRQ handler
    struct info_to_pass *info = dev_id;

    // statuc counter to just print dbg message 10 times
    // saying we received interrupt, adter 10 we dont print
    // but interrupt might still be attended
    //
    if (my_count < 10) {
        printk("Module init: %s\n", info->some_string);
        printk("interrupt != %d\n", my_count);
        my_count++;
    }
    return IRQ_HANDLED;
}

static int __init mymodule_init(void) {
    printk("Module initialized");

    /* request_irq: allocate a given interrupt line  */
    // int request_irq( unsigned int irq,
    //  unsigned long flags,
    //  const char* name,
    //  void* dev)
    dev_info.some_string = "hell";

    if (request_irq(irq, &my_interrupt_handler, IRQF_SHARED, interrutp_name, &dev_info)) {
        printk(KERN_ERR "Cannot reguster IRQ: %d\n", irq);
        return -EIO;
    }

    // the second parameter, handler is a function pointer to the actual interrutp
    // handler that services this interrupt. This function is invoked whenever he operating
    // system receives the interrupt
    //
    // IRQF_SHARED flag specifies that the interrupt line can be shared
    // among multiple interrupt handlers, Each handler registered on a given line 
    // must specify this flag; otherwise only one handler can exist per line.
    printk("request IRQ %d succeeded\n", irq);
    return 0;
}

// Note that a function w/o parameters is not the same
// that a function with void parameter
static void __exit mymodule_exit(void) {
    free_irq(irq, &dev_info);
    printk("freeing irq %d\n", irq);

    printk("unload module\n");
}

module_init(mymodule_init);
module_exit(mymodule_exit);
MODULE_LICENSE("GPL");
