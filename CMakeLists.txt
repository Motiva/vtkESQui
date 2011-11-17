CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
IF(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

# Set the project/library name here.  Classes should be declared as
# "class vtkESQui_EXPORT vtkFoo", where vtkESQui is the name of the
# project set here.
PROJECT (vtkESQui)

#
# Dart config
#

ENABLE_TESTING()

#Dashboard testing
#INCLUDE(CTest)

#
#Enabling/disabling Haptic use
#
OPTION (VTKESQUI_USE_HAPTICS "Set this option if you want to use an haptic device")

#
#Enabling/disabling FEM use
#
OPTION (VTKESQUI_USE_FEM "Set this option if you want to enable FEM")

#
# Need to include class headers and the configuration header.
#
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})

#
# Setup output paths.
#
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib CACHE PATH
    "Single output directory for building all libraries.")
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin CACHE PATH
    "Single output directory for building all executables.")
MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)

#
# Find and load VTK settings.
#
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

#
# Find and load ITK Settings
#
FIND_PACKAGE ( ITK QUIET )
IF ( ITK_FOUND )
      INCLUDE( ${USE_ITK_FILE} )
ENDIF( ITK_FOUND )

#
# Set Common Path Values
#
SET (VTKESQUI_SOURCE_DIR ${PROJECT_SOURCE_DIR})
SET (VTKESQUI_CMAKE_DIR "${PROJECT_SOURCE_DIR}/CMake")

#
# Set Common Prefixes
#
SET (UPROJECT_NAME "VTKESQUI")
SET (UEXPORT_PREFIX "VTK_ESQUI_")

#
# Expand CMAKE_MODULE_PATH
#
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMake/Modules/")

#
# Give user option of building shared or static libraries.  Default to
# the choice made for VTK.
#
OPTION(BUILD_SHARED_LIBS "Build with shared libraries."
       ${VTK_BUILD_SHARED_LIBS})
SET(VTKESQUI_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})

#
# Set VTKLOCAL_BUILD_SHARED_LIBS to 1 or 0 for use in creating the
# configuration header.
#
IF(BUILD_SHARED_LIBS)
  SET(VTKESQUI_BUILD_SHARED_LIBS_CONFIG 1)
ELSE(BUILD_SHARED_LIBS)
  SET(VTKESQUI_BUILD_SHARED_LIBS_CONFIG 0)
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
  CONFIGURE_FILE(
      ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}Configure.h.in
      ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Configure.h
  )

  # Use wrapping hints for this project.
  SET(VTK_WRAP_HINTS "${PROJECT_SOURCE_DIR}/Wrapping/hints")
  # Create Tcl wrappers if VTK is wrapped in Tcl.
  IF(VTK_WRAP_TCL)
  
    OPTION(VTKESQUI_WRAP_TCL
            "Wrap classes into the TCL interpreted language."
            ON)
    
    IF(VTKESQUI_WRAP_TCL)
      INCLUDE(${VTK_CMAKE_DIR}/vtkWrapTcl.cmake)
    ENDIF(VTKESQUI_WRAP_TCL)

  ENDIF(VTK_WRAP_TCL)
  
  # Create Python wrappers if VTK is wrapped in Tcl.
  IF (VTK_WRAP_PYTHON)

    OPTION(VTKESQUI_WRAP_PYTHON
         "Wrap classes into the Python interpreted language."
         ON)

    IF (VTKESQUI_WRAP_PYTHON)
      SET(VTK_WRAP_PYTHON_FIND_LIBS ON)
      INCLUDE(${VTK_CMAKE_DIR}/vtkWrapPython.cmake)
      IF (WIN32)
        IF (NOT BUILD_SHARED_LIBS)
          MESSAGE(FATAL_ERROR "Python support requires BUILD_SHARED_LIBS to be ON.")
          SET (VTKESQUI_CAN_BUILD 0)
        ENDIF (NOT BUILD_SHARED_LIBS)
      ENDIF (WIN32)
    ENDIF (VTKESQUI_WRAP_PYTHON)
  ENDIF (VTK_WRAP_PYTHON)
  
  #
  # Build examples too.
  #

  OPTION(BUILD_EXAMPLES "Build Examples." OFF)
  IF (BUILD_EXAMPLES)
    ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/Examples)
  ENDIF (BUILD_EXAMPLES)
 
  #
  # Include path(s) of the local classes of the project
  #

  INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})
  INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Common)
  INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Scenario)
  INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Simulation)
  INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Interaction)
  #INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Utilities)
  INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/BioMechanics)
  INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/BioMechanics/vtkParticleSpringSystem)
  INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/BioMechanics/vtkExplicitDeformableModel)
  INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/BioMechanics/vtkExplicitDeformableModel/Filters)
  INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/CollisionDetection)
  INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/CollisionDetection/vtkbioeng)
  
  IF(VTKESQUI_USE_HAPTICS)
    
    INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Haptics)
    
    #
    # You may have to change the HAPTIC SDK's path in the FindHAPTIC.cmake located in src/CMake/Modules
    # Note: IHP & VSP may not be used simultaneously
    #
    
    # LSW Haptic libraries
    FIND_PACKAGE(LSWSDK REQUIRED)
    INCLUDE_DIRECTORIES(${LSWSDK_INCLUDE_DIRS})
    
    # IHP Haptic libraries
    FIND_PACKAGE(IHPSDK REQUIRED)
    INCLUDE_DIRECTORIES(${IHPSDK_INCLUDE_DIRS})
    
    # VSP Haptic libraries
    FIND_PACKAGE(VSPSDK REQUIRED)
    INCLUDE_DIRECTORIES(${VSPSDK_INCLUDE_DIRS})
    
  ENDIF(VTKESQUI_USE_HAPTICS)

  IF(VTKESQUI_USE_FEM)
    INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/BioMechanics/vtkFEMSystem)
  ENDIF(VTKESQUI_USE_FEM)
  
  #
  # Build Documentation
  #
  IF (BUILD_DOCUMENTATION)
    ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/Documentation)
  ENDIF (BUILD_DOCUMENTATION)
  
  #
  # Use CUDA
  #
  OPTION(VTKESQUI_USE_CUDA "Enable the use of CUDA GPU computation" OFF)
  IF(VTKESQUI_USE_CUDA)
    INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/BioMechanics/vtkCUDAParticleSystem)
  ENDIF(VTKESQUI_USE_CUDA)
  
  # Add Subdirectories
  ADD_SUBDIRECTORY(Common)
  ADD_SUBDIRECTORY(CollisionDetection)
  ADD_SUBDIRECTORY(BioMechanics)
  ADD_SUBDIRECTORY(Scenario)
  ADD_SUBDIRECTORY(Interaction)
  ADD_SUBDIRECTORY(Simulation)
  
  IF(VTKESQUI_USE_HAPTICS)
    ADD_SUBDIRECTORY(Haptics)
  ENDIF(VTKESQUI_USE_HAPTICS)
  
ENDIF (VTK_FOUND)
