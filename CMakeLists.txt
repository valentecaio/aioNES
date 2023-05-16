cmake_minimum_required(VERSION 3.15)

project(aioNES)

# set (CMAKE_STATIC_LINKER_FLAGS "-g,-O0")

file(GLOB SRC
    "src/*.h"
    "src/*.c"
)

file(GLOB TEST
    "src/test/*.h"
    "src/test/*.c"
)

add_library(aioNES_libretro SHARED src/libretro/libretro.c ${SRC} ${TEST})
