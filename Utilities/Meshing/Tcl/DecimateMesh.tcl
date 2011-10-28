package require vtk

if { $argc < 2 } {
	puts "The command requires at least 2 arguments: PathToFile ReductionTarget"
	puts "e.g.: vtk DecimateMesh.tcl input.vtp 0.5"
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

#Decimate Mesh
vtkDecimatePro decimate
decimate SetInput $input
decimate SetTargetReduction $r
decimate Update
set output [decimate GetOutput]

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