
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
    #rate = 1/(time.time() - start)
    #print rate
    self.timer_count += 1
    if (self.timer_count % 20 == 0):
      self.scenario.Render()

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

  # Left Tool (Probe)
  # Visualization model
  vis_stick_l = vtkesqui.vtkVisualizationModel()
  vis_stick_l.SetName("vis_stick_l")
  vis_stick_l.SetFileName(fn0)
  vis_stick_l.SetTextureFileName(fn0t)
  vis_stick_l.SetOpacity(1.0)
  vis_stick_l.SetColor(1.0, 1.0, 1.0)
  
  # Collision model
  col_stick_l = vtkesqui.vtkCollisionModel()
  col_stick_l.SetName("col_stick_l_vtkbioeng")
  col_stick_l.SetFileName(fn0c)
  col_stick_l.SetOpacity(1.0)
  col_stick_l.SetColor(1.0, 1.0, 1.0)
  
  # First tool element (stick_l)
  stick_l = vtkesqui.vtkScenarioElement()
  stick_l.SetName("stick_l")
  stick_l.SetPosition(-3.0, 0.0, 0.0)
  stick_l.SetOrientation(0, 0, 20)
  stick_l.SetOrigin(0,0,6)
  stick_l.SetVisualizationModel(vis_stick_l)
  stick_l.SetCollisionModel(col_stick_l)
  
  # Visualization model
  vis_tip_l = vtkesqui.vtkVisualizationModel()
  vis_tip_l.SetName("vis_tip_l")
  vis_tip_l.SetFileName(fn3)
  vis_tip_l.SetTextureFileName(fn0t)
  vis_tip_l.SetOpacity(1.0)
  vis_tip_l.SetColor(1.0, 0.0, 0.0)
  
  # Collision model
  col_tip_l = vtkesqui.vtkCollisionModel()
  col_tip_l.SetName("col_tip_l_vtkbioeng")
  col_tip_l.SetFileName(fn3c)
  col_tip_l.SetOpacity(1.0)
  col_tip_l.SetColor(1.0, 1.0, 1.0)
  
  # Second tool element (tip_l)
  tip_l = vtkesqui.vtkScenarioElement()
  tip_l.SetName("tip_l")
  tip_l.SetPosition(-3.0, 0.0, 0.0)
  tip_l.SetOrientation(0, 0, 20)
  tip_l.SetOrigin(0,0,6)
  tip_l.SetVisualizationModel(vis_tip_l)
  tip_l.SetCollisionModel(col_tip_l)
  
  probe_l = vtkesqui.vtkToolProbe()
  probe_l.AddElement(stick_l)
  probe_l.AddElement(tip_l)
  
  # Right tool (Grasper)
  # Visualization model
  vis_stick_r = vtkesqui.vtkVisualizationModel()
  vis_stick_r.SetName("vis_stick_r")
  vis_stick_r.SetFileName(fn0)
  vis_stick_r.SetTextureFileName(fn0t)
  vis_stick_r.SetOpacity(1.0)
  vis_stick_r.SetColor(1.0, 1.0, 1.0)
  
  # Collision model
  col_stick_r = vtkesqui.vtkCollisionModel()
  col_stick_r.SetName("col_stick_r_vtkbioeng")
  col_stick_r.SetFileName(fn0c)
  col_stick_r.SetOpacity(1.0)
  col_stick_r.SetColor(1.0, 1.0, 1.0)
  
  # First tool element (stick_r)
  stick_r = vtkesqui.vtkScenarioElement()
  stick_r.SetName("stick_r")
  stick_r.SetPosition(3.0, 0.0, 0.0)
  stick_r.SetOrientation(0, 0, -20)
  stick_r.SetOrigin(0,0,6)
  stick_r.SetVisualizationModel(vis_stick_r)
  stick_r.SetCollisionModel(col_stick_r)
  
  # Visualization model
  vis_l_lever_r = vtkesqui.vtkVisualizationModel()
  vis_l_lever_r.SetName("vis_l_lever_r")
  vis_l_lever_r.SetFileName(fn1)
  vis_l_lever_r.SetTextureFileName(fn0t)
  vis_l_lever_r.SetOpacity(1.0)
  vis_l_lever_r.SetColor(0.0, 1.0, 0.0)
  
  # Collision model
  col_l_lever_r = vtkesqui.vtkCollisionModel()
  col_l_lever_r.SetName("col_l_lever_r_vtkbioeng")
  col_l_lever_r.SetFileName(fn1c)
  col_l_lever_r.SetOpacity(1.0)
  col_l_lever_r.SetColor(1.0, 1.0, 1.0)
  
  # Second tool element (l_lever_r)
  l_lever_r = vtkesqui.vtkScenarioElement()
  l_lever_r.SetName("l_lever_r")
  l_lever_r.SetPosition(3.0, 0.0, 0.0)
  l_lever_r.SetOrientation(0, 0, -20)
  l_lever_r.SetOrigin(0,0,6)
  l_lever_r.SetVisualizationModel(vis_l_lever_r)
  l_lever_r.SetCollisionModel(col_l_lever_r)
  
    # Visualization model
  vis_r_lever_r = vtkesqui.vtkVisualizationModel()
  vis_r_lever_r.SetName("vis_r_lever_r")
  vis_r_lever_r.SetFileName(fn2)
  vis_r_lever_r.SetTextureFileName(fn0t)
  vis_r_lever_r.SetOpacity(1.0)
  vis_r_lever_r.SetColor(0.0, 1.0, 0.0)
  
  # Collision model
  col_r_lever_r = vtkesqui.vtkCollisionModel()
  col_r_lever_r.SetName("col_r_lever_r_vtkbioeng")
  col_r_lever_r.SetFileName(fn2c)
  col_r_lever_r.SetOpacity(1.0)
  col_r_lever_r.SetColor(1.0, 1.0, 1.0)
  
  # Second tool element (r_lever_r)
  r_lever_r = vtkesqui.vtkScenarioElement()
  r_lever_r.SetName("r_lever_r")
  r_lever_r.SetPosition(3.0, 0.0, 0.0)
  r_lever_r.SetOrientation(0, 0, -20)
  r_lever_r.SetOrigin(0,0,6)
  r_lever_r.SetVisualizationModel(vis_r_lever_r)
  r_lever_r.SetCollisionModel(col_r_lever_r)
  
  grasper_r = vtkesqui.vtkToolGrasper()
  grasper_r.SetStick(stick_r)
  grasper_r.SetLeftLever(l_lever_r)
  grasper_r.SetRightLever(r_lever_r)
  
  # Add objects to the scenario
  scenario = vtkesqui.vtkScenario()
  scenario.AddObject(probe_l)
  scenario.AddObject(grasper_r)
  
  # Scenario camera
  camera = vtk.vtkCamera()
  camera.SetPosition(0, 0, 6)
  camera.SetFocalPoint(0, 0, -6)
  camera.Yaw(0)
  camera.Elevation(20)
  camera.Pitch(-15)
  camera.ParallelProjectionOff()
  camera.SetViewAngle(70)
  scenario.SetCamera(camera)
  
  #/********** Lights  **********/
  headLight = vtk.vtkLight()
  headLight.SetLightTypeToHeadlight()
  headLight.PositionalOn()
  headLight.SetIntensity(0.8)
  headLight.SetConeAngle(20)
  scenario.AddLight(headLight)
  
  ambientLight = vtk.vtkLight()
  ambientLight.SetIntensity(0.8)
  ambientLight.SetLightTypeToHeadlight()
  ambientLight.PositionalOff()
  scenario.AddLight(ambientLight)

  # Force the scenario to initialize
  scenario.Update()
  
  # Sign up to receive TimerEvent
  iren = scenario.GetRenderWindowInteractor()
  cb = vtkTimerCallback()
  iren.AddObserver('TimerEvent', cb.execute)
  # 1ms timer
  timerId = iren.CreateRepeatingTimer(1);
  cb.scenario = scenario
  
  iren.Start()

if __name__ == '__main__':
  main()