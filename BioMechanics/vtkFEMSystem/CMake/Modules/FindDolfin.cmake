# - Try to find Dolfin
# Once done, this will define
#
#  DOLFIN_FOUND - system has DOLFIN
#  DOLFIN_INCLUDE_DIR - the DOLFIN include directories
#  DOLFIN_LIBRARY - link these to use DOLFIN
#  DOLFIN_LIBRARY_DIR - the DOLFIN library directory
#  DOLFIN_FILES - install these to use DOLFIN

#Find out a name of a significant header
FIND_PATH(DOLFIN_INCLUDE_DIR dolfin.h "/usr/local/include" )

#Find out a name of the library.
FIND_LIBRARY(DOLFIN_LIBRARY dolfin "/usr/local/lib")

#Find out a name of the library path
FIND_PATH(DOLFIN_LIBRARY_DIR dolfin "/usr/local/lib" )

IF(DOLFIN_INCLUDE_DIR AND DOLFIN_LIBRARY)
    SET(DOLFIN_FOUND TRUE)
ENDIF(DOLFIN_INCLUDE_DIR AND DOLFIN_LIBRARY)

IF(DOLFIN_FOUND)
    IF(NOT DOLFIN_FIND_QUIETLY)
        MESSAGE(STATUS "Found DOLFIN: ${DOLFIN_LIBRARY}")
    ENDIF(NOT DOLFIN_FIND_QUIETLY)
ELSE(DOLFIN_FOUND)
    IF(DOLFIN_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find DOLFIN")
    ENDIF(DOLFIN_FIND_REQUIRED)
ENDIF(DOLFIN_FOUND)
