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
smooth.SetNumberOfIterations(100)

#Model input mesh will be adapted to the source
model = vtkesqui.vtkModel()
model.SetInput(input)
model.SetSource(smooth.GetOutput())
model.SetColor(0.5, 0.5, 1.0)
model.Init()

#A call to update method is made to force the model to be at its last state
model.Update();

ren = vtk.vtkRenderer()
renWin = vtk.vtkRenderWindow()
renWin.AddRenderer(ren)
iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

actor = model.GetActor()

m = vtk.vtkPolyDataMapper()
m.SetInput(source)
a = vtk.vtkActor()
a.SetMapper(m)
a.GetProperty().SetOpacity(0.5)
ren.AddActor(a)

ren.AddActor(actor)
ren.SetBackground(1.0, 1.0, 1.0)
renWin.SetSize(500, 500)

ren.ResetCamera()
ren.GetActiveCamera().Azimuth(45)
iren.Initialize()

renWin.Render()

iren.Start()
