CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
IF(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0002 NEW)
  CMAKE_POLICY(SET CMP0003 NEW)
  CMAKE_POLICY(SET CMP0011 NEW)
ENDIF(COMMAND CMAKE_POLICY)

# Set the project/library name here.  Classes should be declared as
# "class vtkCUDAParticleSystem_EXPORT vtkFoo", where vtkCUDAParticleSystem is the name of the
# project set here.
PROJECT(vtkCUDAParticleSystem)

# Set your list of sources here.  Do not change the name of the
# vtkCUDAParticleSystem_SRCS variable.
SET(vtkCUDAParticleSystem_SRCS
  vtkCUDAParticleSystem.cxx
  vtkCUDAParticleSystem.cu
  vtkCUDAMotionEquationSolver.cxx
  vtkCUDAMidPointSolver.cxx
  vtkCUDAEulerSolver.cxx
  vtkCUDARK4Solver.cxx
  vtkCUDAVelocityVerletSolver.cxx
)

SET_SOURCE_FILES_PROPERTIES(
  vtkCUDAMotionEquationSolver.cxx
  ABSTRACT
  )

SET_SOURCE_FILES_PROPERTIES(
#    vtkVelocityVerletSolver.cxx
    vtkCUDARK4Solver.cxx
	PROPERTIES
	WRAP_EXCLUDE 1
	)

# List the kits from VTK that are needed by this project.
SET(vtkCUDAParticleSystem_LIBS
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
  CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/vtkCUDAParticleSystemConfigure.h.in
                 ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Configure.h
                 @ONLY IMMEDIATE)

  # Use wrapping hints for this project.
  SET(VTK_WRAP_HINTS "${PROJECT_SOURCE_DIR}/hints")

  # Create the instantiator for these classes.
  SET(VTK_USE_INSTANTIATOR_NEW 1)
  VTK_MAKE_INSTANTIATOR3("${PROJECT_NAME}Instantiator"
    vtkCUDAParticleSystemInstantiator_SRCS
    "${vtkCUDAParticleSystem_SRCS}"
    "VTK_${PROJECT_NAME}_EXPORT"
    ${PROJECT_BINARY_DIR}
    "${PROJECT_NAME}Configure.h")

  # CUDA Support
  OPTION(USE_CUDA "Enable the use of CUDA GPU computation" ON)
  IF(USE_CUDA)
    FIND_PACKAGE(CUDA REQUIRED)
    
    SET(CUDA_PROPAGATE_HOST_FLAGS ON)
    # The cuda_include_directories adds paths to only cuda compilation.
    CUDA_INCLUDE_DIRECTORIES(
      ${CMAKE_CURRENT_SOURCE_DIR}
      ${CMAKE_CURRENT_SOURCE_DIR}/thrust

    # Set CUDA_ATTACH_VS_BUILD_RULE_TO_CUDA_FILE when you want to add the same .cu
    # file to multiple targets.
    SET(CUDA_ATTACH_VS_BUILD_RULE_TO_CUDA_FILE OFF)
    )
    
    IF(CUDA_FOUND)
       
      INCLUDE_DIRECTORIES(${CUDA_INCLUDE_DIRECTORIES}
                          ${CUDA_SDK_ROOT_DIR}/C/common/inc
                          ${CUDA_TOOLKIT_INCLUDE})
      
      CUDA_ADD_LIBRARY(vtkCUDAParticleSystemCUDA
      vtkCUDAParticleSystem.cu
      vtkCUDAParticleSystem.cxx)
      
      SET(vtkCUDAParticleSystem_LIBS ${vtkCUDAParticleSystem_LIBS} vtkCUDAParticleSystemCUDA)
      
    ENDIF(CUDA_FOUND)
    
  ENDIF(USE_CUDA)
  
  # Create the library.
  ADD_LIBRARY(${PROJECT_NAME} ${vtkCUDAParticleSystem_SRCS} ${vtkCUDAParticleSystemInstantiator_SRCS})
  TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${vtkCUDAParticleSystem_LIBS})

 #TCL Wrapping not supported 

SUBDIRS(Testing)
ENDIF(VTK_FOUND)
