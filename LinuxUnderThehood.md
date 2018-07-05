Linux under the Hood


### Linux file descriptios 

- Linux is a file oriented OS
- Everything is happening as a file
  - Device access
  - IO handling
  - Interprocess communication
- every process keeps a table of the file descritptors that are in use in 
`/rpoc/<pid>/fd
- Common file descriptors:
  - `0`: STDIN
  - `1`: STDOUT
  - `2`: STDER

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


- - -

- The linux kernel is not compiled with all possible disk drivers inside; then
it needs the `initramfs`. Also the initramfs contains proprietary drivers and
configurations as a loadable modules.

This means that during boot there are 2 file systems, the ram and the init the "real"
Using the initramfs is optional, only needed if not the modules are in the kernel.


#### initrd

The initrd is initial ram disk. Its located in `/boot` and is in the format of a `cpio`
file, we can see the contents of the `initrd`.

```
$ file /boot/initrd-4.*
/boot/initrd* : ASCII cpio archive SVR4 with no CRC
$ cat /boot/initrd* | cpio -i --no-absolute-filenames
888 blocks
```
If the file is compressed then we need to replace the `cat` command for the `zcat` command. 


#### The infamous `dracut`

This program generates a `initramfs` which is a file system that is created automatically
when installed. But it also can be created with the `dracut` programm. 
This command creates the file system with a given configuration.

The configuration is located in `/etc/dracut.conf`. See `man dracut.conf(5)`
in this file we can add drivers, file systems and other options. 

---

In the grub menu press `e` to edit the configuration.
Lets boot in the phase where the `initramfs` to be loaded, but systemd not has loaded it yet.
In the kernel load line add the instruction `rd.break`

```
linux16 /vmlinuz-4.* root=/dev/mapper... rd.lvm.lv=centos/swap rd.break
```
This means that it will stop when loads the `intramfs`, this can be used to enter in a machine
w/o password.



##### `init` vs `exec init`

This commands are to be run in the kernel mode.
The first command `init` will fail because the `init` is being forked in the shell
and it won 't have the PID==1. But the `exec init` replaces the current process which
is a shell with the `init` itself, then it works.


`init` is not the initial process anymore. Now it can be also upstart or systemd.
Anyhow the init proces should be located int `/sbin/init` and is the responsible for 
starting the user space environment.

The `/sbin/init` could be a link. Then if you want to check if your machine is using
init, upstart or systemd check where is pointing the `/sbin/init` file.

eg:
```
ls -alth /sbin/ini*
lrwxrwxrwx. 1 root root 22 Jun 28  2017 /sbin/init -> ../lib/systemd/systemd
```
Also if your system has an `/etc/inittab`, it is using `system V`

System V uses runlevels to define the state a serer should boot in.
In every runlevel a specific amount of shell scripts is processed to reach the
state that the system should be in.

There is little standarization in the system V init shell scripts.

### Understand upstart

- Upstart was created by ubuntu and was the first serious attempt to replace system v
- upstart is reactionary, it receives events and runs jobs based on these events
- This makes upstart more flexible
- But still, upstart is using shell scripts and uses many featrues that existed in
init already.
- if your system has an `/etc/init` directory, it is using upstart.

### Systemd
- The goal of the systemd project is to "provide an operating system that runs on top of
the linux kernel"
- It takes care of everything, which on occasions scares experienced linux admins.
- systemd can replace
  - mount etc/fstab
  - cron
  - xinitd
  - spawn containers, so fuck off docker
- Systemd is goal oriented: the administrator defines a target that needs to be reached
and next defines all the needs to be loaded to reach that state un unit files
- Unit files specify loading of services and more, as well as all dependencies that need
to be met and load them.
- To know if the system running is using systemd look for the `/usr/lib/systemd`.


- systend unit files define what needs to be started.
- Diferent types of unit files exist:
  - service: to start a service
  - Mount: to mount a file system
  - timer: to run a process, could replace cron
  - autmount: Automatically 
  - target: group of unit files
  - path
  - And more

- systemd works with different types of dependencies:
  - requires: defines units that must be loaded to loat this unit.
  - wants: typically seen in targets, define which should be loaded but
  loading continues if this fails.
  - requisite: the defined unit must already be active. If its not, systemd fails loading the unit
  - conflict: unit that may never be active when this unit is loaded.
  - before: the current unit will activate before the listed units
  - after: the current unit will activate after the listed unit.


```
cd /usr/lib/systemd/system
cat vsftpd.target

##### Logging

Applications need to log information, they can log in:
- stderr
- own log
- rsyslog
  - centralized
- systemd::journald
  - non-centralized
  - controled with journalctl


`dnf install bash-completion`

To get the log `journalctl` and tab for filters.


#### Starting the userspace environment
When init loads the userspace environment, the following order is roughly applied.
- Starting of essential low-level services such as `udevd`, `syslog` file system mounts.
- Loading of network configuration
- Loading of high-level services, such as cron, printing or web server
- Presenting a login prompt
The order in which this happens is essential; serial loading is required for 
dependencies, sometimes parallel loading can be used to increase speed.

For design reasons the block evices, the minor number is multiple of 16, the a 
max of 15 partitions are allowed in a disk TODO: Why?

This also depends on the file system that the HD is getting.
In MBR (master boot record) we can only have four primary partitions.
One of them could be extened partition which can contain an arbitrary 
number of logical partitions.

GPT the new partition model supports up to 128 primary partitions

