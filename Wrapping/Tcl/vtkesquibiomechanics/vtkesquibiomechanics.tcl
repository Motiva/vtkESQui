#package require vtkcommon

if {[::vtk::load_component vtkESQuiBioMechanicsTCL] == ""} {
    package provide vtkesquibiomechanics 0.1
}
