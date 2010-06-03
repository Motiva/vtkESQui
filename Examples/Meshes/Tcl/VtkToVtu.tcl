package require vtk

if { $argc != 2 } {
	puts "The command requires 2 arguments: InputFileName OutputFileName"
	puts "Please try again."
	exit
}

set filename [lindex $argv 0]
set outputname [lindex $argv 1]

set ext [file extension $filename]
set path [file dirname $filename]
#set name [file ]

vtkUnstructuredGridReader reader
reader SetFileName $filename
reader Update

set grid [ reader GetOutput ]

vtkXMLUnstructuredGridWriter writer
writer SetDataModeToAscii
writer SetInput $grid
writer SetFileName $outputname
writer Update
