# - Try to find VSPSDK
# Once done, this will define
#
#  VSPSDK_FOUND - system has VSPSDK
#  VSPSDK_INCLUDE_DIR - the VSPSDK include directories
#  VSPSDK_LIBRARIES - link these to use VSPSDK
#  VSPSDK_LIBRARY_DIR - the VSPSDK library directory
#  VSPSDK_FILES - install these to use VSPSDK

#Find out a name of a significant header
FIND_PATH(VSPSDK_INCLUDE_DIR XiCathInterface.h "C:/Xitact/SDK/VSP" "C:/Xitact/VSP" )

#Find out a name of the library.
FIND_LIBRARY(VSPSDK_LIBRARIES XiRobotVSP "C:/Xitact/SDK/VSP" "C:/Xitact/VSP")

#Find out a name of the library path
FIND_PATH(VSPSDK_LIBRARY_DIR XiRobotVSP.lib "C:/Xitact/SDK/VSP" "C:/Xitact/VSP" )

IF(VSPSDK_INCLUDE_DIR AND VSPSDK_LIBRARIES)
    SET(VSPSDK_FOUND TRUE)
ENDIF(VSPSDK_INCLUDE_DIR AND VSPSDK_LIBRARIES)

IF(VSPSDK_FOUND)
    IF(NOT VSPSDK_FIND_QUIETLY)
        MESSAGE(STATUS "Found VSPSDK: ${VSPSDK_LIBRARIES}")
    ENDIF(NOT VSPSDK_FIND_QUIETLY)
ELSE(VSPSDK_FOUND)
    IF(VSPSDK_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find VSPSDK")
    ENDIF(VSPSDK_FIND_REQUIRED)
ENDIF(VSPSDK_FOUND)