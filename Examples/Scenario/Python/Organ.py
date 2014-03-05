
import vtk
import vtkesqui

fn ="/home/david/vtkESQuiData/Scenario/Organs/ball.vtp"
tfn ="/home/david/vtkESQuiData/Scenario/Textures/steel.jpg"
cfn ="/home/david/vtkESQuiData/Scenario/Organs/ball_col.vtp"
dfn ="/home/david/vtkESQuiData/Scenario/Organs/ball_def_c10.vtp"

# Visualization Model
vis = vtkesqui.vtkVisualizationModel()
vis.SetName("ball_vis")
vis.SetFileName(fn)
vis.SetTextureFileName(tfn)
vis.SetOpacity(1.0)
vis.SetColor(1.0, 0.0, 1.0)

# Visualization Model
col = vtkesqui.vtkCollisionModel()
col.SetName("ball_vtkbioeng")
col.SetFileName(cfn)
col.SetOpacity(1.0)
col.SetColor(0.0, 0.0, 1.0)

# Deformation Model
defo = vtkesqui.vtkParticleSpringSystemInterface()
defo.SetName("ball_ParticleSpringSystem")
defo.SetFileName(dfn)
defo.SetOpacity(0.5)
defo.SetColor(1.0, 1.0, 1.0)

ball = vtkesqui.vtkScenarioElement()
ball.SetId(0)
ball.SetName("ball")
ball.SetPosition(3.0, 0.0, 0.0)
ball.SetOrientation(0, 0, -20)
ball.SetVisualizationModel(vis)
ball.SetCollisionModel(col)
ball.SetDeformationModel(defo)

organ = vtkesqui.vtkOrgan()
organ.AddElement(ball)

organ.Update()

ren = vtk.vtkRenderer()
renWin = vtk.vtkRenderWindow()
renWin.AddRenderer(ren)
iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

ren.AddActor(vis.GetActor())
ren.AddActor(col.GetActor())
ren.AddActor(defo.GetActor())

ren.SetBackground(1.0, 1.0, 1.0)
renWin.SetSize(500, 500)

ren.ResetCamera()
ren.GetActiveCamera().Azimuth(45)
iren.Initialize()

renWin.Render()

iren.Start()
