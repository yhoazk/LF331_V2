cc_library(
   name = "serial" ,
   srcs = ["serial.cpp"],
   hdrs = ["serial.hpp"]
)


cc_binary(
    name = "serial_user",
    srcs = ["serial_user.cpp"],
    deps = [":serial"]
)