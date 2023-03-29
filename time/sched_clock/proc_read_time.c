/****/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/sched/clock.h>
#include <linux/slab.h>

int len;
char* msg;
ssize_t size;
// struct timespec ts;
u64 inittime;

ssize_t read_proc(struct file* filp, char* buf, size_t count, loff_t* offp) {
    u64 time = sched_clock();
    char* temp = kmalloc(50*sizeof(char), GFP_KERNEL);
    sprintf(temp, "%lld", time);
    len = strlen(temp);
    size = sizeof(char) * len;
    
    return simple_read_from_buffer(buf, count, offp, temp, size);
}

// const struct file_operations proc_fops = {
const struct proc_ops proc_fops = {
    proc_read: read_proc
};

void create_new_proc_entry(void) {
    msg = "hello";
    size = strlen(msg);
    proc_create("hello", 0, NULL, &proc_fops);
    inittime = sched_clock();
}

int proc_init(void) {
    create_new_proc_entry();
    return 0;
}

void proc_cleanup(void) {
    remove_proc_entry("hello", NULL);
}

MODULE_LICENSE("GPL");
module_init(proc_init);
module_exit(proc_cleanup);