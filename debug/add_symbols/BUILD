# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
    name = "debug_me",
    srcs = ["debug_me.c"],
    copts = ["-g3 -lm"], # there is also an option if only gdb will be used -ggdb
)

## check also the bin utils eu-strip --strip-debug
##https://access.redhat.com/documentation/en-US/Red_Hat_Enterprise_Linux/4/html/Debugging_with_gdb/separate-debug-files.html