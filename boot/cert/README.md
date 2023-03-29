

Create a certificate


The script as it is at this commit, takes the read-only sections.
Even when the linker script specifies the read only section, the compiler
also includes a readonly section. Then `objcopy` is instructed to copy
the read only, which takes *all* read only sections only the ones we
have specified:




```
build:
	gcc cert.c -c
	ld cert.o -T cert.ld -o cert.bin
	objcopy -O binary cert.bin cert_obj.bin

clean:
	rm *.bin *.o

```
Dumping the `cert.o` file:

```
objdump -h cert.o
cert.o:     file format elf64-x86-64

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .text         00000000  0000000000000000  0000000000000000  00000040  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, CODE                           << ro
  1 .data         00000000  0000000000000000  0000000000000000  00000040  2**0
                  CONTENTS, ALLOC, LOAD, DATA
  2 .bss          00000000  0000000000000000  0000000000000000  00000040  2**0
                  ALLOC
  3 .magic_1      00000004  0000000000000000  0000000000000000  00000040  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  4 .magic_2      00000004  0000000000000000  0000000000000000  00000044  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  5 .comment      0000002c  0000000000000000  0000000000000000  00000048  2**0
                  CONTENTS, READONLY                                              << RO
  6 .note.GNU-stack 00000000  0000000000000000  0000000000000000  00000074  2**0
                  CONTENTS, READONLY                                              << RO   
  7 .note.gnu.property 00000020  0000000000000000  0000000000000000  00000078  2**3 
                  CONTENTS, ALLOC, LOAD, READONLY, DATA                          << RO
```


Then is needed to remove the sections we are not interested in:
```
objcopy -O binary --remove-section=.note.* cert.bin cert_bin.bin
```
