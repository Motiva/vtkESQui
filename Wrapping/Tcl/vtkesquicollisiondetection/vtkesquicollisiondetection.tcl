package require vtkcommon
if {[::vtk::load_component vtkESQuiCollisionDetectionTCL] == ""} {
    package provide vtkesquicollisiondetection 0.1
}
