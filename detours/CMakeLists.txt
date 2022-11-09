# Detours
add_library(detours STATIC
        creatwth.cpp
        detours.cpp
        detours.h
        detver.h
        disasm.cpp
        disolarm.cpp
        disolarm64.cpp
        disolia64.cpp
        disolx64.cpp
        disolx86.cpp
        image.cpp
        modules.cpp
        uimports.cpp)

# This file is included and not compiled on its own
set_property (
    SOURCE uimports.cpp
    APPEND PROPERTY HEADER_FILE_ONLY true)
# complie options
target_compile_options(detours PRIVATE /W4 /WX /Zi /MT /Gy /Gm- /Zl /Od)
# add include directories
target_include_directories(detours PUBLIC ./)
