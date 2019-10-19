# Static, Shared Dynamic and loadable Linux Libraries

[.](http://www.yolinux.com/TUTORIALS/LibraryArchives-StaticAndDynamic.html)

Libraries employ a software design also known as "shared components" or "archive
libraries", which groups together multiple objets already compiled in a
single file which are known as libraries.

The benefits of using libraries include:
* Component reuse.
* Version management, Linux libraries can cohabitate old and new versions on a
single system.
* Component specialization.
* Components that are large can be created for dynamic use, thus the library can
remain separate from the executable reducing the size in disk.


### Linux Library types:

There are two linux C/C++ library types:

1. Static libraries (.a)
  - Library of objects which is linked with, and becomes part of the application.
2. Dynamically linked shared object libraries (.so)
  - There is only one form of this library but it can be used in two ways:
    * Dynamically linked at run time.
      * The libraries must be available during compile/link time phase. The shared
      objects are not included into the executable component but are tied to execution.
    * Dinamically loaded/unloaded and linked during execution, using the dynamic
    linking loader system functions.


#### Library naming convention

Libraries are named with the prefix `lib`, When linking, the command line reference
to the library will not contain the library prefix or suffix.

For example the library for threading `libpthread.a` the command to compile is:
```
gcc src-file.c -lm -lpthread
```

This command is referencing two libraries `libm.a` and `libpthread.a`


## Static Libraries : .a

To generate static libraries:

* Compile `cc -Wall -c ctest1.c ctest2.c`
* create library "libctest.a": `ar -cvq libctest.a ctest.o ctest.o`
* List files in library: `ar -t libctest.a`

To link the library `.a` with another file:

* `cc -o <executable_name> prog.c libctest.a`
* `cc -o <executable_name> prog.c -L/path/to/libdir -lctest`

The option `-L<path>` adds that path to the search list, while the option `-l<name>`
is the library target of the search.

The lnux `.a` library is the same as VisualC++ static `.lib` libraries.



## Dynamically Linked *Shared object* Libraries: so
