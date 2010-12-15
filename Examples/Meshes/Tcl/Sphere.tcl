package require vtk

if { $argc != 3 } {
	puts "The command requires 3 arguments: thetaResolution phiResolution radius"
	puts "e.g.: vtk Sphere.tcl 6 6 2.0"
	puts "Please try again."
	exit
}

set theta [lindex $argv 0]
set phi [lindex $argv 1]
set radius [lindex $argv 2]

puts "$theta $phi $radius"
vtkXMLPolyDataWriter writer

#Generate a synthetic sphere
vtkSphereSource sphere
sphere SetThetaResolution $theta
sphere SetPhiResolution $phi
sphere SetRadius $radius
sphere Update

#Triangulate mesh
vtkTriangleFilter triangleFilter
triangleFilter SetInput [sphere GetOutput]
triangleFilter Update

#Write polydata file
writer SetInput [triangleFilter GetOutput]
writer SetDataModeToAscii
set filename "sphere.vtp"
writer SetFileName $filename
writer Update
