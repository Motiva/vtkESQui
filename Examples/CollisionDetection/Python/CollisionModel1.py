
import vtk
import vtkesqui
import random

# Data source
sphere = vtk.vtkSphereSource()
sphere.SetPhiResolution(12)
sphere.SetThetaResolution(12)
sphere.Update()

#Define model input and source
input = sphere.GetOutput()
source = vtk.vtkPolyData()
source.DeepCopy(input)

#Model input mesh will be adapted to the source
model = vtkesqui.vtkCollisionModel()
model.SetId(0)
model.SetInput(input)
model.SetRadius(0.02)
model.DisplayCollisionsOn()
model.Initialize()

#Insert a few collisions to the model
p = [12, 23, 14, 25]
for i in p:
  c = vtkesqui.vtkCollision()
  c.SetModelId(0)
  c.SetPointId(i)
  c.SetDistance(random.random()*0.5)
  model.AddCollision(c)

model.Update()

ren = vtk.vtkRenderer()
renWin = vtk.vtkRenderWindow()
renWin.AddRenderer(ren)
iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

# Display model output (smoothed polydata)
actor = model.GetActor()
ren.AddActor(actor)

ren.SetBackground(0.3, 0.6, 0.3)
renWin.SetSize(500, 500)

ren.ResetCamera()
ren.GetActiveCamera().Azimuth(45)
iren.Initialize()

renWin.Render()

iren.Start()
