# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
  name = "say_hello",
  srcs = ["src/say_hello.cpp"],
  copts = [],
  deps = [],
  linkstatic = False,
)


# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
  name = "my_read",
  srcs = ["my_read/my_read.cpp"],
  # hdrs = [],
  # deps = [],
  copts = ["-fPIC", "-shared"]
)