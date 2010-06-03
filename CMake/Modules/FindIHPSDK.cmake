# - Try to find IHPSDK
# Once done, this will define
#
#  IHPSDK_FOUND - system has IHPSDK
#  IHPSDK_INCLUDE_DIR - the IHPSDK include directories
#  IHPSDK_LIBRARIES - link these to use IHPSDK
#  IHPSDK_LIBRARY_DIR - the IHPSDK library directory
#  IHPSDK_FILES - install these to use IHPSDK

#Find out a name of a significant header
FIND_PATH(IHPSDK_INCLUDE_DIR XiTrocarInterface.h "C:/Xitact/SDK/IHP" "C:/Xitact/IHP" )

#Find out a name of the library.
FIND_LIBRARY(IHPSDK_LIBRARIES XiRobot "C:/Xitact/SDK/IHP" "C:/Xitact/IHP")

#Find out a name of the library path
FIND_PATH(IHPSDK_LIBRARY_DIR XiRobot.lib "C:/Xitact/SDK/IHP" "C:/Xitact/IHP" )

IF(IHPSDK_INCLUDE_DIR AND IHPSDK_LIBRARIES)
    SET(IHPSDK_FOUND TRUE)
ENDIF(IHPSDK_INCLUDE_DIR AND IHPSDK_LIBRARIES)

IF(IHPSDK_FOUND)
    IF(NOT IHPSDK_FIND_QUIETLY)
        MESSAGE(STATUS "Found IHPSDK: ${IHPSDK_LIBRARIES}")
    ENDIF(NOT IHPSDK_FIND_QUIETLY)
ELSE(IHPSDK_FOUND)
    IF(IHPSDK_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find IHPSDK")
    ENDIF(IHPSDK_FIND_REQUIRED)
ENDIF(IHPSDK_FOUND)