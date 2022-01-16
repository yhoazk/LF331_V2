# intro to linux device driver (2005)

## Anatomy of a device driver

a device driver has three sides: one takls to the rest of the kernel, one to the hardware and one to the user.

## Kernel interface of a device driver 

in order to talk to the kernel, the driver registers with subsistems to respond to events, Such an event might be the opening of a file, a page fault, the plugging in of a new USB device, etc.

## User interface of a device driver

- Since in linux everything is a file, users talk with device drivers through device files.
- Device files are mechanisms, supplied by the kernel for user interface
- There are three main types of drivers
  - char
  - block
  - netowrk

## Anatomy of the klife device driver 

- The user talks with the klife through /dev/klife device.
  - when the user opens /dev/kfile, the kernel calls klife open routine
  - the same for close, reads and writes
- klife talks to the kernel through 
  - initialization function
  - register_chrdev function
  - hooking into the timer input

## Driver initialization code

```c
static int __init klife_module_init() {
    int ret;
    pr_debug("klife module init called");
    if ((ret = register_chrdev(KLIFE_MAJOR_NUM, "klife", &klife_fops)))
        printk(KERN_ERR "register_chrdev: %d\n", ret);
    
    return ret;
}
```

## Driver initialization

- one function `init` is called on the drivers initialization
- one funciton `exit` is called when the driver is removed from the system
- question: what happes if the driver is compiled into the kernel rather than as a module?
  - The init function will register hooks that will get teh driver's code called when the appropiate event happens
- question: what if the init function does not register any hooks?
  - there are various hooks that can be registered:
    - file operations
    - pci operations
    - usb operations
    - network operations

## Registering chardev hooks

```c
struct file_ops klife_ops = {
    .owner = THIS_MODULE,
    .open = klife_open,
    .release = klife_release,
    .read = klife_read,
    .write = klife_write,
    .mmap = klife_mmap,
    .ioctl = klife_ioctl
};
```


## User space access to the driver

Driver registers a char device tied to a given __major number__ which is created with:

```
mknod /dev/klife c 250 0
```

The user will open that file as a regular file:

```c
if ((kfd = open("/dev/klife", O_RDWR)) < 0) {
    perror("open /dev/klife");
    exit(EXIT_FAILURE);
}
```

## File operations

After the file descriptior is creted the user should use the functions:

- `open`: starts the game (allocates resources)
- `release`: finish the game, release resources
- `write`: initialite the game by setting the start positions on the grid
- `read`: for generating and the reading the next state of the games grid
- `ioctl`: for quering the current generation number, and for enabling hooking into the timer interrupt
- `mmap`: for potentially faster but complex direct access to the game

## The open and release routines

open is where any setup not done in initialization is done
release is where the clean up not done in unload time is done

## `klife_open`