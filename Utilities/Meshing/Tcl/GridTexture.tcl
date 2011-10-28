package require vtk

set path "/home/jballesteros/Workspace/data/vtkESQuiData"

vtkXMLUnstructuredGridReader reader
reader SetFileName "$path/Scenario/Meshes/ellipsoid.vtu"
reader Update

set grid [ reader GetOutput ]

vtkTextureMapToSphere map
map SetInput $grid
map PreventSeamOn

vtkTransformTextureCoords xform
xform SetInputConnection [ map GetOutputPort ]
xform SetScale 1 1 1

vtkDataSetMapper mapper
mapper SetInputConnection [ xform GetOutputPort]

vtkJPEGReader jpegReader
jpegReader SetFileName "$path/Scenario/Textures/stomach.jpg"

vtkTexture texture
texture SetInputConnection [ jpegReader GetOutputPort ]
texture InterpolateOn

vtkActor actor
actor SetMapper mapper
actor SetTexture texture
actor SetScale 20

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

set camera [ren1 GetActiveCamera]
#$camera SetPosition 0 0 0

vtkLight light
light SetPositional 1
light SetConeAngle 40
light SetIntensity 0.7
light SetAmbientColor 0 0 0
light SetDiffuseColor 1 1 1
light SetSpecularColor 1 1 1
light SetLightTypeToCameraLight

ren1 AddLight light 
# render the image
#
renWin Render

# prevent the tk window from showing up then start the event loop
wm withdraw .
