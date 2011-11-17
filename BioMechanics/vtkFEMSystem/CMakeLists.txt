CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
IF(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
  CMAKE_POLICY(SET CMP0011 NEW)
ENDIF(COMMAND CMAKE_POLICY)

# Set the project/library name here.  Classes should be declared as
# "class vtkFEMSystem_EXPORT vtkFoo", where vtkFEMSystem is the name of the
# project set here.
PROJECT(vtkFEMSystem)

ENABLE_TESTING()

#
#Enabling/disabling FEM use
#
OPTION (USE_GETFEM "Set this option if you want to enable FEM")

#
#Enabling/disabling DOLFIN use
#
OPTION (USE_DOLFIN "Set this option if you want to enable DOLFIN")

#
# Expand CMAKE_MODULE_PATH
#
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMake/Modules/")

IF(USE_GETFEM)
  FIND_PACKAGE(GetFEM++ REQUIRED)
  INCLUDE_DIRECTORIES(${GETFEM_INCLUDE_DIRS})
ENDIF(USE_GETFEM)

IF(USE_DOLFIN)
  FIND_PACKAGE(Dolfin REQUIRED)
  INCLUDE_DIRECTORIES(${DOLFIN_INCLUDE_DIRS})
ENDIF(USE_DOLFIN)


# Set your list of sources here. Do not change the name of the
# vtkFEMSystem_SRCS variable.
SET(vtkFEMSystem_SRCS
  vtkFEMSystem.cxx
)

IF(GETFEM_FOUND)
    SET(vtkFEMSystem_SRCS ${vtkFEMSystem_SRCS} vtkGetFEMInterface.cxx)
ENDIF(GETFEM_FOUND)

IF(DOLFIN_FOUND)
    SET(vtkFEMSystem_SRCS ${vtkFEMSystem_SRCS} vtkDolfinInterface.cxx)
ENDIF(DOLFIN_FOUND)

#SET_SOURCE_FILES_PROPERTIES(
#  ABSTRACT
#  )

#SET_SOURCE_FILES_PROPERTIES(
#  vtkVelocityVerletSolver.cxx
#  PROPERTIES
#  WRAP_EXCLUDE 1
#  )

# List the kits from VTK that are needed by this project.
SET(vtkFEMSystem_LIBS
  vtkCommon
  vtkFiltering
  vtkRendering
  vtkGraphics
  vtkIO
)

#-----------------------------------------------------------------------------
# Most users should not need to change anything below this line.

# Need to include class headers and the configuration header.
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})

# Setup output paths.
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin CACHE PATH
    "Single output directory for building all libraries.")
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin CACHE PATH
    "Single output directory for building all executables.")
MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)

# Find and load VTK settings.
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

# Give user option of building shared or static libraries.  Default to
# the choice made for VTK.
OPTION(BUILD_SHARED_LIBS "Build with shared libraries."
       ${VTK_BUILD_SHARED_LIBS})

# Set VTKLOCAL_BUILD_SHARED_LIBS to 1 or 0 for use in creating the
# configuration header.
IF(BUILD_SHARED_LIBS)
  SET(VTKLOCAL_BUILD_SHARED_LIBS_CONFIG 1)
ELSE(BUILD_SHARED_LIBS)
  SET(VTKLOCAL_BUILD_SHARED_LIBS_CONFIG 0)
ENDIF(BUILD_SHARED_LIBS)

IF(VTK_FOUND)
  # If this is a build tree, provide an option for putting this
  # project's executables and libraries in with VTK's.
  IF (EXISTS ${VTK_DIR}/bin)
    OPTION(USE_VTK_OUTPUT_PATHS
    "Use VTK's output directory for this project's executables and libraries."
    OFF)
    MARK_AS_ADVANCED (USE_VTK_OUTPUT_PATHS)
    IF (USE_VTK_OUTPUT_PATHS)
      SET (LIBRARY_OUTPUT_PATH ${VTK_DIR}/bin)
      SET (EXECUTABLE_OUTPUT_PATH ${VTK_DIR}/bin)
    ENDIF (USE_VTK_OUTPUT_PATHS)
  ENDIF (EXISTS ${VTK_DIR}/bin)

  # Create the header to configure the classes.
  CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/vtkFEMSystemConfigure.h.in
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Configure.h
    @ONLY IMMEDIATE)

  # Use wrapping hints for this project.
  SET(VTK_WRAP_HINTS "${PROJECT_SOURCE_DIR}/hints")

  # Create the instantiator for these classes.
  SET(VTK_USE_INSTANTIATOR_NEW 1)
  VTK_MAKE_INSTANTIATOR3("${PROJECT_NAME}Instantiator"
  vtkFEMSystemInstantiator_SRCS
  "${vtkFEMSystem_SRCS}"
  "VTK_${PROJECT_NAME}_EXPORT"
  ${PROJECT_BINARY_DIR}
  "${PROJECT_NAME}Configure.h")

  #getFEM library includes & includes (/CMake/Modules/FindGetFEM++.cmake)
  IF(GETFEM_FOUND)
    INCLUDE_DIRECTORIES(${GETFEM_INCLUDE_DIR})
    SET(vtkFEMSystem_LIBS ${vtkFEMSystem_LIBS} ${GETFEM_LIBRARY})
    LINK_DIRECTORIES( ${LINK_DIRECTORIES} ${GETFEM_LIBRARY_DIR})
  ENDIF(GETFEM_FOUND)
  
  IF(DOLFIN_FOUND)
        SET(vtkFEMSystem_LIBS ${vtkFEMSystem_LIBS} dolfin)
    ENDIF(DOLFIN_FOUND)
  

  # Create the library.
  ADD_LIBRARY(${PROJECT_NAME} ${vtkFEMSystem_SRCS} ${vtkFEMSystemInstantiator_SRCS})
  TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${vtkFEMSystem_LIBS})

  #TCL Wrapping not supported 

ADD_SUBDIRECTORY(Testing)
ENDIF(VTK_FOUND)
