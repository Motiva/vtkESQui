package require vtk

if { $argc != 1 } {
	puts "The command requires at least 1 arguments: PathToFile"
	puts "e.g.: vtk vtktovtp.tcl input.vtk"
	puts "Please try again."
	exit
}

# set path "/home/jballesteros/Woorkspace/data/vtkESQuiData/Scenario/Meshes/STL/cylinder.stl"

set path [lindex $argv 0]

vtkPolyDataReader reader
reader SetFileName $path
reader Update

vtkXMLPolyDataWriter writer
writer SetInput [reader GetOutput]
writer SetFileName "output.vtp"
writer Update

# prevent the tk window from showing up then start the event loop
wm withdraw .

puts "File succesfully converted."