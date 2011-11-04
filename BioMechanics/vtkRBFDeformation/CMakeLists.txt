CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
IF(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
  CMAKE_POLICY(SET CMP0011 NEW)
ENDIF(COMMAND CMAKE_POLICY)

# Set the project/library name here.  Classes should be declared as
# "class vtkRBFDeformation_EXPORT vtkFoo", where vtkRBFDeformation is the name of the
# project set here.
PROJECT(vtkRBFDeformation)

ENABLE_TESTING()

# Set your list of sources here. Do not change the name of the
# vtkRBFDeformation_SRCS variable.
SET(vtkRBFDeformation_SRCS
  vtkRBFDeformation.cxx
  RBFInterpolator.cpp
  newmat.cpp
  #vtkRBFInterpolator.cxx
)

#SET_SOURCE_FILES_PROPERTIES(
#  ABSTRACT
#  )

SET_SOURCE_FILES_PROPERTIES(
  RBFInterpolator.cpp
  newmat.cpp
  PROPERTIES
  WRAP_EXCLUDE 1
  )

# List the kits from VTK that are needed by this project.
SET(vtkRBFDeformation_LIBS
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
  CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/vtkRBFDeformationConfigure.h.in
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Configure.h
    @ONLY IMMEDIATE)

  # Use wrapping hints for this project.
  SET(VTK_WRAP_HINTS "${PROJECT_SOURCE_DIR}/hints")

  # Create the instantiator for these classes.
  SET(VTK_USE_INSTANTIATOR_NEW 1)
  VTK_MAKE_INSTANTIATOR3("${PROJECT_NAME}Instantiator"
  vtkRBFDeformationInstantiator_SRCS
  "${vtkRBFDeformation_SRCS}"
  "VTK_${PROJECT_NAME}_EXPORT"
  ${PROJECT_BINARY_DIR}
  "${PROJECT_NAME}Configure.h")

  # Create the library.
  ADD_LIBRARY(${PROJECT_NAME} ${vtkRBFDeformation_SRCS} ${vtkRBFDeformationInstantiator_SRCS})
  TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${vtkRBFDeformation_LIBS})

  #TCL Wrapping not supported 
SUBDIRS(Testing)
ENDIF(VTK_FOUND)
