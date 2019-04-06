# `printk()`

`printk` is similar to standard `printf` function but there are important differences:

- `printk` does not support floats
- Every message in a printk has a log level by default `KERN_INFO`.


| macro          | Level          | Meaning        |
| :------------- | :------------- | :------------- |
| `KERN_EMERG`   | 0              | system is unusable                |
| `KERN_ALERT`   | 1              | action must be taken              |
| `KERN_CRIT`    | 2              | critical conditions               |
| `KERN_ERR`     | 3              | error conditions                  |
| `KERN_WARNING` | 4              | warning conditions                |
| `KERN_NOTICE`  | 5              | normal but significant conditions |
| `KERN_INFO`    | 6              | informational                     |
| `KERN_DEBUG`   | 7              | debug-level messages              |


Example:
```c
printk(KERN_INFO "The device was opened with major number: %d\n", major_number);
```


The pseudo file `/proc/sys/kernel/printk` list the levels set on the system:
```bash
$ cat /proc/sys/kernel/printk
4    4    1    7
```


To control the rate limit of the messages printed by `printk` there's the
function `int printk_ratelimit(void)`

A tipical use would be:

```c
if(printk_ratelimit())
{
  printk(KERN_WARNING "Device failing");
}
```

If the threshold is exceeded `printk_ratelimit()` will return `0`.

The threshold can be contolled by modifying `/proc/sys/kernel/printk_ratelimit`
and the burst time in `/proc/sys/kernel/printk_ratelimit_burst`


**NOTE:** Since kernel version 2.6.33 `printk_ratelimit()` is deprecated in favor
of `printk_ratelimited(fmt, ...)` for the use case when the message is needed only
once use the function `printk_once`.


- is `printk` buffered?
  yes the messages are stored in a ring buffer of size `__LOG_BUF_LEN` which is
  equal to `1 << CONFIG_LOG_BUF_SHIFT`
- Where is the buffer located in RAM? 
- If we deactivate memory layout randomization is possible to get the contents of oKERNlljam attack.RAM log?
- There's any mechanism to store the buffer of `printk` in case of a kernel error?
- 


For debugging upto which point in the execution was reached it's useful to
use the macros `__FUNCTION__` and `__LINE__` for example:

```cpp
printk(KERN_ALERT "DEBUG: Exec passed %s::%d \n", __FUNCTION__, __LINE__)
```
For debugging most users use the `KERN_ERR` to ensure that the log gets to the
console. `pr_err`

There is a special macro and function for message continuations: `KERN_CONT` and `pr_cont`


### Print from userspace

To print into `dmesg` from userspace use:
```sh
echo "Dmesg message" >  /dev/kmsg
```
The message gets the default value, if a different level is required:

```sh
echo "2Writing critical to dmesg" > /dev/kmsg
```

If `/dev/kmsg` is not present can be created with:

```sh
mknod -m 600 /dev/kmsg c 1 11
```


## Early printing

Use the function `early_printk` to write directly to the VGA buffer or serial port