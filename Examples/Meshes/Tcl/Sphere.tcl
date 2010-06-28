package require vtk

if { $argc != 3 } {
	puts "The command requires 3 arguments: thetaResolution phiResolution radius"
	puts "e.g.: vtk Sphere.tcl 6.0 2.0"
	puts "Please try again."
	exit
}

set theta [lindex $argv 0]
set phi [lindex $argv 1]
set radius [lindex $argv 2]

vtkXMLPolyDataWriter writer

#Generate a synthetic sphere
vtkSphereSource sphere
sphere SetThetaResolution $theta
sphere SetPhiResolution $phi
sphere SetRadius $radius
sphere Update

#Write polydata file
writer SetInput [sphere GetOutput]
set filename "sphere.vtp"
writer SetFileName $filename
writer Update
