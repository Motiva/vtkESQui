
import vtk
import vtkesqui

fn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball.vtp"
fnc ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball_col.vtp"
fnd ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball_def_c10.vtp"
fntl = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/leftball.jpg";
fntr = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/rightball.jpg";
fntb = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/bothball.jpg";

ren = vtk.vtkRenderer()
renWin = vtk.vtkRenderWindow()
renWin.AddRenderer(ren)
iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

reader = vtk.vtkXMLPolyDataReader()
reader.SetFileName(fn)
reader.Update()

readerc = vtk.vtkXMLPolyDataReader()
readerc.SetFileName(fnc)
readerc.Update()

# Organs
balls = list()
textures = list()
textures.append(fntl)
textures.append(fntr)
textures.append(fntb)

for i in range(9):
  
  # Define organ type
  # 3 organs of each type (left, right and both)
  t = i%3
  px = int(i/3)
  
  # Add an Organ
  # Visualization model
  vis_ball = vtkesqui.vtkVisualizationModel()
  vis_ball.SetName("vis_ball"+str(i))
  vis_ball.SetInput(reader.GetOutput())
  vis_ball.SetTextureFileName(textures[t])
  vis_ball.SetOpacity(1.0)

  # Collision model
  col_ball = vtkesqui.vtkCollisionModel()
  col_ball.SetName("col_ball_vtkbioeng"+str(i))
  col_ball.SetInput(readerc.GetOutput())
  col_ball.SetOpacity(1.0)

  # Organ element (ball)
  #x = -3 + i*2.0
  e_ball = vtkesqui.vtkScenarioElement()
  e_ball.SetName("e_ball"+str(i))
  e_ball.SetPosition(2*px-2, -2*t, -3)
  e_ball.SetOrientation(15, -0, 0)
  e_ball.SetVisualizationModel(vis_ball)
  e_ball.SetCollisionModel(col_ball)

  ball = vtkesqui.vtkOrgan()
  ball.SetName("ball_"+str(i)+"_"+str(t))
  ball.AddElement(e_ball)
  ball.Update()
  
  ren.AddActor(vis_ball.GetActor())
  ren.AddActor(col_ball.GetActor())

ren.SetBackground(1.0, 1.0, 1.0)
renWin.SetSize(500, 500)

ren.ResetCamera()
ren.GetActiveCamera().Azimuth(45)
iren.Initialize()

renWin.Render()

iren.Start()