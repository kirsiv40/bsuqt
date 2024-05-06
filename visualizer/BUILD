load("@rules_qt//:qt.bzl", "qt_cc_binary", "qt_ui_library", "qt_cc_library")
load("@pip//:requirements.bzl", "requirement")

py_binary(
    name = "script",
    srcs = ["script.py"],
    deps = [requirement("gradio-client")],
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

# load("@rules_python//python:pip.bzl", "compile_pip_requirements")

# compile_pip_requirements(
#     name = "requirements",
#     src = "requirements.",
#     requirements_txt = "requirements_lock.txt",
# )