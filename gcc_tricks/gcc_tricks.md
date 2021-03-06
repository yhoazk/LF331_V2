# GCC tricks and opts

[src](https://gcc.gnu.org/onlinedocs/gcc/Loop-Specific-Pragmas.html#index-pragma-GCC-unroll-n)

Fro cuda there is the option to unroll a loop with the option `#pragma unroll`
for gcc there are several options to insruct the compiler to unroll them.


With this pragma the developer asserts that there are no loop-carried
dependencies which prevent consecutve interations of the following loop from
executing in with SIMD instructions.

```c
void foo(int n, int* a, int* b, int* c){
  int i, j;
  #pragma GCC ivdep
  for(i=0, i<0; ++i){
    a[i] = b[i] + c[i];
  }
}
```

The pragma `#pragma GCC unroll n` controls how many times the loop should be
unrolled. This must be placed inmediatelly before `for`, `while` or `do` or a
`#pragma GCC ivdep` and applies only to the next loop. `n` is a integer constant
specifying the unrolling factor. The values 0 and 1 block any unrolling.



## Add enter/exit functions

[gcc](https://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html#index-finstrument_002dfunctions-2112)
will call a function on every other function call, it could be useful to
debug or track calls also supported in C++.
This is enabled with the flag `-finstrument-functions`.

Then the entry and exit functions need to be implemented:

- `void __cyg_profile_func_enter(void* this_fn, void* call_site)`:
- `void __cyg_profile_func_exit(void* this_fn, void* call_site)`:

To avoid instrument the instrumented functions an attribute needs to be added:
`__attribute__((no_instrument_function))` also add this attribute to the main fnc.

## Show include tree

Helps to check what are the files to be included by all the headers, helps to
visualize and prolly improve the header structure

```
cat > show_me.c
#include <linux/mmc/ioclt.h>
^D
gcc -M show_me.c
show_me.o: show_me.c /usr/include/stdc-predef.h \
 /usr/include/linux/mmc/ioctl.h /usr/include/linux/types.h \
 /usr/include/asm/types.h /usr/include/asm-generic/types.h \
 /usr/include/asm-generic/int-ll64.h /usr/include/asm/bitsperlong.h \
 /usr/include/asm-generic/bitsperlong.h /usr/include/linux/posix_types.h \
 /usr/include/linux/stddef.h /usr/include/asm/posix_types.h \
 /usr/include/asm/posix_types_64.h /usr/include/asm-generic/posix_types.h
```
