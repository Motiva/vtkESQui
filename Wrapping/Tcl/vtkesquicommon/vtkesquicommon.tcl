package require vtkcommon
if {[::vtk::load_component vtkESQuiCommonTCL] == ""} {
    package provide vtkesquicommon 0.1
}
