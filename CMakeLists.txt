cmake_minimum_required(VERSION 3.18)
set(CMAKE_TOOLCHAIN_FILE $ENV{DEVKITPRO}/cmake/WiiU.cmake)

project(wiiu-tower-defense LANGUAGES C)

file(GLOB SOURCES "source/*.c" "source/*.h")
add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE -O2 -Wall -Wextra -pedantic)

wut_create_rpx(${PROJECT_NAME})
wut_create_wuhb(${PROJECT_NAME})
