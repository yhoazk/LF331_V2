# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
  name = "loop_unroll",
  srcs = ["loop_unroll/loop_unroll.c"],
)

# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
  name = "instrumented_calls",
  srcs = ["instrumented_calls/instrumented.c"],
  copts = ["-finstrument-functions"],
)