package require vtk

if { $argc != 3 } {
	puts "The command requires 3 arguments: normalDirection x, y, z"
	puts "e.g.: vtk Plane.tcl 1.0 0.0 0.0"
	puts "Please try again."
	exit
}

set x [lindex $argv 0]
set y [lindex $argv 1]
set z [lindex $argv 2]

vtkXMLPolyDataWriter writer

#Generate a synthetic plane
vtkPlaneSource plane
plane SetCenter 1.0 0.0 0.0
plane SetNormal $x $y $z

set data [ plane GetOutput ]

# Write the polydata file 
writer SetInput $data
writer SetFileName "plane.vtp"
writer Write
