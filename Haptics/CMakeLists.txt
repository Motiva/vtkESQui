# 
# CMakeLists file for a project kit (subdir). You will only need to set the Kit name and 
# the source files and libraries
# 

SET(KIT Haptics)   #Kit Name
SET(UKIT HAPTICS)  #Uppercase Kit Name

# Set your list of sources here.  Do not change the name of the
# Kit_SRCS variable.
SET (Kit_SRCS vtkHaptic.cxx)

IF(VTKESQUI_USE_LSW)
  SET(Kit_SRCS ${Kit_SRCS} vtkLSW.cxx vtkLSWTool.cxx)
ENDIF(VTKESQUI_USE_LSW)

IF(VTKESQUI_USE_VSP)
  SET(Kit_SRCS ${Kit_SRCS} vtkVSP.cxx)
ENDIF(VTKESQUI_USE_VSP)

IF(VTKESQUI_USE_IHP)
  SET(Kit_SRCS ${Kit_SRCS} vtkIHP.cxx)
ENDIF(VTKESQUI_USE_IHP)

IF(VTKESQUI_USE_SBM)
  SET(Kit_SRCS ${Kit_SRCS} vtkSBM.cxx)
ENDIF(VTKESQUI_USE_SBM)

SET_SOURCE_FILES_PROPERTIES(
vtkHaptic
ABSTRACT
)
#SET_SOURCE_FILES_PROPERTIES(
#vtkLSWTool.cxx
#WRAP_EXCLUDE
#)

# Set your list of libraries here.
SET(KIT_LIBS 
${PROJECT_NAME}Common
${PROJECT_NAME}Scenario
vtkCommon
)

# Set your list of TCL libraries here.
SET(KIT_TCL_LIBS
${PROJECT_NAME}CommonTCL
${PROJECT_NAME}ScenarioTCL 
vtkCommonTCL
)

# Set your list of Python libraries here.
SET(KIT_PYTHON_LIBS 
${PROJECT_NAME}CommonPythonD
${PROJECT_NAME}ScenarioPythonD
vtkCommonPythonD
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

