# Non blocking IO

I/O operations like `read()` `write()` block until I/O becomes possible.
But there is the use case when we want to check if the IO is possible with out
blocking if is not. Also should be possible to monitor multiple files to see if
IO operations are possible in any of them.

For this we need to set the flag `O_NONBLOCK` in the file descriptor. For this
the function `fcntl` is used. We can poll/select to monitor several files for 
IO operations but that is wasteful. There are other options that ask to the 
kernel to notify with signals when the IO is possible on the `fd`.

Other options is use `epoll` which monitors multiple files like select*poll,
but it has better performance and is easier to program than a signal-driven IO.