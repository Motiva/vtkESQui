package require vtk

set numberOfPoints 20

vtkPoints points
vtkCellArray lines

for {set x 0} {$x<$numberOfPoints} {incr x} {
	set p [expr {$x}]
	#puts "$x $p"
	vtkLine line$x
	[line$x GetPointIds] SetId 0 $x
	[line$x GetPointIds] SetId 1 [expr { $x+1 } ]
	points InsertNextPoint $p 0 0
	if {$x!=19} {
		lines InsertNextCell line$x
	}
}

vtkPolyData line
	line SetPoints points
	line SetLines lines

vtkTubeFilter tube0
tube0 SetInput line
tube0 SetRadius 0.4
tube0 SetNumberOfSides 8

vtkTubeFilter tube1
tube1 SetInput line
tube1 SetRadius 0.5
tube1 SetNumberOfSides 8
	
vtkAppendPolyData merge
merge AddInput [tube0 GetOutput]
merge AddInput [tube1 GetOutput]

vtkCleanPolyData clean
clean SetInput [merge GetOutput]

vtkDelaunay3D del
del SetInput [clean GetOutput]

#Write polydata file
vtkXMLUnstructuredGridWriter writer
writer SetInput [del GetOutput]
writer SetDataModeToAscii
set filename "cylinder3D.vtu"
writer SetFileName $filename
writer Update

vtkDataSetMapper mapper
mapper SetInput [del GetOutput]

vtkActor actor
actor SetMapper mapper

# Create the standard rendering stuff.
vtkRenderer ren1
vtkRenderWindow renWin
	renWin AddRenderer ren1
vtkRenderWindowInteractor iren
	iren SetRenderWindow renWin

# Add the actors to the renderer, set the background and size
#
ren1 AddActor actor
ren1 SetBackground 1 1 1 
renWin SetSize 300 300 
renWin Render

# render the image
#
renWin Render

# prevent the tk window from showing up then start the event loop
wm withdraw .
