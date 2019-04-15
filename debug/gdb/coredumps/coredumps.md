# Coredumps

## Check if coredumps are enabled

Run the command `ulimit -c`, if the result is not unlimited the coredumps are
disabled or limited, enable them with `ulimit -c unlimited` this will remove
restrictions to the size of the coredumps in the _current session_

Now set the location where this coredumps will be stored.
```
cat /proc/sys/kernel/core_pattern
|/usr/libexec/abrt-hook-ccpp %s %c %p %u %g %t %P %I
```

If the output starts with `|`, the core dumps are redirected to the specified
program instead of being saved to the disk. In this case modify the core file
pattern to write the dumps in the directory where the binary was launched.

```
sudo su
echo %e_%p.core > /proc/sys/kernel/core_pattern
```

Then verify the changes with the `cat` command.

## Debug with a coredump

`gdb` expects the next command to set the program in the state of the crash:

```
gdb <program> -c <core_dump>
```

`gdb` will load the system symbols and then the command `bt` can be used or the
command `frame <frame>` to go to a specific frame and the cmd `where` 