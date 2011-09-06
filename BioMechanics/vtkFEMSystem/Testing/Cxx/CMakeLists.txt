IF(GETFEM_FOUND)
    ADD_EXECUTABLE(TestvtkGetFEMInterface TestvtkGetFEMInterface.cxx)
    TARGET_LINK_LIBRARIES(TestvtkGetFEMInterface vtkFEMSystem)
ENDIF(GETFEM_FOUND)

IF(DOLFIN_FOUND)
    ADD_EXECUTABLE(TestvtkDolfinInterface TestvtkDolfinInterface.cxx)
    TARGET_LINK_LIBRARIES(TestvtkDolfinInterface vtkFEMSystem)
ENDIF(DOLFIN_FOUND)