import vtk
import vtkesqui

class TimerCallback1():
	def __init__(self):
		#vtk.vtkCommand.__init__(self)
		print "hi"

	def Execute(caller, eventId, callData):
		if vtk.vtkCommand.TimerEvent != eventId:
			return

		tid = callData

		if tid != self.TimerId:
			return

		iren = caller

		p0 = self.Model0.GetActor().GetPosition()
		p1 = self.Model1.GetActor().GetPosition()
		
		self.Filter.Update()

		n = self.Filter.GetNumberOfCollisionPairs()
		for i in range(0,n):
			self.Filter.GetCollisionPair(i,p)
			print "Model\t: %i\t:%i" % p
			print "Colls\t: %i\t:%i" % [self.Model0.GetNumberOfCollisions(), self.Model1.GetNumberOfCollisions()]
			print "--------------------------"

		iren.Render()

	def SetTimerId(i):
		self.TimerId = i

	def SetModel(i, model):
		if i == 0:
			self.Model0 = model
		else:
			self.Model1 = model

	def SetFilter(f):
		self.Filter = f

def TestvtkBioEngInterface():
	src0 = vtk.vtkSphereSource()
	src0.SetRadius(1)
	src0.SetThetaResolution(12)
	src0.SetPhiResolution(12)
	src0.Update()

	s0 = src0.GetOutput()

	src1 = vtk.vtkSphereSource()
	src1.SetRadius(0.5)
	src1.SetThetaResolution(12)
	src1.SetPhiResolution(12)
	src1.Update()

	s1 = src1.GetOutput()

	col0 = vtkesqui.vtkCollisionModel()
	col0.SetName("col0")
	col0.SetInput(s0)
	col0.SetObjectId(0)
	col0.SetId(0)
	col0.Update()
	col0.GetActor().SetPosition(0,0,0)

	col1 = vtkesqui.vtkCollisionModel()
	col1.SetName("col1")
	col1.SetInput(s1)
	col1.SetObjectId(0)
	col1.SetId(0)
	col1.Update()
	col1.GetActor().SetPosition(3,0,0)

	renderer = vtk.vtkRenderer()

	renWin = vtk.vtkRenderWindow()
	renWin.SetSize(500,500)
	renWin.AddRenderer(renderer)

	iren = vtk.vtkRenderWindowInteractor()
	iren.SetRenderWindow(renWin)

	renderer.AddActor(col0.GetActor())
	renderer.AddActor(col1.GetActor())

	col = vtkesqui.vtkBioEngInterface()
	col.SetBoxTolerance(0)
	col.SetCellTolerance(0)
	col.SetNumberOfCellsPerNode(2)
	col.SetModeToFull()
	col.Initialize()

	col.AddModel(col0)
	col.AddModel(col1)
	col.Update()

	renderer.SetBackground(0.3, 0.6, 0.3)
	
	renderer.ResetCamera()
	iren.Initialize()
	iren.CreateRepeatingTimer(500)

	print "here"
	cb = TimerCallback1() # Segmention fault here
	iren.AddObserver("TimerEvent", cb)

	cb.SetModel(0, col0)
	cb.SetModel(1, col1)
	cb.SetFilter(col)

	tid = iren.CreateRepeatingTimer(500)
	cb.SetTimerId(tid)

	renWin.Render()
	
	iren.Start()

if __name__ == "__main__":
	TestvtkBioEngInterface()

