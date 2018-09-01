# Understanding Linux `/proc/<pid>/maps` file

Each row in `/proc/$PID/maps` describes a region of contiguous vitual memory
in a processor or thread. Each row has the following fields

```
address                   perms  offset dev   inode     pathname
7fd69be00000-7fd69be55000 r-xp 00000000 f3:00 1987551   /usr/lib64/libz.so.1.5.8
```

### adress

This is the starting and end address of the region in the process address space

### permissions

Thsi describes how pages in the region can be accessed. There are four different
permissions: read, write, execure and shared. if _read/write/execute_ are 
dissabled, a '-' will appear instead of `r/w/x`. If a region is not shared, it's
private, so a `p` will appear instead of an `s` if the process attemps to access
memory in a way that is not permited, a segmentation fault is generated. 
Permissions can be changed using the `mprotect` system call.

### offset

If the region was mapped from a file (using `mmap`), this is the offset in the
file where the mapping begins. If the memory was not mapped from a file, its 0.

### device

If the region was mapped from a file, this is the major and minor device number 
where the file lives.

### inode

If the region was mapped from a file, this is the file number.

### pathname

If the region was mapped from a file, this is the name of the file. This field
is blank for anonymous mapped regions. There are also special regions with 
names like `[heap]`, `[stack]` or `[vdso]`. The `[vdso]` stands for virtual
dynamic shared object. It's used by system calls to switch to kernel mode.



