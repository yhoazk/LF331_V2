/**
 * Simple char module
 */

#include <linux/cdev.h>     /* cdev utils */
#include <linux/fs.h>       /* File operations */
#include <linux/init.h>     /* module_init, module_exit */
#include <linux/module.h>   /* For modules */
#include <linux/slab.h>     /* kmalloc */
#include <linux/uaccess.h>  /* Copy to/from user */


static const char DEV_NAME[] = "mychdrv";
const size_t ramdisk_size = (16*PAGE_SIZE);
static dev_t first;
static unsigned int count = 1;
static int my_major = 700, my_minor = 0;
static struct cdev *my_cdev;
static char* ramdisk;
static int mychdrv_open(struct inode *inode, struct  file *file){
  pr_info("Opening dev: %s\n\n", DEV_NAME);
  return 0;
}


static int mychdrv_release(struct inode *inode, struct file *file){
  pr_info(" Closig device %s\n\n", DEV_NAME);
  return 0;
}

static ssize_t mychdrv_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos){
  int nbytes;
  if((lbuf + *ppos) > ramdisk_size){
    pr_info("Trying to read past the end of dev memory\n");
    return 0;
  }
  nbytes = lbuf - copy_to_user(buf, ramdisk + *ppos, lbuf);
  *ppos += nbytes;
  pr_info("\n Reading fnc, nbytes=%d pos=%d\n", nbytes, (int)*ppos);
  return nbytes;
}

static ssize_t mychdrv_write(struct file *file, const char __user *buf, size_t lbuf,
                             loff_t *ppos){
  int nbytes;
  if((lbuf + ppos) > ramdisk_size){
    pr_info("Trying to write past the end of dev memory, aborting");
    return 0;
  }
  nbytes = lbuf - copy_from_user(ramdisk + *ppos, buf, lbuf);
  *ppos += nbytes;
  pr_info("\nWriting fnc, nbytes=%d pos=%d\n", nbytes, (int)*ppos);
  return nbytes;
}

static const struct file_operations mychdrv_fops = {
  .owner = THIS_MODULE,
  .read = mychdrv_read,
  .write = mychdrv_write,
  .open = mychdrv_open,
  .release = mychdrv_release,
};

static int __init my_init(void){
  ramdisk = kmalloc(ramdisk_size, GFP_KERNEL);
  first = MKDEV(my_major, my_minor);
  register_chrdev_region(first, count, DEV_NAME);
  my_cdev = cdev_alloc();
  cdev_init(my_cdev, &mychdrv_fops);
  cdev_add(my_cdev, first, count);
  pr_info("\nsuccess registering the chr dev %s\n", DEV_NAME);
  return 0;
}

static void __exit my_exit(void){
  cdev_del(my_cdev);
  unregister_chrdev_region(first, count);
  pr_info("\nDevice unregistered\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Thomas Jerringston");
MODULE_LICENSE("GPL v2"); 