#
# CMakeLists file for a project kit (subdir). You will only need to set the Kit name and 
# the source files and libraries
#

SET(KIT ESQUIHapticsExamples)   #Kit Name
SET(UKIT ESQUIHAPTICSEXAMPLES)  #Uppercase Kit Name

#VSP and IHP Examples cannot be compiled simultaneously due to dll conflicts
SET(Kit_SRCS
    HapticIHPExample
    HapticLSWExample
    HapticVSPExample
 )
 
#-----------------------------------------------------------------------------
# Include common Haptics CMake code. 
# Note: Should be located before KitCommonBlock INCLUDE command
IF(VTKESQUI_USE_HAPTICS)
  INCLUDE(${VTKESQUI_CMAKE_DIR}/HapticCommonBlock.cmake)
ENDIF(VTKESQUI_USE_HAPTICS)
#-----------------------------------------------------------------------------

 # Set your list of libraries here.
SET(KIT_LIBS 
    ${PROJECT_NAME}Haptics
    ${PROJECT_NAME}Scenario
    vtkRendering
  vtkGraphics 
  vtkCommon
  vtkHybrid
)

# Set your list of TCL libraries here.
SET(KIT_TCL_LIBS 
    ${PROJECT_NAME}HapticsTCL
    ${PROJECT_NAME}ScenarioTCL
    vtkRenderingTCL
    vtkGraphicsTCL
  vtkCommonTCL
  vtkHybridTCL
)
 
IF(APPLE)
    SET(EXECUTABLE_FLAG MACOSX_BUNDLE)
ENDIF(APPLE)

FOREACH(name ${Kit_SRCS})
    ADD_EXECUTABLE(${name} ${EXECUTABLE_FLAG} ${name}.cxx)
    TARGET_LINK_LIBRARIES(${name} ${KIT_LIBS})
ENDFOREACH(name)
