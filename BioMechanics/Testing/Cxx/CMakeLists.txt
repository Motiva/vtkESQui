ADD_EXECUTABLE(TestvtkPSSInterface TestvtkPSSInterface.cxx)
TARGET_LINK_LIBRARIES(TestvtkPSSInterface 
    ${PROJECT_NAME}BioMechanics 
    vtkParticleSpringSystem)
