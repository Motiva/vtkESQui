import vtk
import vtkesqui

fn1 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/lever_l.vtp";

#Data source (grasper lever)
reader = vtk.vtkXMLPolyDataReader()
reader.SetFileName(fn1)
reader.Update()

source = reader.GetOutput()

#Define model input
sphere1 = vtk.vtkSphereSource()
sphere1.SetPhiResolution(8)
sphere1.SetThetaResolution(8)
sphere1.SetRadius(3)
sphere1.Update()

input = sphere1.GetOutput()

syncfilter = vtkesqui.vtkSyncPolyDataFilter()
syncfilter.SetInput(input)
syncfilter.SetSource(source)
syncfilter.Update()

ren = vtk.vtkRenderer()
renWin = vtk.vtkRenderWindow()
renWin.AddRenderer(ren)
iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

m0 = vtk.vtkPolyDataMapper()
m0.SetInput(source)
a0 = vtk.vtkActor()
a0.SetMapper(m0)
a0.GetProperty().SetOpacity(0.5)
a0.GetProperty().SetColor(0.5, 0.5, 1)
#ren.AddActor(a0)

m = vtk.vtkPolyDataMapper()
m.SetInput(syncfilter.GetOutput())
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
