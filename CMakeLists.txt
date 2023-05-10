cmake_minimum_required(VERSION 3.15)

project(aioNES)

# set (CMAKE_STATIC_LINKER_FLAGS "-g,-O0")

add_library(aioNES_libretro SHARED src/libretro/libretro.c)
