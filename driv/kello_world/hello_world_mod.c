#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("okokok");
MODULE_DESCRIPTION("Kello World");
MODULE_VERSION("0.1");

static int __init hello_mod_init(void) {
    printk("hellllll\n");
    return 0;
}

static void __exit hello_mod_exit(void) {
    printk("byeee\n");
}

module_init(hello_mod_init);
module_exit(hello_mod_exit);
