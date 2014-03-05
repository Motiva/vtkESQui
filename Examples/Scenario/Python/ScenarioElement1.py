#!/usr/bin/env python

import sys
import vtk
import vtkesqui

class vtkTimerCallback():
  def __init__(self):
    self.timer_count = 0
    self.round = 1
 
  def execute(self,obj,event):
    #print self.timer_count
    
    #Round Timer 
    if (self.timer_count % 100 == 0):
      print "Step"
      #print(self.element)
      if self.round == 1:
        self.round = 0
        self.element.Disable()
      else:
        self.round = 1
        self.element.Enable()
      self.element.Update()
    
    #start = time.time()
    #self.organs
    #rate = 1/(time.time() - start)
    #print rate
    obj.GetRenderWindow().Render()
    self.timer_count += 1

tfn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/steel.jpg";

# Scene center
sp = vtk.vtkSphereSource()
sp.SetRadius(0.02)
sp.Update();

center = sp.GetOutput()

# Generate scenario element
sphere = vtk.vtkSphereSource()
sphere.SetPhiResolution(12)
sphere.SetThetaResolution(12)
sphere.Update()

input = sphere.GetOutput()

decimate = vtk.vtkDecimatePro()
decimate.SetInput(input)
decimate.SetTargetReduction(.50)
decimate.Update()

input_col = decimate.GetOutput()

vis = vtkesqui.vtkVisualizationModel();
vis.SetName("stick_vis");
vis.SetInput(input);
vis.SetTextureFileName(tfn);
vis.SetOpacity(1.0);
vis.SetColor(1.0, 1.0, 1.0);

col = vtkesqui.vtkCollisionModel();
col.SetName("vtkbioeng");
col.SetInput(input_col);
col.SetOpacity(0.5);
col.SetColor(0.0, 0.0, 1.0);
col.SetRadius(0.02)

stick = vtkesqui.vtkScenarioElement();
stick.SetId(0);
stick.SetName("stick");
stick.SetOrigin(0.0, 0.0, 0.0);
stick.SetPosition(3.0, 0.0, 0.0);
stick.SetOrientation(0, 0, -20);
stick.SetVisualizationModel(vis);
stick.SetCollisionModel(col);

stick.Update()

ren = vtk.vtkRenderer()
renWin = vtk.vtkRenderWindow()
renWin.AddRenderer(ren)
iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

ren.AddActor(vis.GetActor())
ren.AddActor(col.GetActor())

m = vtk.vtkPolyDataMapper()
m.SetInput(center)
a = vtk.vtkActor()
a.SetMapper(m)
a.GetProperty().SetColor(1.0, 0.5, 0.5)
ren.AddActor(a)

ren.SetBackground(1.0, 1.0, 1.0)
renWin.SetSize(500, 500)

ren.ResetCamera()
ren.GetActiveCamera().Azimuth(45)
iren.Initialize()

# Sign up to receive TimerEvent
cb = vtkTimerCallback()
iren.AddObserver('TimerEvent', cb.execute)
# 10ms timer
timerId = iren.CreateRepeatingTimer(10);
cb.element = stick

iren.Start()

