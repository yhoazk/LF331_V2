## Get the configuration of the kernel 

To check what options were configured in the kernel when compiled, `.config` file sort-of.
There are two options:

- `zcat /proc/config.gz`
- `/usr/src/linux/scripts/extract-ikconfig `

This two options depend on the configuration itself, so may or may not be present in the
bzImage
