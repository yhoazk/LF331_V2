

cc_library(
    name = "hw_config",
    #visibility = [""],
    srcs = ["src/hw_config/hw_config.cpp"],
    hdrs = ["src/hw_config/hw_config.h"],
    strip_include_prefix = "src/hw_config"
)

cc_library(
    name = "rx_ts",
    #visibility = [""],
    srcs = ["src/rx_ts/rx_ts.cpp"],
    hdrs = ["src/rx_ts/rx_ts.h"],
    strip_include_prefix = "src/rx_ts"
)

cc_library(
    name = "tx_ts",
    #visibility = [""],
    srcs = ["src/tx_ts/tx_ts.cpp"],
    hdrs = ["src/tx_ts/tx_ts.h"],
    strip_include_prefix = "src/tx_ts"
)

cc_test(
    name = "net_test",
    srcs = glob(["test/*.cpp"]),
    deps = ["@gtest//:gtest", "@gtest//:gtest_main"]
)

cc_binary(
    name = "example",
    srcs = ["src/example.cpp"],
    copts = ["-std=c++17"],
    deps = [":hw_config", ":tx_ts", "rx_ts"]
)
