import vtk
import vtkesqui

#Data source
sphere = vtk.vtkSphereSource()
sphere.SetPhiResolution(12)
sphere.SetThetaResolution(12)
sphere.Update()

input = sphere.GetOutput()

#Define model source
cube = vtk.vtkCubeSource()
cube.SetXLength(0.6)
cube.SetYLength(0.6)
cube.SetZLength(0.6)
cube.Update()

source = cube.GetOutput()

#Model input mesh will be adapted to the source
model = vtkesqui.vtkModel()
model.SetInput(input)
model.SetSource(source)
model.SetColor(0.5, 0.5, 1.0)

#A call to update method is made to force the model to be at its last state
model.Update();

ren = vtk.vtkRenderer()
renWin = vtk.vtkRenderWindow()
renWin.AddRenderer(ren)
iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

actor = model.GetActor()

m = vtk.vtkPolyDataMapper()
m.SetInput(input)
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
