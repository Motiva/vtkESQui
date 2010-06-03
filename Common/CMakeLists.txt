#
# CMakeLists file for a project kit (subdir). You will only need to set the Kit name and 
# the source files and libraries
#

SET(KIT Common)   #Kit Name
SET(UKIT COMMON)  #Uppercase Kit Name

# Set your list of sources here.  Do not change the name of the
# Kit_SRCS variable.
SET (Kit_SRCS 
vtkContact.cxx
vtkContactCollection.cxx
vtkTimerCallback.cxx
)

SET_SOURCE_FILES_PROPERTIES(
vtkTimerCallback
WRAP_EXCLUDE
)

# Set your list of libraries here.
SET(KIT_LIBS 
vtkCommon
vtkRendering 
)

# Set your list of TCL libraries here.
SET(KIT_TCL_LIBS 
vtkCommonTCL
vtkRenderingTCL
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
# Include CMake code common to all kits.
INCLUDE(${VTKESQUI_CMAKE_DIR}/KitCommonBlock.cmake)
#-----------------------------------------------------------------------------

SUBDIRS(Testing)
