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
of `printk_ratelimited(fmt, ...)`



- is `printk` buffered?
- Where is the buffer located in RAM? 
- If we deactivate memory layout randomization is possible to get the contents of the RAM log?
- There's any mechanism to store the buffer of `printk` in case of a kernel error?
- 