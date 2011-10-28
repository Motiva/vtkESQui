package require vtk

vtkXMLUnstructuredGridWriter writer

#Generate a synthetic cube
vtkCubeSource cube
cube SetCenter 0.0 0.0 0.0
cube SetXLength 0.5
cube SetYLength 0.5
cube SetZLength 0.5
cube Update

vtkCleanPolyData clean
clean SetInput [cube GetOutput]

writer SetInput [ clean GetOutput ]
writer SetDataModeToAscii
set filename "cube.vtp"
writer SetFileName $filename
writer Update