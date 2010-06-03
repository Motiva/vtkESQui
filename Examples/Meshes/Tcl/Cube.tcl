package require vtk

vtkUnstructuredGridWriter writer

#Generate a synthetic cube
vtkCubeSource cube
cube SetCenter 0.0 0.0 0.0
cube SetXLength 0.5
cube SetYLength 0.5
cube SetZLength 0.5
cube Update

vtkDelaunay3D cubeMesh
cubeMesh SetInput [ cube GetOutput ]
cubeMesh Update

#set mesh [cubeMesh GetOutput]
#vtkPoints ps
#ps SetDataTypeToDouble
#ps DeepCopy [$mesh GetPoints]
#$mesh SetPoints ps

#writer SetInputConnection [ cube GetOutputPort ]
#set filename ""
#append filename $path "cube.vtp"
#writer SetFileName $filename
#writer Update

writer SetInput [ cubeMesh GetOutput ]
set filename "cube.vtu"
writer SetFileName $filename
writer Update