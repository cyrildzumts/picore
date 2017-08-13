
# usage
# cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain-arm-none-eabi.cmake ../

# The Generic system name is used for embedded targets (targets without OS) in
# CMake
set( CMAKE_SYSTEM_NAME          Generic )
set( CMAKE_SYSTEM_PROCESSOR     BCM2836 )


# The toolchain prefix for all toolchain executables
set(CMAKE_C_COMPILER  arm-none-eabi-gcc)
#set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set (CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_LINKER arm-none-eabi-ld)
set(CMAKE_OBJECTCOPY  arm-none-eabi-objcopy)
set(CMAKE_OBJECTDUMP  arm-none-eabi-objdump)
set( CMAKE_C_OUTPUT_EXTENSION ".o")

set(CMAKE_FIND_ROOT_PATH /usr/lib/arm-none-eabi)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_EXE_LINKER_FLAGS_INIT --specs=nosys.specs CACHE INTERNAL "")

INCLUDE_DIRECTORIES(/usr/lib/arm-none-eabi/include)
#INCLUDE_DIRECTORIES(/usr/lib/arm-none-eabi/include/c++/4.9.3) # needed for C++
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfloat-abi=hard")

# Rapsberry PI 2 Flags
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=neon-vfpv4")
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-a7")

# Rapsberry PI 3 Flags
#set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=neon-fp-armv8")
#set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-a53")

set(CMAKE_FIND_ROOT_PATH /usr/lib/arm-none-eabi)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_EXE_LINKER_FLAGS_INIT --specs=nosys.specs CACHE INTERNAL "")

#set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "" )
set( CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "" )

