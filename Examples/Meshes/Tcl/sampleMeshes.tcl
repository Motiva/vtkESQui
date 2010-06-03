package require vtk

set path "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Meshes/"

vtkXMLPolyDataWriter writer
vtkUnstructuredGridWriter meshWriter

#Generate a synthetic Tetrahedron

vtkPoints points
#DataType must be set to double
points SetDataTypeToDouble
points InsertNextPoint 0 -1 -1
points InsertNextPoint -1 -1 1
points InsertNextPoint 1 1 1
points InsertNextPoint 0 1 0
		
vtkTetra tetra
[tetra GetPointIds] SetId 0 0
[tetra GetPointIds] SetId 1 1
[tetra GetPointIds] SetId 2 2
[tetra GetPointIds] SetId 3 3

set cellType [tetra GetCellType]
vtkUnstructuredGrid tetraGrid
tetraGrid SetPoints points
tetraGrid InsertNextCell $cellType [tetra GetPointIds]

vtkDelaunay3D tetraMesh
tetraMesh SetInput tetraGrid
tetraMesh Update
set filename ""
append filename $path "tetraGrid.vtk"
meshWriter SetInput [ tetraMesh GetOutput ]
meshWriter SetFileName $filename
meshWriter Update


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

#DataType must be set to double
set mesh [cubeMesh GetOutput]
vtkPoints ps
ps SetDataTypeToDouble
ps DeepCopy [$mesh GetPoints]
$mesh SetPoints ps

writer SetInputConnection [ cube GetOutputPort ]
set filename ""
append filename $path "cube.vtp"
writer SetFileName $filename
writer Update

meshWriter SetInput [ cubeMesh GetOutput ]
set filename ""
append filename $path "cubeGrid.vtk"
meshWriter SetFileName $filename
meshWriter Update

#Generate a synthetic sphere

vtkSphereSource sphere
sphere SetRadius 5.0
sphere Update

#Generate a volume
#vtkAppendPolyData spheres

#vtkPoints center
#center InsertNextPoint 0 0 0
#vtkPolyData centerPD
#centerPD SetPoints center

#spheres AddInput [centerPD]

#spheres AddInput [sphere GetOutput]

#for {set i 1} {$i <=4} {incr i} {
#  vtkSphereSource sphere$i
#  sphere$i SetRadius $i
#  sphere$i Update
  
#  spheres AddInput [sphere$i GetOutput]
#}

#spheres Update

vtkCleanPolyData clean
clean SetInput [sphere GetOutput]
clean Update

vtkPolyDataNormals normals
normals SetInput [clean GetOutput]
normals SetFeatureAngle 60.0
normals FlipNormalsOff
normals Update

vtkDelaunay3D sphereMesh
sphereMesh SetInput [normals GetOutput]
sphereMesh Update

#DataType must be set to double
set mesh [sphereMesh GetOutput]
ps Reset
ps SetDataTypeToDouble
ps DeepCopy [$mesh GetPoints]
$mesh SetPoints ps

writer SetInput [sphere GetOutput]
set filename ""
append filename $path "sphere.vtp"
writer SetFileName $filename
writer Update

meshWriter SetInput [ sphereMesh GetOutput ]
set filename ""
append filename $path "sphereGrid.vtk"
meshWriter SetFileName $filename
meshWriter Update
