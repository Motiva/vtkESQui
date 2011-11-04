
import sys

import vtk
import vtkesqui

filename ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Meshes/ellipsoid16_16_1.vtp";

reader = vtk.vtkXMLPolyDataReader()
reader.SetFileName(filename)

model = vtkesqui.vtkModel()
model.SetInput(reader.GetOutput())
model.SetColor(0.0 ,1.0 , 0.0)
model.Init()

#A call to update method is made to force the model to be at its last state
model.Update();

ren = vtk.vtkRenderer()
renWin = vtk.vtkRenderWindow()
renWin.AddRenderer(ren)
iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

actor = model.GetActor()

ren.AddActor(actor)
ren.SetBackground(1.0, 1.0, 1.0)
renWin.SetSize(500, 500)

ren.ResetCamera()
iren.Initialize()

renWin.Render()

iren.Start()