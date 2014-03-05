import vtk
import vtkesqui

#Data source
sphere = vtk.vtkSphereSource()
sphere.SetPhiResolution(12)
sphere.SetThetaResolution(12)
sphere.Update()

input = sphere.GetOutput()

#Define model input
source = vtk.vtkPolyData()
source.DeepCopy(input)
points = source.GetPoints()
p = source.GetPoint(10)

points.SetPoint(10, p[0]+0.1, p[1], p[2])
#p[0] = p[0] + 0.1
#p[1] = p[1] + 0.1

smooth = vtk.vtkSmoothPolyDataFilter()
smooth.SetInput(input)
smooth.SetSource(source)
smooth.SetNumberOfIterations(1)

#A call to update method is made to force the model to be at its last state
smooth.Update();

ren = vtk.vtkRenderer()
renWin = vtk.vtkRenderWindow()
renWin.AddRenderer(ren)
iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

m0 = vtk.vtkPolyDataMapper()
m0.SetInput(smooth.GetOutput())
a0 = vtk.vtkActor()
a0.SetMapper(m0)
a0.GetProperty().SetOpacity(1)
a0.GetProperty().SetColor(0.5, 0.5, 1)
ren.AddActor(a0)

m = vtk.vtkPolyDataMapper()
m.SetInput(input)
a = vtk.vtkActor()
a.SetMapper(m)
a.GetProperty().SetOpacity(0.5)
ren.AddActor(a)

ren.SetBackground(1.0, 1.0, 1.0)
renWin.SetSize(500, 500)

ren.ResetCamera()
ren.GetActiveCamera().Azimuth(45)
iren.Initialize()

renWin.Render()

iren.Start()
