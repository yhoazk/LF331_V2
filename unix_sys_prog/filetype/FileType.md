# testing the type of a file

https://www.gnu.org/software/libc/manual/html_node/Testing-File-Type.html#Testing-File-Type

Available checks:

- `S_ISDIR`: Directory
- `S_ISCHR`: Char file
- `S_ISBLK`: Block file
- `S_ISREG`: Regular file
- `S_ISFIFO`: Fifo file
- `S_ISLNK`: Symbolic link
- `S_ISSOCK`: Socket

Each return a non-zero value if the check is true for the file type.
