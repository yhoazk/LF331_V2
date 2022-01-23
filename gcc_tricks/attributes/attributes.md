# Compiler specific `__attributes__`

## `__attribute__((format))` function attribute

this attribute causes the compiler to check that the arguments are in the correct
format for the specified function.

## syntax

`__attribute__((format(function, string-index, first-to-check)))`
where `function` is a printf style function `string-index` specifies the index if the string argument in the function (starting from one) `first-to-check` is the index of the first argument to check against the format string.


