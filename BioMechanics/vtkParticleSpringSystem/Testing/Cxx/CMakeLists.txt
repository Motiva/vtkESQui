ADD_EXECUTABLE(TestvtkParticleSpringSystem TestvtkParticleSpringSystem.cxx)
TARGET_LINK_LIBRARIES(TestvtkParticleSpringSystem 
    vtkESQuiCommon
    vtkESQuiUtilities
    vtkParticleSpringSystem)
