load("@rules_qt//:qt.bzl", "qt_cc_binary", "qt_ui_library", "qt_cc_library")
load("@pip//:requirements.bzl", "requirement")

load("@rules_python//python:pip.bzl", "compile_pip_requirements")

py_binary(
    name = "script",
    srcs = ["script.py"],
    deps = [requirement("gradio-client")],
    data = [":requirements"]
)

qt_cc_binary(
    name = "visualizer",
    srcs = ["main.cpp", ],
    deps = [
        "@rules_qt//:qt_core",
        "@rules_qt//:qt_widgets",
    ],
    data = [":script"],
    copts = ["-std=c++20",],
)

