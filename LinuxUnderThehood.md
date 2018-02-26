Linux under the Hood

1.1 understanding the stack:

 User
processes use libraries



The kernel is the only one to access the HW.
Users cannot directly access the HW, they need to go through the 
kernel.

System space / User space


There are a limited amount of options for the user to get to the 
kernel:

- Signals
- System calls

Provided by the kernel.

The kernel knows how to use the HW by using drivers.
The linux kernel is pluggable, the drivers do not form part of the 
kernel.


Network is different from other drivers, they go direclty to the kernel.

