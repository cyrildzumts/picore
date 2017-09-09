#
# Find the PiCore includes and library
#

# This module defines :
# PICORE_INCLUDE_DIR, where to find picore headers
# PICORE_LIBRARIES, the libraries to link against to use PICORE
# PICORE_DEFINITIONS - You should call add_definitions(${PICORE_DEFINITIONS})
# before compiling code that include picore library file
# PICORE_FOUND, if false , do not try to use PICORE

# PICORE_LIBRARY, where to find picore library
set(PICORE_PATH "$ENV{HOME}/Project/picore" )
set(PICORE_BUILD_DIR "${PICORE_PATH}/build")
message("Find  Picore module called")
message("Picore Lib info")
message("Picore directory : " ${PICORE_PATH})
message("Search Picore lib in directory : " ${PICORE_BUILD_DIR})
find_library(PICORE_LIBRARY PiCore PATHS ${PICORE_BUILD_DIR})
if(PICORE_LIBRARY)
   set(PICORE_INCLUDE_DIR "${PICORE_PATH}/include")
   set(PICORE_LIBRARIES ${PICORE_LIBRARY})
   set(PICORE_FOUND "YES")
   message("Picore Library found")
else (PICORE_LIBRARY)
message("Picore lib could not be found")
endif()
message("Quitting Find Picore module now")
