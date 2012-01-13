
##
# Precision & Speed Laparoscopy exercise. The objective of this exercise is to
# touch the balls with the appropiate tool (red or green).
##

import vtk
import vtkesqui
import time

class vtkTimerCallback():
  def __init__(self):
    self.timer_count = 0
    self.round = 0
    self.simulation = 0
    self.tid = 0
    self.target = 0
    self.pos = list()
    
    self.pos.append([0,0,-5])
    self.pos.append([2,-3,1])
    self.pos.append([-4,2,-2])
    self.pos.append([4,2,0])
    self.pos.append([2,-3,1])
    self.pos.append([-4,2,-2])
    self.pos.append([4,2,1])
    self.pos.append([-8,-4,0])
    
  def execute(self,obj,event):
    #print self.timer_count
    
    #Next round timer (20s)
    if (self.timer_count % 10000 == 0):
      self.nextRound()
    
    self.updateRound()
    
    #start = time.time()
    #self.organs
    #rate = 1/(time.time() - start)
    #print rate
    self.timer_count += 1

  def nextRound(self):
    self.round += 1
    if self.tid == 0:
      self.tid = 1
    else:
      self.tid = 0
    
    r = self.round
    o = self.target
    if r > 8:
      exit()

    for i in range(2):
      
      #Select active tool
      t = self.simulation.GetScenario().GetObject(i)
      #Change tip color
      rgb = [1.0, 1.0, 1.0]
      if i == self.tid:
        rgb = [1.0,1.0,0.2]

      tip = t.GetElement(1)
      tip.GetVisualizationModel().SetColor(rgb[0],rgb[1],rgb[2])

    #Translate target
    print "## round "+str(r)+" ##"
    o.Enable()
    p = self.pos[r-1] 
    
    o.Translate(p[0],p[1],p[2])
    o.Update()

  def updateRound(self):
    r = self.round
    o = self.target
    
    #Check collisions
    colls = self.simulation.GetCollisions()
    if (colls and colls.GetNumberOfItems() > 0):
      for i in range(colls.GetNumberOfItems()):
        c = colls.GetCollision(i)
        id0 = c.GetObjectId(0)
        id1 = c.GetObjectId(1)
        #Check first tool (Left)
        if c.GetObjectType(0) == 0:
          tid = id0
        else:
          tid = id1
        
        if self.tid == tid:
          print "remove: "+str(tid)
          o.Disable()
          self.nextRound()
    
    return r
    
