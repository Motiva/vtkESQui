#!/usr/bin/env python

import sys
import vtk
import vtkesqui

fn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/stick.vtp";
cfn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/stick_col.vtp";
tfn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/steel.jpg";

fnb ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/tip.vtp";
cfnb ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/tip_col.vtp";
tfnb ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/metal.jpg";


# Generate tool's first element (stick)
vis = vtkesqui.vtkVisualizationModel();
vis.SetName("stick_vis");
vis.SetInput(istick);
vis.SetTextureFileName(tfn);
vis.SetOpacity(1.0);
vis.SetColor(1.0, 1.0, 1.0);

col = vtkesqui.vtkCollisionModel();
col.SetName("vtkbioeng");
col.SetFileName(cfn);
col.SetOpacity(0.5);
col.SetColor(0.0, 0.0, 1.0);

stick = vtkesqui.vtkScenarioElement();
stick.SetId(0);
stick.SetName("stick");
stick.SetOrigin(0.0, 0.0, 0.0);
stick.SetPosition(3.0, 0.0, 0.0);
stick.SetOrientation(0, 0, -20);
stick.SetVisualizationModel(vis);
stick.SetCollisionModel(col);

# Second element (tip)
visb = vtkesqui.vtkVisualizationModel();
visb.SetName("tip_vis");
visb.SetFileName(fnb);
visb.SetTextureFileName(tfnb);
visb.SetOpacity(1.0);
visb.SetColor(1.0, 0.0, 1.0);

colb = vtkesqui.vtkCollisionModel();
colb.SetName("tip_vtkbioeng");
colb.SetFileName(cfnb);
colb.SetOpacity(0.5);
colb.SetColor(0.0, 0.0, 1.0);

tip = vtkesqui.vtkScenarioElement();
tip.SetId(1);
tip.SetName("tip");
tip.SetPosition(3.0, 0.0, 0.0);
tip.SetOrientation(0, 0, -20);
tip.SetVisualizationModel(visb);
tip.SetCollisionModel(colb);

tool = vtkesqui.vtkToolProbe();
tool.SetStick(stick);
tool.SetTip(tip);

tool.SetDepth(1);
tool.Update();

renderer = vtk.vtkRenderer();

renWin = vtk.vtkRenderWindow();
renWin.SetSize(800,600);
renWin.AddRenderer(renderer);

iren = vtk.vtkRenderWindowInteractor();
iren.SetRenderWindow(renWin);

axes = vtk.vtkAxesActor();

renderer.AddActor(axes);
renderer.AddActor(vis.GetActor());
renderer.AddActor(col.GetActor());
renderer.AddActor(visb.GetActor());
renderer.AddActor(colb.GetActor());
renderer.SetBackground(.8,.8,.8);

# Adjust Camera
camera = renderer.GetActiveCamera();
camera.SetPosition(3.0, 0, 6);
camera.SetFocalPoint(0, 0, -6);
camera.SetViewAngle(60);
camera.Yaw(0);
camera.Elevation(20);
camera.Pitch(-15);
camera.Dolly(1);

iren.Initialize();

renWin.Render();

iren.Start();

