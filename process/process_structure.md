# Process structure


### To find the limits of the stack, heap, file locks, etc.

```
cat /proc/<pid>/limits
Limit                     Soft Limit           Hard Limit           Units     
Max cpu time              unlimited            unlimited            seconds   
Max file size             unlimited            unlimited            bytes     
Max data size             unlimited            unlimited            bytes     
Max stack size            8388608              unlimited            bytes     
Max core file size        unlimited            unlimited            bytes     
Max resident set          unlimited            unlimited            bytes     
Max processes             128105               128105               processes 
Max open files            1073741816           1073741816           files     
Max locked memory         67108864             67108864             bytes     
Max address space         unlimited            unlimited            bytes     
Max file locks            unlimited            unlimited            locks     
Max pending signals       128105               128105               signals   
Max msgqueue size         819200               819200               bytes     
Max nice priority         0                    0                    
Max realtime priority     0                    0                    
Max realtime timeout      unlimited            unlimited            us   
```

This limits are set by the OS, but they can be changed with `ulimit` and/or
`cgroups`.


--- 

note: cgrops limit how much a process can use, while namespaces limit what a
process can use.
