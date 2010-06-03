#
# Source files
#

SET (LIBRARY_NAME
    rdm_vtk
)


SET (SOURCES
	Distance.cpp
	Surface.cpp
	Volume.cpp
	vtkRDM.cpp
	InitialValueProblemSolver.cpp
	EulerSolver.cpp
	RK2Solver.cpp
	RK4Solver.cpp
	VerletSolver.cpp
	VelocityVerletSolver.cpp
	UGridAccelerator.cpp
	#${PROJECT_SOURCE_DIR}/whiteboard_vtk/vtkSimulator.cpp	
)

# Example for abstract classes:
SET_SOURCE_FILES_PROPERTIES(
	Distance.cpp
	Surface.cpp
	Volume.cpp
	InitialValueProblemSolver.cpp
	EulerSolver.cpp
	RK2Solver.cpp
	RK4Solver.cpp
	VerletSolver.cpp
	VelocityVerletSolver.cpp
	UGridAccelerator.cpp
	#${PROJECT_SOURCE_DIR}/whiteboard_vtk/vtkSimulator.cpp
  PROPERTIES
    WRAP_EXCLUDE 1   
)


SET (WRAPPED_LIBRARIES
    vtkCommon
    #whiteboard_vtk
)


SET (LIBRARIES
    ${WRAPPED_LIBRARIES}
)


SET (INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/${LIBRARY_NAME}
    #${PROJECT_SOURCE_DIR}/whiteboard_vtk
    #${PROJECT_SOURCE_DIR}/ugridaccelerator
)



INCLUDE(../CMakeCommon.txt)