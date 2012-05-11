
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
    self.items = list()
    self.organs = 0
    
    #Movement parameters
    self.steps = list()
    self.angles = list()
    
    self.steps.append(0.01)
    self.steps.append(0.015)
    self.steps.append(0.01)
    self.steps.append(0.01)
    self.steps.append(0.008)
    self.steps.append(0.01)
    self.steps.append(0.008)
    self.steps.append(0.01)
    self.steps.append(0.015)
    self.angles.append(0.40)
    self.angles.append(0.35)
    self.angles.append(0.40)
    self.angles.append(0.25)
    self.angles.append(0.35)
    self.angles.append(0.2)
    self.angles.append(0.40)
    self.angles.append(0.25)
    self.angles.append(0.5)
 
  def execute(self,obj,event):
    #print self.timer_count
    
    #Next round timer (20s)
    if (self.timer_count % 4000 == 0):
      self.nextRound()
    
    self.updateRound()
    
    #start = time.time()
    #self.organs
    #rate = 1/(time.time() - start)
    #print rate
    self.timer_count += 1

  def nextRound(self):
    self.round += 1
    
    r = self.round
    items = self.items
    organs = self.organs
    #Disable all scene organs
    for o in organs:
      o.Disable()
  
    #Clear active items
    del items[:]
  
    act = []
    if r == 1:
      print "## first round ##"
      act = [0, 1]
    elif r == 2:
      print "## second round ##"
      act = [1, 2, 4]
    elif r == 3:
      print "## third round ##"
      act = [1, 3, 4, 6]
    elif r == 4:
      print "## fourth round ##"
      act = [1, 3, 4, 5, 6, 8]
    else:
      act = []
      print "end"
      exit()
  
    print "Balls: "+str(len(items))+"/"+str(len(organs))
    
    #Activate organs
    for a in act:
      print str(a)+":"+organs[a].GetName()
      organs[a].Enable()
      items.append(organs[a])

  def updateRound(self):
    r = self.round
    items = self.items
    organs = self.organs
    steps = self.steps
    angles = self.angles
    
    #Check collisions object ids
    colls = self.simulation.GetCollisionPairs()
    obIds = list()
    left = -1
    right = -1
    if (colls and colls.GetNumberOfTuples() > 0):
      for i in range(colls.GetNumberOfTuples()):
        c = colls.GetTuple2(i)
        id0 = int(c[0])
        id1 = int(c[1])

        #Get tool and organ ids 
        if id0 == 0 or id0 == 1:
          tid = id0
          oid = id1
        elif id1 == 0 or id1 == 1:
          tid = id1
          oid = id0
     
        organ = self.organs[(oid-3)]       

        if tid == 1:
          #right (green) tool
          right = oid
          if oid%3 == 0:
            #green ball
            organ.Disable()
          if oid%3 == 2 and left >= 0:
            #blue ball
            organ.Disable()
            
        elif tid == 0:
          #left (red) tool
          left = oid
          if oid%3 == 1:
            #red ball
            organ.Disable()
          if oid%3 == 2 and right >= 0:
            #blue ball
            organ.Disable()
    
    e = 0
    for i in range(len(items)):
      item = items[i]
      s = steps[i]
      a = angles[i]
      item.Translate(s, 0.0, 0.0)
      item.RotateY(a)
      item.Update()
      if(item.IsEnabled()):
        e = e+1
    
    #No active items
    if e == 0:
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
  fn4tr = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/rightball.jpg";
  fn4tb = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/bothball.jpg";
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
  stick_l.SetPosition(-4.0, 0.0, 0.0)
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
  col_tip_l.SetVisibility(0.0)
  
  # Second tool element (tip_l)
  tip_l = vtkesqui.vtkScenarioElement()
  tip_l.SetName("tip_l")
  tip_l.SetPosition(-4.0, 0.0, 0.0)
  tip_l.SetOrientation(0, 0, 20)
  tip_l.SetOrigin(0,0,6)
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
  stick_r.SetPosition(4.0, 0.0, 0.0)
  stick_r.SetOrientation(0, 0, 20)
  stick_r.SetOrigin(0,0,6)
  stick_r.SetVisualizationModel(vis_stick_r)
  stick_r.SetCollisionModel(col_stick_r)
  
  # Visualization model
  vis_tip_r = vtkesqui.vtkVisualizationModel()
  vis_tip_r.SetName("vis_tip_r")
  vis_tip_r.SetFileName(fn3)
  vis_tip_r.SetTextureFileName(fn0t)
  vis_tip_r.SetOpacity(1.0)
  vis_tip_r.SetColor(0.0, 1.0, 0.0)
  
  # Collision model
  col_tip_r = vtkesqui.vtkCollisionModel()
  col_tip_r.SetName("col_tip_r_vtkbioeng")
  col_tip_r.SetFileName(fn3c)
  col_tip_r.SetVisibility(0.0)
  
  # Second tool element (tip_r)
  tip_r = vtkesqui.vtkScenarioElement()
  tip_r.SetName("tip_r")
  tip_r.SetPosition(4.0, 0.0, 0.0)
  tip_r.SetOrientation(0, 0, 20)
  tip_r.SetOrigin(0,0,6)
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
  e_cavity.SetPosition(0.0, 0.0, -3.0)
  e_cavity.SetOrientation(-45, 0, 0)
  e_cavity.SetVisualizationModel(vis_cavity)
  
  cavity = vtkesqui.vtkOrgan()
  cavity.SetName("cavity")
  cavity.AddElement(e_cavity)
  scenario.AddObject(cavity)
  
  # Organs
  balls = list()
  textures = list()
  textures.append(fn4tl)
  textures.append(fn4tr)
  textures.append(fn4tb)
  
  reader = vtk.vtkXMLPolyDataReader()
  reader.SetFileName(fn4)
  reader.Update()
  
  readerc = vtk.vtkXMLPolyDataReader()
  readerc.SetFileName(fn4c)
  readerc.Update()
  
  for i in range(9):
    
    # Define organ type
    # 3 organs of each type (left, right and both)
    t = i%3
    px = int(i/3)
    
    # Add a ball
    # Visualization model
    vis_ball = vtkesqui.vtkVisualizationModel()
    vis_ball.SetName("vis_ball"+str(i))
    vis_ball.SetFileName(fn4)
    vis_ball.SetTextureFileName(textures[t])
    vis_ball.SetOpacity(1.0)
  
    # Collision model
    col_ball = vtkesqui.vtkCollisionModel()
    col_ball.SetName("col_ball_vtkbioeng"+str(i))
    col_ball.SetFileName(fn4c)
    col_ball.SetVisibility(0.0)
    col_ball.SetOpacity(0.0)

    # Organ element (ball)
    #x = -3 + i*2.0
    e_ball = vtkesqui.vtkScenarioElement()
    e_ball.SetName("e_ball"+str(i))
    e_ball.SetPosition(px-1, -2*t, -3)
    e_ball.SetOrientation(15, -0, 0)
    e_ball.SetVisualizationModel(vis_ball)
    e_ball.SetCollisionModel(col_ball)
  
    ball = vtkesqui.vtkOrgan()
    ball.SetName("ball_"+str(i)+"_"+str(t))
    ball.AddElement(e_ball)
    ball.Update()
    ball.Disable()

    scenario.AddObject(ball)
    balls.append(ball)
  
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
  headLight.SetIntensity(0.8)
  headLight.SetConeAngle(60)
  scenario.AddLight(headLight)
  
  ambientLight = vtk.vtkLight()
  ambientLight.SetLightTypeToSceneLight()
  ambientLight.PositionalOff()
  ambientLight.SetIntensity(0.1)
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
  cb.organs = balls
  
  simulation.Run()
  
if __name__ == '__main__':
  main()