def main():

  fn0 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/stick.vtp";
  fn0c = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/stick_col.vtp";
  fn0t = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/metal.jpg";
  fn3 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/tip.vtp";
  fn3c = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/tip_col.vtp";
  fn4 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball.vtp";
  fn4c = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball_col_bb.vtp";
  fn4tl = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/leftball.jpg";
  fn5 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/cavity.vtp";
  fn5t = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/intestine2.jpg";

  #Create scenario
  scenario = vtkesqui.vtkScenario()
  scenario.SetName("vtkESQui - Laparoscopy (Precission & Speed)")

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
  col_stick_l.SetVisibility(0.0)
  
  # First tool element (stick_l)
  stick_l = vtkesqui.vtkScenarioElement()
  stick_l.SetName("stick_l")
  stick_l.SetPosition(-3.0, 0.0, 0.0)
  stick_l.SetOrientation(0, 0, 15)
  stick_l.SetOrigin(0,0,5)
  stick_l.SetVisualizationModel(vis_stick_l)
  stick_l.SetCollisionModel(col_stick_l)
  
  # Visualization model
  vis_tip_l = vtkesqui.vtkVisualizationModel()
  vis_tip_l.SetName("vis_tip_l")
  vis_tip_l.SetFileName(fn3)
  vis_tip_l.SetTextureFileName(fn0t)
  vis_tip_l.SetOpacity(1.0)
  
  # Collision model
  col_tip_l = vtkesqui.vtkCollisionModel()
  col_tip_l.SetName("col_tip_l_vtkbioeng")
  col_tip_l.SetFileName(fn3c)
  col_tip_l.SetVisibility(0.0)
  
  # Second tool element (tip_l)
  tip_l = vtkesqui.vtkScenarioElement()
  tip_l.SetName("tip_l")
  tip_l.SetPosition(-3.0, 0.0, 0.0)
  tip_l.SetOrientation(0, 0, 15)
  tip_l.SetOrigin(0,0,5)
  tip_l.SetVisualizationModel(vis_tip_l)
  tip_l.SetCollisionModel(col_tip_l)
  
  probe_l = vtkesqui.vtkToolProbe()
  probe_l.AddElement(stick_l)
  probe_l.AddElement(tip_l)
  scenario.AddObject(probe_l)
  
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
  col_stick_r.SetVisibility(0.0)
  
  # First tool element (stick_r)
  stick_r = vtkesqui.vtkScenarioElement()
  stick_r.SetName("stick_r")
  stick_r.SetPosition(3.0, 0.0, 0.0)
  stick_r.SetOrientation(0, 0, 15)
  stick_r.SetOrigin(0,0,5)
  stick_r.SetVisualizationModel(vis_stick_r)
  stick_r.SetCollisionModel(col_stick_r)
  
  # Visualization model
  vis_tip_r = vtkesqui.vtkVisualizationModel()
  vis_tip_r.SetName("vis_tip_r")
  vis_tip_r.SetFileName(fn3)
  vis_tip_r.SetTextureFileName(fn0t)
  vis_tip_r.SetOpacity(1.0)
  
  # Collision model
  col_tip_r = vtkesqui.vtkCollisionModel()
  col_tip_r.SetName("col_tip_r_vtkbioeng")
  col_tip_r.SetFileName(fn3c)
  col_tip_r.SetVisibility(0.0)
  
  # Second tool element (tip_r)
  tip_r = vtkesqui.vtkScenarioElement()
  tip_r.SetName("tip_r")
  tip_r.SetPosition(3.0, 0.0, 0.0)
  tip_r.SetOrientation(0, 0, 15)
  tip_r.SetOrigin(0,0,5)
  tip_r.SetVisualizationModel(vis_tip_r)
  tip_r.SetCollisionModel(col_tip_r)
  
  probe_r = vtkesqui.vtkToolProbe()
  probe_r.AddElement(stick_r)
  probe_r.AddElement(tip_r)
  scenario.AddObject(probe_r)
  
  # Add an Organ (Cavity)
  # Visualization model
  vis_cavity = vtkesqui.vtkVisualizationModel()
  vis_cavity.SetName("vis_cavity")
  vis_cavity.SetFileName(fn5)
  vis_cavity.SetTextureFileName(fn5t)
  vis_cavity.SetOpacity(1.0)
  
  # Organ element (cavity)
  e_cavity = vtkesqui.vtkScenarioElement()
  e_cavity.SetName("e_cavity")
  e_cavity.SetPosition(0.0, 0.0, -1.0)
  e_cavity.SetOrientation(-45, 0, 0)
  e_cavity.SetVisualizationModel(vis_cavity)
  
  cavity = vtkesqui.vtkOrgan()
  cavity.SetName("cavity")
  cavity.AddElement(e_cavity)
  scenario.AddObject(cavity)
  
   # Add a ball
  # Visualization model
  vis_ball = vtkesqui.vtkVisualizationModel()
  vis_ball.SetName("vis_ball")
  vis_ball.SetFileName(fn4)
  vis_ball.SetColor(1.0,1.0,0.2)
  vis_ball.SetOpacity(1.0)
  
  # Collision model
  col_ball = vtkesqui.vtkCollisionModel()
  col_ball.SetName("col_ball_vtkbioeng")
  col_ball.SetFileName(fn4c)
  col_ball.SetVisibility(0.0)
  col_ball.SetOpacity(0.0)

  # Organ element (ball)
  e_ball = vtkesqui.vtkScenarioElement()
  e_ball.SetName("e_ball")
  e_ball.SetVisualizationModel(vis_ball)
  e_ball.SetCollisionModel(col_ball)
  
  ball = vtkesqui.vtkOrgan()
  ball.SetName("ball")
  ball.AddElement(e_ball)
 
  scenario.AddObject(ball)

  # Scenario camera
  camera = vtk.vtkCamera()
  camera.SetPosition(0, 0, 6)
  camera.SetFocalPoint(0, 0, -6)
  camera.Yaw(0)
  camera.Elevation(20)
  camera.Pitch(-15)
  camera.ParallelProjectionOff()
  camera.SetViewAngle(70)
  camera.Dolly(1.2)
  scenario.SetCamera(camera)
  
  #/********** Lights  **********/
  headLight = vtk.vtkLight()
  headLight.SetLightTypeToHeadlight()
  headLight.PositionalOn()
  headLight.SetIntensity(0.7)
  headLight.SetConeAngle(90)
  scenario.AddLight(headLight)
  
  ambientLight = vtk.vtkLight()
  ambientLight.SetLightTypeToSceneLight()
  ambientLight.PositionalOff()
  ambientLight.SetPosition(0,1,1)
  ambientLight.SetIntensity(0.5)
  scenario.AddLight(ambientLight)

  simulation = vtkesqui.vtkSimulation()
  simulation.SetScenario(scenario)
  simulation.SetRenderTimerRate(30)
  simulation.SetSimulationTimerRate(1)
  simulation.SetInteractionTimerRate(1)
  simulation.InteractionOn()
  simulation.CollisionOn()
  simulation.Initialize()
  
  #Wait for initialization to set collision mode
  simulation.SetCollisionModeToSimple()
  
  # Sign up to receive TimerEvent
  iren = scenario.GetRenderWindowInteractor()
  cb = vtkTimerCallback()
  iren.AddObserver('TimerEvent', cb.execute)
  # 100ms timer
  timerId = iren.CreateRepeatingTimer(100);
  cb.simulation = simulation
  cb.target = ball
  
  simulation.Run()
  
if __name__ == '__main__':
  main()
