# LD Trick

The `LD_PRELOAD` trick exploits functionality provided by the dynamic linker
on unix systems that allow to tell the linker to bind symbols provided by a
certain shared library _before other libraries_. The OS dynamic loader will
first load dynamic libs you link into the process memory address space such
that the dynamic linker can then resolve symbols at load or run time and bind
them to the defintions. The symbol means, functions, structures, or variables.
The linker is the responsible for resolving symbol references to their defs.

It's possible to provide more than one definition for a symbol, but it needs
to be done carefully to avoid duplicated symbols.


In this example the application `say_hello` waits for your name to geet, and
then prings the result to console. The `my_read.c` will overwrite the read
function to always return the same input.



    LD_PRELOAD

    A whitespace-separated list of additional, user-specified, ELF shared libraries to be loaded before all others. This can be used to selectively override functions in other shared libraries.

## Dynamic Loading and Dynamic Linking