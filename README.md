# LF331_V2
Linux Device Drivers V2
[Vids](https://free-electrons.com/blog/elce-2008-videos/)



## 3.7 Error Numbers:


There are a bunch of error numbers defined in linux, the first bunch are in
For fedora 24 `/usr/src/kernels/4.11.12-100.fc24.x86_64+debug/include/uapi/asm-generic`

Usually one returns `-ERROR`, while the return from the system is almos always `-1`
with the actual code error being stored in the thread local variable `errno` in user space.


https://kukuruku.co/post/writing-a-file-system-in-linux-kernel/
