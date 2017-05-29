
project(APP C )

# Add the raspberry-pi 2 definition so conditional compilation works
add_definitions( -DRPI2=1 )
#set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O1" )
#set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall" )
#set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g " )
#set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostdlib" )
#set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostartfiles" )
#set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=neon-vfpv4" )
#set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfloat-abi=hard" )
set( CMAKE_EXE_LINKER_FLAGS "-Wl,-T,${PiCore_SOURCE_DIR}/picore.ld" )
include_directories(include)
# INCLUDE_DIRECTORIES(/usr/lib/arm-none-eabi/include)
# add boot source file : boot.s and cstubs.c

set(EXECUTABLE_OUTPUT_PATH
    ${PROJECT_BINARY_DIR}/bin
    CACHE PATH
    "Single directory for all libraries")
set(LIBRARY_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/lib
    CACHE PATH
    "Single directory for all libraries")
file(GLOB SOURCES "src/*.c" "include/*.h" )

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PiCore)
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME} -O binary ${CMAKE_BINARY_DIR}/kernel.img
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Convert the ELF output file to a binary image" )
