CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
#
# Set the project name.
#

PROJECT (VTKESQUI)

#
# Dart config
#
ENABLE_TESTING()
INCLUDE(CTest)
OPTION (VTKESQUI_USE_HAPTICS "Set this option if you want to use an haptic device")

#
# Assume everything is set up correctly for build.
#
# If some requirements are not met, set it to 0.
# In that case, the build won't be done.
#

SET (VTKESQUI_CAN_BUILD 1)

#
# Load CMake commands that you probably should not modify.
#
 
INCLUDE (${VTKESQUI_SOURCE_DIR}/src/CMakeOptions.cmake)


#
# If everything is OK, then go into the sub directories and build.
#

IF (VTKESQUI_CAN_BUILD)

  #
  # Here is where you can list the sub-directories holding your local 
  # classes. Sorting classes by 'package' type like VTK does (Common, 
  # Rendering, Filtering, Imaging, IO, etc.) is a good thing and prevents 
  # numerous dependencies problems.
  #

  IF (VTKESQUI_USE_HAPTICS)
    SUBDIRS (
      src/Tools
      src/Scenario
      src/Common
      src/Haptic
      src/BMM/T2Mesh
      src/Simulation/HapticOn
      src/Manager
      src/CDM/vtkbioeng
      src/Test
    )
  ELSE (VTKESQUI_USE_HAPTICS)
    SUBDIRS (
      src/Tools
      src/Scenario	
      src/Common
      src/BMM/T2Mesh
      src/Simulation/HapticOff
      src/Manager
      src/CDM/vtkbioeng
      src/Test
    )
   ENDIF (VTKESQUI_USE_HAPTICS)

  #
  # Include path(s) of the local classes of the project
  #

  INCLUDE_DIRECTORIES(${VTKESQUI_SOURCE_DIR}/src/Scenario)
  INCLUDE_DIRECTORIES(${VTKESQUI_SOURCE_DIR}/src/Manager)
  INCLUDE_DIRECTORIES(${VTKESQUI_SOURCE_DIR}/src/Tools)
  INCLUDE_DIRECTORIES(${VTKESQUI_SOURCE_DIR}/src/Common)
  INCLUDE_DIRECTORIES(${VTKESQUI_SOURCE_DIR}/src/BEM)
  INCLUDE_DIRECTORIES(${VTKESQUI_SOURCE_DIR}/src/Haptic)
  INCLUDE_DIRECTORIES(${VTKESQUI_SOURCE_DIR}/src/BMM/T2Mesh)
  INCLUDE_DIRECTORIES(${VTKESQUI_SOURCE_DIR}/src/CDM/vtkbioeng)
  INCLUDE_DIRECTORIES(${VTKESQUI_SOURCE_DIR}/src/Test)

  IF (VTKESQUI_USE_HAPTICS)
	INCLUDE_DIRECTORIES(${VTKESQUI_SOURCE_DIR}/src/Simulation/HapticOn)
#	INCLUDE_DIRECTORIES(${VTKESQUI_SOURCE_DIR}/src/T2MeshHapticOn)
  ELSE (VTKESQUI_USE_HAPTICS)
	INCLUDE_DIRECTORIES(${VTKESQUI_SOURCE_DIR}/src/Simulation/HapticOff)
#	INCLUDE_DIRECTORIES(${VTKESQUI_SOURCE_DIR}/src/T2MeshHapticOff)
  ENDIF (VTKESQUI_USE_HAPTICS)

  #
  # 3rd Party Modules Includes and Links
  #
 
  # Add include directories needed to use vtkBioeng.
  #INCLUDE_DIRECTORIES(${VTKESQUI_SOURCE_DIR}/src/CDM/vtkbioeng)
  #INCLUDE_DIRECTORIES(${VTKESQUI_SOURCE_DIR}/src/CDM/ColDetect) #Solo est� incluyendo MathGlobal
  
  # Add link directories needed to use vtkBioeng.
  #IF (UNIX)
  #  LINK_DIRECTORIES(${VTKESQUI_SOURCE_DIR}/src/CDM/vtkbioeng/bin/unix)
  #ELSE (UNIX)
  #  LINK_DIRECTORIES(${VTKESQUI_SOURCE_DIR}/src/CDM/vtkbioeng/bin/win)
  #ENDIF (UNIX)  
  
  #
  # Build examples too.
  #

  OPTION(BUILD_EXAMPLES "Build examples." OFF)
  IF (BUILD_EXAMPLES)
    SUBDIRS(Examples)
  ENDIF (BUILD_EXAMPLES)
  
  #
  # Common configuration settings
  #
  # Do not worry about this one.
  #

  CONFIGURE_FILE(
    ${VTKESQUI_SOURCE_DIR}/src/vtkesquiConfigure.h.in 
    ${VTKESQUI_BINARY_DIR}/vtkesquiConfigure.h
   # ${VTKESQUI_BINARY_DIR}/vtkesquiConfigure.h
  )

  INCLUDE_DIRECTORIES(${VTKESQUI_BINARY_DIR})
  
ENDIF (VTKESQUI_CAN_BUILD)
