load("@rules_qt//:qt.bzl", "qt_cc_binary")

qt_cc_binary(
    name = "app",
    srcs = ["main.cpp"],
    deps = [
        "@rules_qt//:qt_core",
        "@rules_qt//:qt_widgets",
    ],
    copts = ["-std=c++20"],
)

