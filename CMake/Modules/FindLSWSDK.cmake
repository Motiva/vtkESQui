# - Try to find LSWSDK
# Once done, this will define
#
#  LSWSDK_FOUND - system has LSWSDK
#  LSWSDK_INCLUDE_DIR - the LSWSDK include directories
#  LSWSDK_LIBRARIES - link these to use LSWSDK
#  LSWSDK_LIBRARY_DIR - the LSWSDK library directory
#  LSWSDK_FILES - install these to use LSWSDK

#Find out a name of a significant header
FIND_PATH(LSWSDK_INCLUDE_DIR ImmSurgicalWorkstation.h "C:/Immersion/include" "C:/Immersion/LSWSDK/include" )

#Find out a name of the library.
FIND_LIBRARY(LSWSDK_LIBRARIES ImmLSW "C:/Immersion/lib" "C:/Immersion/LSWSDK/lib" "C:/Immersion/bin" "C:/Immersion/LSWSDK/bin")

#Find out a name of the library path
FIND_PATH(LSWSDK_LIBRARY_DIR ImmLSW.lib "C:/Immersion/lib" "C:/Immersion/LSWSDK/lib" "C:/Immersion/bin" "C:/Immersion/LSWSDK/bin" )

IF(LSWSDK_INCLUDE_DIR AND LSWSDK_LIBRARIES)
    SET(LSWSDK_FOUND TRUE)
ENDIF(LSWSDK_INCLUDE_DIR AND LSWSDK_LIBRARIES)

IF(LSWSDK_FOUND)
    IF(NOT LSWSDK_FIND_QUIETLY)
        MESSAGE(STATUS "Found LSWSDK: ${LSWSDK_LIBRARIES}")
    ENDIF(NOT LSWSDK_FIND_QUIETLY)
ELSE(LSWSDK_FOUND)
    IF(LSWSDK_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find LSWSDK")
    ENDIF(LSWSDK_FIND_REQUIRED)
ENDIF(LSWSDK_FOUND)