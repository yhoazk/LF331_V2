# Loop Unrolling

Note that unrolling loops does not always improves performance due to 
instruction cache<sup>(aka i-cache)</> misses. This could help the avoid
flushing the branch prediction and the expected flush of the instruction
pipeline at the end of the current iteration of the loop.

Thes function can be enabled in gcc with pragmas or globally with the option `-funroll-all-loops`


## Loop unrolling vs Duff Device

