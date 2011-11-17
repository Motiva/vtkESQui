
import vtk
import vtkesqui
import time

class vtkTimerCallback():
   def __init__(self):
       self.timer_count = 0
 
   def execute(self,obj,event):
       #print self.timer_count
       
       start = time.time()
       self.scenario.Update()
       #self.tool.Update()
       #iren = obj
       #iren.GetRenderWindow().Render()
       rate = 1/(time.time() - start)
       print rate
       self.timer_count += 1

def main():

  fn0 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/stick.vtp";
  fn0c = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/stick_col.vtp";
  fn1 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/lever_l.vtp";
  fn1c = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/lever_l_col.vtp";
  fn2 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/lever_r.vtp";
  fn2c = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/lever_r_col.vtp";
  fn0t = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/metal.jpg";
  fn3 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/tip.vtp";
  fn3c = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/tip_col.vtp";


  # Create visualization window
  ren = vtk.vtkRenderer()
  renWin = vtk.vtkRenderWindow()
  renWin.AddRenderer(ren)
  iren = vtk.vtkRenderWindowInteractor()
  iren.SetRenderWindow(renWin)
  
  ren.SetBackground(1.0, 1.0, 1.0)
  renWin.SetSize(800, 600)

  # Visualization Model
  vis_stick = vtkesqui.vtkVisualizationModel()
  vis_stick.SetName("vis_stick")
  vis_stick.SetFileName(fn0)
  vis_stick.SetTextureFileName(fn0t)
  vis_stick.SetOpacity(1.0)
  vis_stick.SetColor(1.0, 1.0, 1.0)
  
  # Visualization Model
  col_stick = vtkesqui.vtkCollisionModel()
  col_stick.SetName("col_stick_vtkbioeng")
  col_stick.SetFileName(fn0c)
  col_stick.SetOpacity(1.0)
  col_stick.SetColor(0.0, 0.0, 1.0)
  
  stick = vtkesqui.vtkScenarioElement()
  stick.SetName("stick")
  stick.SetPosition(-3.0, 0.0, 0.0)
  stick.SetOrientation(0, 0, 20)
  stick.SetVisualizationModel(vis_stick)
  stick.SetCollisionModel(col_stick)
  
  # Visualization Model
  vis_tip = vtkesqui.vtkVisualizationModel()
  vis_tip.SetName("vis_tip")
  vis_tip.SetFileName(fn3)
  vis_tip.SetTextureFileName(fn0t)
  vis_tip.SetOpacity(1.0)
  vis_tip.SetColor(1.0, 0.0, 0.0)
  
  # Visualization Model
  col_tip = vtkesqui.vtkCollisionModel()
  col_tip.SetName("col_tip_vtkbioeng")
  col_tip.SetFileName(fn3c)
  col_tip.SetOpacity(1.0)
  col_tip.SetColor(0.0, 0.0, 1.0)
  
  tip = vtkesqui.vtkScenarioElement()
  tip.SetName("tip")
  tip.SetPosition(-3.0, 0.0, 0.0)
  tip.SetOrientation(0, 0, 20)
  tip.SetVisualizationModel(vis_tip)
  tip.SetCollisionModel(col_tip)
  
  probe = vtkesqui.vtkOrgan()
  probe.AddElement(stick)
  probe.AddElement(tip)
  
  scenario = vtkesqui.vtkScenario()
  scenario.AddObject(probe)

  scenario.SetRenderWindow(renWin)
  scenario.Update()
  
  ren.ResetCamera()
  camera = ren.GetActiveCamera()
  camera.SetPosition(0, 0, 6)
  camera.SetFocalPoint(0, 0, -6)
  camera.Yaw(0)
  camera.Elevation(20)
  camera.Pitch(-15)
  camera.ParallelProjectionOff()
  camera.SetViewAngle(70)
  
  iren.Initialize()
  
   # Sign up to receive TimerEvent
  cb = vtkTimerCallback()
  cb.scenario = scenario
  cb.tool = probe
  iren.AddObserver('TimerEvent', cb.execute)
  timerId = iren.CreateRepeatingTimer(20);
  cb.renderTimerId = timerId
  
  iren.Start()

if __name__ == '__main__':
  main()