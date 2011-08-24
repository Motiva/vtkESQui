puts "----------------------------------------------"
puts "|    ESQuiLap Precission & Speed Exercise    |"
puts "----------------------------------------------\n"

puts "Loading VTK in progress ...\n"
package require vtk
puts "VTK loaded.\n"
puts "Loading vtkInteraction in progress ...\n"
package require vtkinteraction
puts "vtkInteraction loaded.\n"
puts "Loading vtkESQui in progress ...\n"
package require vtkesqui
puts "vtkESQui loaded.\n"


# idle loop
proc sleep {} {
	after 10 scene
}

# Modify scenario
proc scene {} {
	sleep
}

#catch {package require vtkesquiHaptics}
#catch {::vtk::load_component vtkesquiHapticsTCL}

# Paths
set fn "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball.vtp"
set cfn "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball_col.vtp"
set dfn "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball_def_c10.vtp"
set tfn "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/muscle.jpg"

vtkVisualizationModel vis
vis SetName "ellipsoid_16_16_1"
vis SetFileName $fn
vis SetTextureFileName $tfn
vis SetOpacity 0.8
vis SetColor 1.0 1.0 1.0

vtkCollisionModel col 
col SetName "vtkbioeng"
col SetFileName $cfn
col SetOpacity 0.5
col SetColor 0.0 0.0 1.0

#Deformation model. Particle-Spring system
vtkPSSInterface def 
def SetName "ParticleSpring"
def SetFileName $dfn
def SetOpacity 1
def SetColor 0.0 1.0 0.0

vtkScenarioElement element 
element SetId 0
element SetName "ellipsoid"
element SetPosition 3.0 2.5 0.0
element SetOrientation 25 -15 30
element SetVisualizationModel vis
element SetCollisionModel col
element SetDeformationModel def
element Init

element RotateZ 35
element Update 

vtkOrgan organ 
organ AddElement element
organ Init 

# Create renderer window
vtkRenderer ren1
vtkRenderWindow renWin
renWin AddRenderer ren1
renWin SetSize 800 600

# Default window interactor
vtkRenderWindowInteractor iren
iren SetRenderWindow renWin
iren SetInteractorStyle ""

ren1 AddActor [vis GetActor]
ren1 AddActor [col GetActor]
ren1 AddActor [def GetActor]

# render the image
#
iren AddObserver UserEvent {wm deiconify .vtkInteract}
iren Initialize

# prevent the tk window from showing up then start the event loop
wm withdraw .

