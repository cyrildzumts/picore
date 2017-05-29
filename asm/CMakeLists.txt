
project(startup  ASM)
cmake_minimum_required(VERSION 2.8)

SET(CMAKE_ASM_FLAGS ${CFLAGS} )

add_library(${PROJECT_NAME}  startup.s)


