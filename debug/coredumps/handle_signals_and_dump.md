# Handle `SIGSEGV` & dump

Send signal `kill -11 $(pidof app)` and the core will be dumped accordingly
to the `/proc/sys/kernel/core_pattern` or to the settings given by
`sysctl kernel.core_pattern`