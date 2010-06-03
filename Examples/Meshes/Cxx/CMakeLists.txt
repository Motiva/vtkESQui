#
# CMakeLists file for a project kit (subdir). You will only need to set the Kit name and 
# the source files and libraries
#

SET(KIT ESQUIExamples)   #Kit Name
SET(UKIT ESQUIEXAMPLES)  #Uppercase Kit Name

SET(Kit_SRCS
    STLtoUnstructuredGrid
)

 # Set your list of libraries here.
SET(KIT_LIBS 
    ${PROJECT_NAME}Common
    vtkRendering
	vtkGraphics 
	vtkCommon
)

# Set your list of TCL libraries here.
SET(KIT_TCL_LIBS 
    ${PROJECT_NAME}CommonTCL
    vtkRenderingTCL
    vtkGraphicsTCL
	vtkCommonTCL
)
 
IF(APPLE)
    SET(EXECUTABLE_FLAG MACOSX_BUNDLE)
ENDIF(APPLE)

FOREACH(name ${Kit_SRCS})
    ADD_EXECUTABLE(${name} ${EXECUTABLE_FLAG} ${name}.cxx)
    TARGET_LINK_LIBRARIES(${name} ${KIT_LIBS})
ENDFOREACH(name)
