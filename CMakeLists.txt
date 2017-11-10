project(PiCore C ASM)
cmake_minimum_required(VERSION 2.8)
set(PICORE_MAJOR_VERSION 0)
set(PICORE_MINOR_VERSION 7)
set(PICORE_VERSION ${PICORE_MAJOR_VERSION}.${PICORE_MINOR_VERSION})
set (version 0.7)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mthumb-interwork")
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2" )
#set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -c" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall" )
##set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostdlib" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostartfiles" )
#set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffreestanding" )
set(CMAKE_LINKER arm-none-eabi-ld)
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
set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} include)
include_directories(include)
#INCLUDE_DIRECTORIES(/usr/lib/arm-none-eabi/include)
file(GLOB SOURCES startup.S cstubs.c gpio.c mu_uart.c
    monitor.c hypervisor.c supervisor.c  init.c
    tft.c raio8870.c aux_spi.c spi.c timer.c system.c cpu.c i2c.c ds1621.c "include/*h")
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} libm.so)
add_executable(${TARGET_EXE} ${SOURCES} main.c  modules.c )
target_link_libraries(${TARGET_EXE} ${PROJECT_NAME})
add_custom_command(
    TARGET ${TARGET_EXE} POST_BUILD
    COMMAND ${CMAKE_OBJECTCOPY} ${CMAKE_BINARY_DIR}/bin/${TARGET_EXE} -O binary ${CMAKE_BINARY_DIR}/kernel.img
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Convert the ELF output file to a binary image" )
# create automatically a dump file of the executable
add_custom_command(TARGET ${TARGET_EXE} POST_BUILD
COMMAND ${CMAKE_OBJECTDUMP} -D ${CMAKE_BINARY_DIR}/bin/${TARGET_EXE} > kernel.dump
WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
COMMENT "Dumping the ELF output file done !" )


#Install and export targets.
install (
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${PROJECT_BINARY_DIR}/usr/include/picore-${version}
    FILES_MATCHING PATTERN "*.h")
#install(FILES "include/*.h" DESTINATION ${PiCore_BINARY_DIR}/include/picore-${version})
install(TARGETS ${PROJECT_NAME}
        DESTINATION ${PiCore_BINARY_DIR}/usr/lib/picore-${version}
        EXPORT picore-targets)
install(EXPORT picore-targets DESTINATION ${PiCore_BINARY_DIR}/usr/lib/picore-${version})
# Make project useable from build tree
export(TARGETS ${PROJECT_NAME} FILE picore-targets.cmake)
configure_file(${PiCore_SOURCE_DIR}/pkg/picore-config.cmake.in
               ${PiCore_BINARY_DIR}/pkg/picore-config.cmake @ONLY)
           # create and install package configuration add version files.

configure_file(${PiCore_SOURCE_DIR}/pkg/picore-config-version.cmake.in
               ${PiCore_BINARY_DIR}/pkg/picore-config-version.cmake @ONLY)
#install(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_BINARY_DIR}/lib/picore/generators EXPORT picore-targets)
#install(EXPORT picore-targets DESTINATION ${PROJECT_BINARY_DIR}/lib/picore)
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${PiCore_BINARY_DIR}/usr/bin
        LIBRARY DESTINATION ${PiCore_BINARY_DIR}/usr/lib/shared
        ARCHIVE DESTINATION ${PiCore_BINARY_DIR}/usr/lib/static
        )





install (FILES ${PiCore_BINARY_DIR}/pkg/picore-config.cmake
               ${PiCore_BINARY_DIR}/pkg/picore-config-version.cmake
               DESTINATION ${PiCore_BINARY_DIR}/usr/lib/picore-${version})
