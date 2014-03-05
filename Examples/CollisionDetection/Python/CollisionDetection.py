
import vtk
import vtkesqui

fn ="/home/david/vtkESQuiData/Scenario/Tools/Grasper/lever_l_col.vtp"
fn1 ="/home/david/vtkESQuiData/Scenario/Tools/Grasper/lever_r_col.vtp"
fn2 ="/home/david/vtkESQuiData/Scenario/Organs/ball_col.vtp"

# Collision Model
reader = vtk.vtkXMLPolyDataReader()
reader.SetFileName(fn)
reader.Update()

col0 = vtkesqui.vtkCollisionModel()
col0.SetName("lever_l_col")
col0.SetInput(reader.GetOutput())
col0.SetObjectId(0)
col0.SetOpacity(1.0)
col0.SetColor(1.0, 0.0, 1.0)
col0.Update()

reader1 = vtk.vtkXMLPolyDataReader()
reader1.SetFileName(fn1)
reader1.Update()

col1 = vtkesqui.vtkCollisionModel()
col1.SetName("lever_r_col")
col1.SetInput(reader1.GetOutput())
col1.SetObjectId(1)
col1.SetOpacity(1.0)
col1.SetColor(0.0, 1.0, 1.0)
col1.Update()

reader2 = vtk.vtkXMLPolyDataReader()
reader2.SetFileName(fn2)
reader2.Update()

col2 = vtkesqui.vtkCollisionModel()
col2.SetName("ball_col")
col2.SetInput(reader2.GetOutput())
col2.SetObjectId(2)
col2.SetOpacity(1.0)
col2.SetColor(1.0, 1.0, 0.0)
col2.Update()

detection = vtkesqui.vtkBioEngInterface()
detection.AddModel(col0)
detection.AddModel(col1)
detection.AddModel(col2)
detection.Initialize()
detection.Update()

ren = vtk.vtkRenderer()
renWin = vtk.vtkRenderWindow()
renWin.AddRenderer(ren)
renWin.SetWindowName("vtkESQui - Collision Detection")
iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

ren.AddActor(col0.GetActor())
ren.AddActor(col1.GetActor())
ren.AddActor(col2.GetActor())

ren.SetBackground(1.0, 1.0, 1.0)
renWin.SetSize(500, 500)

ren.ResetCamera()
ren.GetActiveCamera().Azimuth(45)
iren.Initialize()

renWin.Render()

iren.Start()
