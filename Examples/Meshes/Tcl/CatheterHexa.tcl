package require vtk

set numberOfPoints 20

vtkPoints points
vtkCellArray lines

for {set x 0} {$x<$numberOfPoints} {incr x} {
	set p [expr {$x/20.0}]
	#puts "$x $p"
	vtkLine line$x
	[line$x GetPointIds] SetId 0 $x
	[line$x GetPointIds] SetId 1 [expr { $x+1 } ]
	points InsertNextPoint $p 0 0
	if {$x!=19} {
		lines InsertNextCell line$x
		
	}
}

vtkPolyData cath
	cath SetPoints points
	cath SetLines lines
	
vtkXMLPolyDataWriter writer

#Write polydata file
writer SetDataModeToAscii
writer SetInput cath
set filename "catheter.vtp"
writer SetFileName $filename
writer Update
