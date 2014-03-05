# - Try to find SBMSDK
# Once done, this will define
#
#  SBMSDK_FOUND - system has SBMSDK
#  SBMSDK_INCLUDE_DIR - the SBMSDK include directories
#  SBMSDK_LIBRARIES - link these to use SBMSDK
#  SBMSDK_LIBRARY_DIR - the SBMSDK library directory
#  SBMSDK_FILES - install these to use SBMSDK

#Find out a name of a significant header
FIND_PATH(SBMSDK_INCLUDE_DIR SimballMedicalHID.h "C:/Simball")

#Find out a name of the library.
FIND_LIBRARY(SBMSDK_LIBRARIES SimballMedicalHID "C:/Simball")

#Find out a name of the library path
FIND_PATH(SBMSDK_LIBRARY_DIR SimballMedicalHID.lib "C:/Simball")

IF(SBMSDK_INCLUDE_DIR AND SBMSDK_LIBRARIES)
    SET(SBMSDK_FOUND TRUE)
ENDIF(SBMSDK_INCLUDE_DIR AND SBMSDK_LIBRARIES)

IF(SBMSDK_FOUND)
    IF(NOT SBMSDK_FIND_QUIETLY)
        MESSAGE(STATUS "Found SBMSDK: ${SBMSDK_LIBRARIES}")
    ENDIF(NOT SBMSDK_FIND_QUIETLY)
ELSE(SBMSDK_FOUND)
    IF(SBMSDK_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find SBMSDK")
    ENDIF(SBMSDK_FIND_REQUIRED)
ENDIF(SBMSDK_FOUND)