#
# Source files
#

SET (LIBRARY_NAME
    mss_vtk
)

SET (SOURCES
    DistanceMSS.cpp
    UGridAcceleratorMSS.cpp
    vtkMassSpring.cpp
)

SET_SOURCE_FILES_PROPERTIES(
    DistanceMSS.cpp
    UGridAcceleratorMSS.cpp
  PROPERTIES
    WRAP_EXCLUDE 1
)

SET (WRAPPED_LIBRARIES
    vtkCommon
    vtkFiltering
    vtkVolumeRendering
)

SET (LIBRARIES
    ${WRAPPED_LIBRARIES}
)

SET (INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/${LIBRARY_NAME}
    ${PROJECT_SOURCE_DIR}/include
)

INCLUDE(${PROJECT_SOURCE_DIR}/CMakeCommon.txt)
