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



The linker looks for `so` files in the paths specified in the file:
`/etc/ld.so.conf`, after the contents of this file were modified the next
command must be run: `ldconfig -v` this will update the library cache


#### Linux commands and how they work

When a user runs a simple command as `cat /etc/hosts/` the system needs to
complete several tasks in order to execute that command.

* The `cat` command must be read and loaded from disk.
* Related libraries must be found and loaded into RAM
* The `/etc/hosts` file needs to be located on disk.
* Permissions of the current user need to be checked on this file.
* If that is appropiate, the file contents can be copied to RAM.

All this tasks are provided through system calls and library calls.

#### Understanding system calls
As the process cannot access the kernel directly, system calls are used as an
interface, glibc provides a library interface to use system calls from programs.

Two important system calls are `frok()` and `exec()`, these determine how a
process is starts. `fork()` creates almost an identical copy of the current 
process. `exec()` the kernel starts a program, which replaced the current 
process.

Executing a `ls`
```
shell
   fork()->shell
          exec() <- removes the shell code with the ls command
```

#### Intro to system calls
* `man 2 intro`
* `man 2 syscall`

See the memory maps:
`man memmap`

Using the directories in `proc`, to check what is the command line used to run
the current process use: `cat /proc/<pid>/cmdline`. After we can check the
mappings, for fedora `cat /proc/<pid>/numa_maps`.


#### Library calls

`man 3 intro`

user space is memory that is allocated by the kernel for user process. Several
elements are all running in user space:
- Network config
- Sevices like web server
- applications
- user interfaces

User are created to assign permissions and limits to entities on linux, every
process or task running on linux has an owner

```
strace -c ls
```
For library calls use `ltrace` 

#### Signals
A signlas provies a software interrupt; it's a method to tell a process that
it has to do something. 
Signals are strictly defined, see `man 7 signal` for information.
Some signals cannot be ignored.

for sending signals use the kill command and the flag -s <SIGNAME>


For the booting process, when the drivers for the disk are not yet present
the disk can be accessed using the *Linear Block Access* LBA which provides
a slow, but uniform method to access disks.

## Avoid alias on a command:

To avoid a command to be replaced with the system alias escape the command with
`\` for example `\ls` 
