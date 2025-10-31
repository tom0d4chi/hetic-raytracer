# CMake configuration for the math module
# This target is intended to be header-only (INTERFACE) by default.
# If you later prefer a compiled static library, change add_library to STATIC
# and move implementations from headers to src/.

add_library(math INTERFACE)

target_include_directories(math INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Optionally export target properties or set compile options here
# target_compile_features(math INTERFACE cxx_std_17)
