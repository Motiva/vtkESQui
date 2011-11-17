
# CMakeLists file for a project kit (subdir). You will only need to set the Kit name and 
# the source files and libraries
#

SET(KIT BioMechanics)   #Kit Name
SET(UKIT BioMechanics)  #Uppercase Kit Name

# Set your list of sources here.  Do not change the name of the
# Kit_SRCS variable.
SET (Kit_SRCS 
vtkDeformationModel.cxx
vtkParticleSpringSystemInterface.cxx
vtkEDMInterface.cxx
vtkBoundaryCondition.cxx
vtkBoundaryConditionCollection.cxx
)

SET_SOURCE_FILES_PROPERTIES(
vtkBioMechanicalModel
vtkDeformationModel
ABSTRACT
)

# Set your list of libraries here.
SET(KIT_LIBS ${PROJECT_NAME}Common
vtkCommon
vtkParticleSpringSystem
vtkExplicitDeformableModel
)

# Set your list of TCL libraries here.
SET(KIT_TCL_LIBS 
${PROJECT_NAME}CommonTCL
vtkCommonTCL
)

# Set your list of Python libraries here.
SET(KIT_PYTHON_LIBS 
${PROJECT_NAME}CommonPythonD
vtkCommonPythonD
)

IF(VTKESQUI_USE_CUDA)
  SET(Kit_SRCS ${Kit_SRCS}
  vtkCUDAParticleSystemInterface.cxx)
  SET(KIT_LIBS ${KIT_LIBS}
      vtkCUDAParticleSystem)
ENDIF(VTKESQUI_USE_CUDA)

#
# Uncomment the following code if you are plannig to add python and/or java support
#


#SET(KIT_PYTHON_LIBS)
#SET(KIT_JAVA_LIBS)


#
# Uncomment the following code if you are plannig to add extra files, commands or dependencies
#

#SET(Kit_EXTRA_SRCS)
#SET(Kit_EXTRA_CMDS)
#SET(Kit_TCL_EXTRA_SRCS)
#SET(Kit_PYTHON_EXTRA_SRCS)
#SET(Kit_JAVA_EXTRA_SRCS)
#SET(KIT_TCL_DEPS)
#SET(KIT_PYTHON_DEPS)
#SET(KIT_JAVA_DEPS)

#-----------------------------------------------------------------------------
# Include CMake code common to all kits.
INCLUDE(${VTKESQUI_CMAKE_DIR}/KitCommonBlock.cmake)
#-----------------------------------------------------------------------------

# Subdirs to be added

ADD_SUBDIRECTORY(Testing)
ADD_SUBDIRECTORY(vtkParticleSpringSystem)
ADD_SUBDIRECTORY(vtkExplicitDeformableModel)
IF(VTKESQUI_USE_FEM)
  ADD_SUBDIRECTORY(vtkFEMSystem)
ENDIF(VTKESQUI_USE_FEM)
IF(VTKESQUI_USE_CUDA)
  ADD_SUBDIRECTORY(vtkCUDAParticleSystem)
ENDIF(VTKESQUI_USE_CUDA)