
import vtk
import vtkesqui
import time
import random

class vtkTimerCallback():
  def __init__(self):
    self.timer_count = 0
  
  def execute(self,obj,event):
    start = time.time()
    self.ball.Modified()
    self.ball.Update()
    rate = 1/(time.time() - start)
    #print rate

    iren = obj
    iren.GetRenderWindow().Render()
    
    if (self.timer_count % 500 == 0):
      print "collision"
      
      cm = self.ball.GetCollisionModel()
      normals = cm.GetCellNormals()
      cid = 26
      cps = [14,5,6]
      cn = normals.GetTuple3(cid)
      for i in cps:
        pn = cm.GetPointNormals().GetTuple3(i)
        c = vtkesqui.vtkCollision()
        c.SetObjectId(0)
        c.SetModelId(0)
        c.SetCellId(cid)
        c.SetCellNormal(cn)
        c.SetPointId(i)
        c.SetPoint(cm.GetOutput().GetPoint(i))
        d = 0.2*random.random()
        dsp = [0.0,0.0,0.0]
        j = 0
        for n in pn:
          dsp[j] = d*n
          j = j+1

        c.SetPointDisplacement(dsp)
        c.SetDistance(d)

        cm.AddCollision(c)

    self.timer_count += 1

def main():

  fn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball.vtp"
  tfn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/muscle.jpg"
  cfn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball_col.vtp"
  dfn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball_def_c10.vtp"

  # Visualization Model
  vis = vtkesqui.vtkVisualizationModel()
  vis.SetName("ball_vis")
  vis.SetFileName(fn)
  vis.SetTextureFileName(tfn)

  # Visualization Model
  col = vtkesqui.vtkCollisionModel()
  col.SetName("ball_vtkbioeng")
  col.SetFileName(cfn)
  col.SetRadius(0.01)
  col.SetVisibility(1)
  col.DisplayCollisionsOn()
  
  # Deformation Model
  defo = vtkesqui.vtkParticleSpringSystemInterface()
  defo.SetName("ball_defo")
  defo.SetFileName(dfn)
  defo.SetOpacity(0.8)
  defo.SetVisibility(1)
  defo.SetColor(.1, .1, .1)
  defo.SetSpring(150)
  defo.SetDistance(1.0)
  defo.SetDamping(2)
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

  ren = vtk.vtkRenderer()
  renWin = vtk.vtkRenderWindow()
  renWin.AddRenderer(ren)
  renWin.SetWindowName("vtkESQui - Particle Spring System")
  iren = vtk.vtkRenderWindowInteractor()
  iren.SetRenderWindow(renWin)

  ren.AddActor(vis.GetActor())
  ren.AddActor(col.GetActor())
  ren.AddActor(defo.GetActor())

  ren.SetBackground(0.3, 0.6, 0.3)
  renWin.SetSize(800, 600)

  ren.ResetCamera()
  camera = ren.GetActiveCamera()
  camera.Azimuth(15)
  camera.Zoom(1.6)
  
  iren.Initialize()

  # Sign up to receive TimerEvent
  cb = vtkTimerCallback()
  cb.ball = ball
  iren.AddObserver('TimerEvent', cb.execute)
  timerId = iren.CreateRepeatingTimer(1)
  cb.renderTimerId = timerId
  
  iren.Start()
  
if __name__ == '__main__':
  main()
  
