cmake_minimum_required (VERSION 3.8)

add_library(Quick-Access-Extended SHARED "ShellEX.cpp" "ShellEX.h" "Selector.cpp" "Selector.h" "Handler.cpp" "Handler.h" "Utilities.h" "GUID.h" "Defenitions.h" "C-Linker.h" "ShellEX.def")

install(TARGETS Quick-Access-Extended DESTINATION ${CMAKE_BINARY_DIR}/bin)