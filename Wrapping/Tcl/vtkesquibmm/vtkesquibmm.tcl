#package require vtkcommon

if {[::vtk::load_component vtkESQuiBMMTCL] == ""} {
    package provide vtkesquibmm 0.1
}
