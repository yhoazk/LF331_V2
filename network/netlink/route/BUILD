# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
    name = "get_route",
    srcs = ["get_route.c"],
    deps = ["helpers"]
)

# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
    name = "helpers",
    srcs = ["route_helpers.c"],
    hdrs = ["route_helpers.h"]
)