
import vtk
import vtkesqui
import time
from Scientific.Visualization.VRML2 import Camera

class vtkTimerCallback():
   def __init__(self):
       self.timer_count = 0
 
   def execute(self,obj,event):
       #print self.timer_count
       
       start = time.time()
       #self.actor.SetPosition(self.timer_count, self.timer_count,0);
       self.ball.Modified()
       self.ball.Update()
       rate = 1/(time.time() - start)
       print rate
       iren = obj
       iren.GetRenderWindow().Render()
       self.timer_count += 1

def main():

  fn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball.vtp"
  tfn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/steel.jpg"
  cfn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball_col.vtp"
  dfn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball_def_c10.vtp"

  # Visualization Model
  vis = vtkesqui.vtkVisualizationModel()
  vis.SetName("ball_vis")
  vis.SetFileName(fn)
  vis.SetTextureFileName(tfn)
  vis.SetOpacity(1.0)
  vis.SetColor(1.0, 1.0, 1.0)

  # Visualization Model
  col = vtkesqui.vtkCollisionModel()
  col.SetName("ball_vtkbioeng")
  col.SetFileName(cfn)
  col.SetOpacity(0.5)
  col.SetVisibility(1)
  col.SetColor(0.0, 0.0, 1.0)
  
  # Deformation Model
  defo = vtkesqui.vtkParticleSpringSystemInterface()
  defo.SetName("ball_defo")
  defo.SetFileName(dfn)
  defo.SetOpacity(0.5)
  defo.SetVisibility(1)
  defo.SetColor(0.0, 1.0, 0.0)
  defo.SetSpring(150)
  defo.SetDistance(1.0)
  defo.SetDamping(3)
  defo.SetMass(.5)
  defo.SetTimeStep(0.001)

  ball = vtkesqui.vtkScenarioElement()
  ball.SetId(0)
  ball.SetName("ball")
  ball.SetPosition(3.0, 0.0, 0.0)
  ball.SetOrientation(0, 0, -20)
  ball.SetVisualizationModel(vis)
  ball.SetCollisionModel(col)
  ball.SetDeformationModel(defo)

  ball.Update()

  # Add a collision
  defo.InsertDisplacement(19, 0.2, 0.05, 0.0)
  defo.InsertDisplacement(20, 0.35, 0.05, 0.0)
  defo.InsertDisplacement(21, 0.2, 0.05, 0.0)

  ren = vtk.vtkRenderer()
  renWin = vtk.vtkRenderWindow()
  renWin.AddRenderer(ren)
  iren = vtk.vtkRenderWindowInteractor()
  iren.SetRenderWindow(renWin)

  ren.AddActor(vis.GetActor())
  ren.AddActor(col.GetActor())
  ren.AddActor(defo.GetActor())

  ren.SetBackground(0.8, 0.8, 0.9)
  renWin.SetSize(500, 500)

  ren.ResetCamera()
  camera = ren.GetActiveCamera()
  camera.Azimuth(15)
  camera.Zoom(1.6)
  
  iren.Initialize()

  # Sign up to receive TimerEvent
  cb = vtkTimerCallback()
  cb.ball = ball
  iren.AddObserver('TimerEvent', cb.execute)
  timerId = iren.CreateRepeatingTimer(1);
  cb.renderTimerId = timerId
  
  
  iren.Start()
  
if __name__ == '__main__':
  main()
  