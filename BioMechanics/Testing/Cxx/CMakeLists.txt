ADD_EXECUTABLE(TestvtkPSSInterface TestvtkPSSInterface.cxx)
TARGET_LINK_LIBRARIES(TestvtkPSSInterface 
    ${PROJECT_NAME}BioMechanics 
    vtkParticleSpringSystem)

ADD_EXECUTABLE(TestvtkEDMInterface TestvtkEDMInterface.cxx)
TARGET_LINK_LIBRARIES(TestvtkEDMInterface 
    ${PROJECT_NAME}BioMechanics 
    vtkExplicitDeformableModel)