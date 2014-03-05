
import vtk
import vtkesqui

# Data source
sphere = vtk.vtkSphereSource()
sphere.SetPhiResolution(12)
sphere.SetThetaResolution(12)
sphere.Update()

#Define model input and source
input = sphere.GetOutput()
source = vtk.vtkPolyData()
source.DeepCopy(input)

#Smooth (deform) source mesh
smooth = vtk.vtkSmoothPolyDataFilter()
smooth.SetInput(source)
smooth.SetNumberOfIterations(200)

#Model input mesh will be adapted to the source
model = vtkesqui.vtkCollisionModel()
model.SetInput(input)
model.SetSource(smooth.GetOutput())
model.SetColor(0.5 ,0.5 , 1.0)
model.SetRadius(0.01)

#A call to update method is made to force the model to be at its last state
model.Update();

ren = vtk.vtkRenderer()
renWin = vtk.vtkRenderWindow()
renWin.AddRenderer(ren)
iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

# Display original mesh
m = vtk.vtkPolyDataMapper()
m.SetInput(source)
a = vtk.vtkActor()
a.SetMapper(m)
a.GetProperty().SetOpacity(0.5)
ren.AddActor(a)

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
