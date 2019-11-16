# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
    name = "is_palindrome",
    srcs = ["is_palindrome.cpp"],
    hdrs = ["is_palindrome.h"],
)

# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
    name = "test_is_palindrome",
    srcs = ["test_shared.cpp"],
    deps = [":is_palindrome"],
    linkshared = True
)