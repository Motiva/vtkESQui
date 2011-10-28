package require vtk

vtkSphereSource sphereSource
sphereSource Update

vtkPolyDataPointSampler pointSampler
pointSampler SetDistance 0.1
pointSampler SetInputConnection [ sphereSource GetOutputPort ]
pointSampler Update

vtkPolyDataMapper sphereMapper
sphereMapper SetInputConnection [ sphereSource GetOutputPort ]

vtkActor sphereActor
sphereActor SetMapper sphereMapper

vtkPolyDataMapper sampleMapper
sampleMapper SetInputConnection [ pointSampler GetOutputPort ]

vtkActor sampleActor
sampleActor SetMapper sampleMapper
[ sampleActor GetProperty ] SetColor 1.0 0.0 0.0

vtkRenderer renderer
vtkRenderWindow renderWindow
renderWindow AddRenderer renderer
vtkRenderWindowInteractor iren
iren SetRenderWindow renderWindow

renderer AddActor sphereActor
renderer AddActor sampleActor
renderer SetBackground 0.3 0.6 0.3

iren Initialize

iren AddObserver UserEvent {wm deiconify .vtkInteract}

# prevent the tk window from showing up then start the event loop
wm withdraw .
