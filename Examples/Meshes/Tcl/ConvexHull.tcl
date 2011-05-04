package require vtk

if { $argc != 1 } {
	puts "The command requires at least 2 arguments: PathToFile"
	puts "e.g.: vtk ConvexHull.tcl input.vtp"
	puts "Please try again."
	exit
}

puts "\n#### Mesh decimation script ####\n"
set path [lindex $argv 0]
set r [lindex $argv 1]

#Read file
vtkXMLPolyDataReader reader
reader SetFileName $path
reader Update

set input [reader GetOutput]

#Convex Hull
vtkHull hullFilter
hullFilter SetInput $input
hullFilter AddCubeFacePlanes
hullFilter Update;

set output [hullFilter GetOutput]

puts "Mesh succesfully decimated.\n"
puts "Input Mesh points: [$input GetNumberOfPoints]"
puts "Output Mesh points: [$output GetNumberOfPoints]"
#Write output file

set filename "output.vtp"
vtkXMLPolyDataWriter writer
writer SetDataModeToAscii
writer SetInput $output
writer SetFileName $filename
writer Update

wm withdraw .
puts "################################"

exit