#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
// https://www.linuxtopia.org/online_books/linux_kernel/linux_kernel_module_programming_2.6/x323.html
MODULE_LICENSE("GPL");
MODULE_AUTHOR("okokok");
MODULE_DESCRIPTION("Kello World");
MODULE_VERSION("0.1");
/**
 * @brief
 * Passing parameters to a driver
 * This module accepts a comma separated argument for device names
 * insmod mod_params.ko dev_names=aaa,eee,ttt
 * 
 * Expected output in dmesg:
 * [182647.248631] Hello Param[0]: aaa
 * [182647.248638] Hello Param[1]: eee
 * [182647.248638] Hello Param[2]: ttt
 */

#define DEV_NAME "mod_params"
// Note the missing comma after the "%s:"
#define mod_params_info(fmt, args...) \
    pr_info("%s: " fmt, __func__, ##args)

#define mod_params_dbg(fmt, args...) \
    pr_debug("%s: " fmt, __func__, ##args)

#define mod_params_err(fmt, args...) \
    pr_err("%s: " fmt, __func__, ##args)

/* Module parameters passed in cmdline */
#define DEVICE_NUMBER (5)
static char *dev_names[DEVICE_NUMBER];
static u32 dev_count;

module_param_array(dev_names, charp, &dev_count, 0644);
MODULE_PARM_DESC(dev_names, "device names(comma separated)");

static int __init mod_params_init(void)
{

    for (size_t i = 0; i < dev_count; i++)
    {
        printk("Hello Param[%li]: %s\n", i, dev_names[i]);
    }
    return 0;
}

static void __exit mod_params_exit(void)
{
    printk("byeee\n");
}

module_init(mod_params_init);
module_exit(mod_params_exit);
