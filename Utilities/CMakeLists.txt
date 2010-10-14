#
# CMakeLists file for a project kit (subdir). You will only need to set the Kit name and 
# the source files and libraries
#

SET(KIT Utilities)   #Kit Name
SET(UKIT UTILITIES)  #Uppercase Kit Name

# Set your list of sources here.  Do not change the name of the
# Kit_SRCS variable.
SET (Kit_SRCS 
vtk3DSImporterEsqui.cpp
vtkSRMLImporter.cxx
vtkPointPlotter.cxx
)

# Set your list of libraries here.
SET(KIT_LIBS
${PROJECT_NAME}Manager
${PROJECT_NAME}Scenario
${PROJECT_NAME}BMM
vtkCommon 
vtkRendering
vtkHybrid
vtkParticleSpringSystem
vtkBioeng
)

# Set your list of TCL libraries here.
SET(KIT_TCL_LIBS
${PROJECT_NAME}ManagerTCL
${PROJECT_NAME}ScenarioTCL
${PROJECT_NAME}BMMTCL
vtkCommonTCL
vtkRenderingTCL
vtkHybridTCL
)

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
# Include common Haptics CMake code. 
# Note: Should be located before KitCommonBlock INCLUDE command
IF(VTKESQUI_USE_HAPTICS)
  INCLUDE(${VTKESQUI_CMAKE_DIR}/HapticCommonBlock.cmake)
ENDIF(VTKESQUI_USE_HAPTICS)
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Include CMake code common to all kits.
INCLUDE(${VTKESQUI_CMAKE_DIR}/KitCommonBlock.cmake)
#-----------------------------------------------------------------------------

# Subdirs to be added
SUBDIRS(Testing)
