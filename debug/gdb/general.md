# GDB

Use intel notation, the `%` and `$` is the AT&T notation, which is the default.
```
(gdb) set disassembly-flavor intel
```

## Writing the executable while debugging

To enable this option, pass the flag `--write` when invoking gdb. It's also
possible to enable this once inside the gdb cli: `set write on`. The current
status can be verified with `show write`

Example:
```c
unsigned k;
int main(){
  volatile unsigned j = 1;
  for(k=0; k < 10; ++k){
    j += 2; // just to avoid optimization
  }
  return j;
}
```

Commands in gdb:

> After mofidication, the next command needs to be quit. Any other command
> will discard the changes.

```
(gdb) disas/r main
   0x00000000004004da <+4>:     c7 45 fc 01 00 00 00    movl   $0x1,-0x4(%rbp)
   0x00000000004004e1 <+11>:    c7 05 3d 0b 20 00 00 00 00 00   movl   $0x0,0x200b3d(%rip)        # 0x601028 <k>
   0x00000000004004eb <+21>:    eb 18   jmp    0x400505 <main+47>
   0x00000000004004ed <+23>:    8b 45 fc        mov    -0x4(%rbp),%eax
   0x00000000004004f0 <+26>:    83 c0 02        add    $0x2,%eax
   0x00000000004004f3 <+29>:    89 45 fc        mov    %eax,-0x4(%rbp)
   0x00000000004004f6 <+32>:    8b 05 2c 0b 20 00       mov    0x200b2c(%rip),%eax        # 0x601028 <k>
   0x00000000004004fc <+38>:    83 c0 01        add    $0x1,%eax
   0x00000000004004ff <+41>:    89 05 23 0b 20 00       mov    %eax,0x200b23(%rip)        # 0x601028 <k>
   0x0000000000400505 <+47>:    8b 05 1d 0b 20 00       mov    0x200b1d(%rip),%eax        # 0x601028 <k>
   0x000000000040050b <+53>:    83 f8 09        cmp    $0x9,%eax
   0x000000000040050e <+56>:    76 dd   jbe    0x4004ed <main+23>
   0x0000000000400510 <+58>:    8b 45 fc        mov    -0x4(%rbp),%eax
   0x0000000000400513 <+61>:    5d      pop    %rbp
```

in the offset +53 is where the loop comparison is being made. We can change this 
value with the `set` operand and giving the content of the address.
Change the loop to run until 0x20. The instruction is in `0x000000000040050b`
If we change the addressign to `unsigned char` the opcode is in `0x000000000040050b`
and the 9 which is part of the comparison is in `0x000000000040050b + 2` the offset
is from left to right on little endian (?).

```
(gdb) set {unsigned char}0x000000000040050d = 20
(gdb) disas/r main
Dump of assembler code for function main:
   0x00000000004004d6 <+0>:     55      push   %rbp
   0x00000000004004d7 <+1>:     48 89 e5        mov    %rsp,%rbp
   0x00000000004004da <+4>:     c7 45 fc 01 00 00 00    movl   $0x1,-0x4(%rbp)
   0x00000000004004e1 <+11>:    c7 05 3d 0b 20 00 00 00 00 00   movl   $0x0,0x200b3d(%rip)        # 0x601028 <k>
   0x00000000004004eb <+21>:    eb 18   jmp    0x400505 <main+47>
   0x00000000004004ed <+23>:    8b 45 fc        mov    -0x4(%rbp),%eax
   0x00000000004004f0 <+26>:    83 c0 02        add    $0x2,%eax
   0x00000000004004f3 <+29>:    89 45 fc        mov    %eax,-0x4(%rbp)
   0x00000000004004f6 <+32>:    8b 05 2c 0b 20 00       mov    0x200b2c(%rip),%eax        # 0x601028 <k>
   0x00000000004004fc <+38>:    83 c0 01        add    $0x1,%eax
   0x00000000004004ff <+41>:    89 05 23 0b 20 00       mov    %eax,0x200b23(%rip)        # 0x601028 <k>
   0x0000000000400505 <+47>:    8b 05 1d 0b 20 00       mov    0x200b1d(%rip),%eax        # 0x601028 <k>
   0x000000000040050b <+53>:    83 f8 14        cmp    $0x14,%eax
   0x000000000040050e <+56>:    76 dd   jbe    0x4004ed <main+23>
   0x0000000000400510 <+58>:    8b 45 fc        mov    -0x4(%rbp),%eax
   0x0000000000400513 <+61>:    5d      pop    %rbp
   0x0000000000400514 <+62>:    c3      retq
End of assembler dump.set {unsigned char}0x000000000040050b
```

### `.gdbinit`

This file acts like `.bashrc` it gets executed before we get the gdb console it
can be disabled by passing the flag `-nx` for the local `./.gdbinit` and `-nh`
for the global user `~/.gdbinit` when invoking gdb.

Example of a `.gdbinit`
```
# Comments are supported with
# Always use intel disassembly
set disassembly-flavor intel
# Store gdb history
set history filename ~/.gdb_history
#
set history save

```

In newer versions, only the init file in the user `$HOME` is parsed, not project local.

### Breakpoints, watchpoints, catchpoints and tracepoints


Save the inspection points:
```
(gdb) save breakpoints <filename>
```

Restore them:
```
(gdb) source <filename>
```


[src](https://sourceware.org/gdb/current/onlinedocs/gdb/)