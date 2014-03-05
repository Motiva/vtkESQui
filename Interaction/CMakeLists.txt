#
# CMakeLists file for a project kit (subdir). You will only need to set the Kit name and 
# the source files and libraries
#

SET(KIT Interaction)   #Kit Name
SET(UKIT INTERACTION)  #Uppercase Kit Name

# Set your list of sources here.  Do not change the name of the
# Kit_SRCS variable.
SET (Kit_SRCS 
vtkDefaultInteractorStyle.cxx
vtkSingleChannelInteractorStyle.cxx
)

# Set your list of libraries here.
SET(KIT_LIBS
${PROJECT_NAME}Scenario 
vtkCommon
vtkRendering
)

# Set your list of TCL libraries here.
SET(KIT_TCL_LIBS 
${PROJECT_NAME}ScenarioTCL
vtkCommonTCL
vtkRenderingTCL
)

# Set your list of Python libraries here.
SET(KIT_PYTHON_LIBS 
${PROJECT_NAME}ScenarioPythonD
vtkCommonPythonD
vtkRenderingPythonD
)

#-----------------------------------------------------------------------------
# Include CMake code common to all kits.
INCLUDE(${VTKESQUI_CMAKE_DIR}/KitCommonBlock.cmake)
#-----------------------------------------------------------------------------

ADD_SUBDIRECTORY(Testing)
