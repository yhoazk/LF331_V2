# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
  name = "raw_socket_tx",
  srcs = ["raw_socket.c"],
)

cc_binary(
  name = "raw_socket_rx",
  srcs = ["raw_socket.c"],
  copts = ["-DRX"]
)
