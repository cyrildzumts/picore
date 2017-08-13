project(PiCore C ASM)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mthumb-interwork")
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0" )
#set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -c" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall" )
#set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostdlib" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostartfiles" )
set( CMAKE_EXE_LINKER_FLAGS "-Wl,-T,${PROJECT_SOURCE_DIR}/picore.ld" )
set(EXECUTABLE_OUTPUT_PATH
    ${PROJECT_BINARY_DIR}/bin
    CACHE PATH
    "Single directory for all libraries")
set(LIBRARY_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/lib
    CACHE PATH
    "Single directory for all libraries")

set(TARGET_EXE picore_app)
# add subdirectory
include_directories(include)
INCLUDE_DIRECTORIES(/usr/lib/arm-none-eabi/include)
file(GLOB SOURCES startup.s cstubs.c init.c
    gpio.c mu_uart.c tft.c
    raio8870.c spi.c timer.c system.c i2c.c ds1621.c "include/*h")
add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_executable(${TARGET_EXE} ${SOURCES} main.c  modules.c )
# target_link_libraries(${TARGET_EXE} ${PROJECT_NAME})

add_custom_command(
    TARGET ${TARGET_EXE} POST_BUILD
    COMMAND ${CMAKE_OBJECTCOPY} ${CMAKE_BINARY_DIR}/bin/${TARGET_EXE} -O binary ${CMAKE_BINARY_DIR}/kernel.img
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Convert the ELF output file to a binary image" )
# create automatically a dump file of the executable
add_custom_command(TARGET ${TARGET_EXE} POST_BUILD
COMMAND ${CMAKE_OBJECTDUMP} -d ${CMAKE_BINARY_DIR}/bin/${TARGET_EXE} > kernel.dump
WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
COMMENT "Dumping the ELF output file done !" )
install(TARGETS ${PROJECT_NAME}
        DESTINATION ${PROJECT_BINARY_DIR}/lib/picore/generators
        EXPORT picore-targets)
install(EXPORT picore-targets DESTINATION ${PROJECT_BINARY_DIR}/lib/picore)
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin
        LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/lib/shared
        ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}/lib/static
        )


