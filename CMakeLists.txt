cmake_minimum_required(VERSION 3.18)
set(CMAKE_TOOLCHAIN_FILE $ENV{DEVKITPRO}/cmake/WiiU.cmake)

project(wiiu-tower-defense LANGUAGES C)

add_executable(wiiu-tower-defense main.c)

target_compile_options(wiiu-tower-defense PRIVATE -Wall -Wextra -pedantic)

wut_create_rpx(wiiu-tower-defense)
wut_create_wuhb(wiiu-tower-defense)
