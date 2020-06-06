# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
    name = "ptp_capture",
    srcs = ["ptp_capture.cpp"],
    # copts = [],
    deps = [":ptp_cap"],
)

# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
    name = "ptp_cap",
    srcs = ["lib_ptp/ptp_cap.cpp"],
    hdrs = ["lib_ptp/ptp_cap.h"],
)