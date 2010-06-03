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

vtkXMLUnstructuredGridReader reader
reader SetFileName $filename
reader Update

set grid [ reader GetOutput ]

#VTK_DOUBLE = 11
if { [[[$grid GetPoints] GetData] GetDataType] != 11 } {
	puts "Input Data Type != Double"
	vtkPoints ps
	ps SetDataTypeToDouble
	ps DeepCopy [ $grid GetPoints ]
	$grid SetPoints ps
	$grid Update
}

vtkXMLUnstructuredGridWriter writer
writer SetDataModeToAscii
writer SetInput $grid
writer SetFileName $outputname
writer Update
