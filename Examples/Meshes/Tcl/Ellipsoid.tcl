package require vtk

vtkParametricEllipsoid ellipsoid
ellipsoid SetXRadius 1.5
ellipsoid SetYRadius 1.0 
ellipsoid SetZRadius 1.25 

vtkParametricFunctionSource ellipsoidSource
	ellipsoidSource SetParametricFunction ellipsoid
	ellipsoidSource SetScalarModeToV

set data [ ellipsoidSource GetOutput ]

# Write the polydata file 
vtkXMLPolyDataWriter writer
writer SetDataModeToAscii
writer SetInput $data
writer SetFileName "ellipsoid.vtp"
writer Write

vtkPolyDataMapper mapper
mapper SetInput $data
#mapper SetScalarRange 0 3.14

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
