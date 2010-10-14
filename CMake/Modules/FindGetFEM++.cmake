# - Try to find getFEM
# Once done, this will define
#
#  GETFEM_FOUND - system has GETFEM
#  GETFEM_INCLUDE_DIR - the GETFEM include directories
#  GETFEM_LIBRARY - link these to use GETFEM
#  GETFEM_LIBRARY_DIR - the GETFEM library directory
#  GETFEM_FILES - install these to use GETFEM

#Find out a name of a significant header
FIND_PATH(GETFEM_INCLUDE_DIR getfem_mesh.h "/usr/local/include/getfem" )

#Find out a name of the library.
FIND_LIBRARY(GETFEM_LIBRARY getfem "/usr/local/lib")

#Find out a name of the library path
FIND_PATH(GETFEM_LIBRARY_DIR getfem "/usr/local/lib" )

IF(GETFEM_INCLUDE_DIR AND GETFEM_LIBRARY)
    SET(GETFEM_FOUND TRUE)
ENDIF(GETFEM_INCLUDE_DIR AND GETFEM_LIBRARY)

IF(GETFEM_FOUND)
    IF(NOT GETFEM_FIND_QUIETLY)
        MESSAGE(STATUS "Found GETFEM: ${GETFEM_LIBRARY}")
    ENDIF(NOT GETFEM_FIND_QUIETLY)
ELSE(GETFEM_FOUND)
    IF(GETFEM_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find GETFEM")
    ENDIF(GETFEM_FIND_REQUIRED)
ENDIF(GETFEM_FOUND)
