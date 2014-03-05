data_path = '<vtkESQuiData>'

import vtk
from vtkesqui import *
import sys

def TimerCallBack(obj, event):
    bioeng.Update()
    cols = bioeng.GetNumberOfCollisions()
    sys.stdout.write('\rColisiones: %d' % cols)
    sys.stdout.flush()

def Keypress(obj, event):
	key = obj.GetKeySym()
	if key == "q":
		sys.stdout.write('\r--- Fin ---      \n')
		quit()
	elif key == "Up":
		ele0.Translate(-0.1,0,0)
		ele1.Translate(0.1,0,0)
	elif key == "Down":
		ele0.Translate(0.1,0,0)
		ele1.Translate(-0.1,0,0)
	scenario.Update()
	scenario.Render()

# Modelos de visualizacion
vis0 = vtkVisualizationModel()
vis0.SetFileName(data_path+'Scenario/Organs/ball.vtp')
vis0.SetTextureFileName(data_path+'Scenario/Textures/leftball.jpg')

vis1 = vtkVisualizationModel()
vis1.SetFileName(data_path+'Scenario/Organs/ball.vtp')
vis0.SetTextureFileName(data_path+'Scenario/Textures/rightball.jpg')

# Modelos de colision
col0 = vtkCollisionModel()
col0.SetFileName(data_path+'Scenario/Organs/ball_col.vtp')

col1 = vtkCollisionModel()
col1.SetFileName(data_path+'Scenario/Organs/ball_col.vtp')

# Elementos
ele0 = vtkScenarioElement()
ele0.SetVisualizationModel(vis0)
ele0.SetCollisionModel(col0)
ele0.SetPosition(-2,0,0)
ele0.Update()

ele1 = vtkScenarioElement()
ele1.SetVisualizationModel(vis1)
ele1.SetCollisionModel(col1)
ele1.SetPosition(2,0,0)
ele1.Update()

# Organos
org0 = vtkOrgan()
org0.AddElement(ele0)

org1 = vtkOrgan()
org1.AddElement(ele1)

# Deteccion de colisiones
bioeng = vtkBioEngInterface()
bioeng.AddModel(col0)
bioeng.AddModel(col1)
bioeng.Initialize()

# Escenario
scenario = vtkScenario()
scenario.AddObject(org0)
scenario.AddObject(org1)
scenario.Update()
scenario.Render()

# Interaccion con teclado
iren = scenario.GetRenderWindowInteractor()
iren.SetInteractorStyle(None)
iren.CreateRepeatingTimer(250)
iren.AddObserver("KeyPressEvent", Keypress)
iren.AddObserver("TimerEvent", TimerCallBack)
iren.Start()
