# $PWD vs `pwd`


What is the difference between `export PWD=/tmp` and `cd /tmp`?
The cd calls the `chdir` system call, which according to the manual changes the
current working directory.

This still does not answers the question.



## SLAB Allocation

Slab allocation is a memory management mechanism intended for the efficient
memory allocation of kernel objects. 


## cache coloring


Starting a bash console without any information on the partent process.

`env -i bash --norc --noprofile` 

- `env -i` Ignores any enviroment variable
- `bash --norc --noprofile`: Starts a bash without reading the rc file.

The `env -i` is needed as the bash command and its flags still inherit from the starting process.


