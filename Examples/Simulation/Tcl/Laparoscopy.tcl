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
set fn0 "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/stick.vtp"
set fn0t "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/metal.jpg"
set fn1 "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/lever_l.vtp"
set fn2 "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/lever_r.vtp"
set fn0c "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/stick_col.vtp"
set fn1c "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/lever_l_col_lr.vtp"
set fn2c "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/lever_r_col_lr.vtp"
set fn3 "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball.vtp"
set fn3c "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball_col.vtp"
set fn3t "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/muscle.jpg"
set fn4 "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/stomach.vtp"
set fn4c "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/stomach_col.vtp"
set fn4t "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/stomach.jpg"

###  Render Window Definitions  ###
vtkRenderer ren1
ren1 SetBackground 1.0 1.0 1.0

vtkRenderWindow renWin
renWin AddRenderer ren1
renWin SetSize 840 480

vtkRenderWindowInteractor iren
iren SetRenderWindow renWin

###  Scenario Definitions  ###
vtkScenario scenario
scenario SetRenderWindow renWin

### Tools ###
#Add new tool to the scenario 
#Left grasper
vtkVisualizationModel vis_stick 
vis_stick SetName "vis_stick"
vis_stick SetFileName $fn0
vis_stick SetTextureFileName $fn0t
vis_stick SetOrigin 0.0 0.0 6.0
vis_stick SetVisibility 1
vis_stick SetColor 1.0 1.0 1.0

vtkCollisionModel col_stick 
col_stick SetName "col_stick"
col_stick SetFileName $fn0c
col_stick SetOrigin 0 0 6
col_stick SetVisibility 0
col_stick SetColor 0.0 0.0 1.0

vtkScenarioElement stick 
stick SetName "stick"
stick SetVisualizationModel vis_stick
stick SetCollisionModel col_stick

#Second element (left lever)
vtkVisualizationModel vis_lever_l 
vis_lever_l SetName "vis_lever_r"
vis_lever_l SetFileName $fn1
vis_lever_l SetTextureFileName $fn0t
vis_lever_l SetOrigin 0 0 6
vis_lever_l SetVisibility 1
vis_lever_l SetColor 0.0 1.0 1.0

vtkCollisionModel col_lever_l 
col_lever_l SetName "col_lever_l"
col_lever_l SetFileName $fn1c
col_lever_l SetOrigin 0 0 6
col_lever_l SetVisibility 0
col_lever_l SetColor 0.0 0.0 1.0

vtkScenarioElement left_lever 
left_lever SetName "lever_left"
left_lever SetVisualizationModel vis_lever_l
left_lever SetCollisionModel col_lever_l

#Third element (right lever)
vtkVisualizationModel vis_lever_r 
vis_lever_r SetName "vis_lever_r"
vis_lever_r SetFileName $fn2
vis_lever_r SetOrigin 0 0 6
vis_lever_r SetTextureFileName $fn0t
vis_lever_r SetVisibility 1
vis_lever_r SetColor 0.0 1.0 1.0

vtkCollisionModel col_lever_r 
col_lever_r SetName "col_lever_r"
col_lever_r SetFileName $fn2c
col_lever_r SetOrigin 0 0 6
col_lever_r SetVisibility 0
col_lever_r SetColor 0.0 0.0 1.0

vtkScenarioElement right_lever 
right_lever SetName "lever_right"
right_lever SetVisualizationModel vis_lever_r
right_lever SetCollisionModel col_lever_r

vtkToolGrasper leftGrasper 
leftGrasper SetStick stick
leftGrasper SetLeftLever left_lever
leftGrasper SetRightLever right_lever


###  Load Organs  ###
vtkVisualizationModel vis_organ
vis_organ SetName "sphere_vis"
vis_organ SetFileName $fn3 
vis_organ SetTextureFileName $fn3t 
vis_organ SetVisibility 1 
vis_organ SetColor 1.0 1.0 1.0 

vtkCollisionModel col_organ
col_organ SetName "sphere_col"
col_organ SetFileName $fn3c
col_organ SetVisibility 1
col_organ SetColor 0.0 0.0 1.0 

#Deformation model. Particle-Spring system
vtkPSSInterface def_organ
def_organ SetName "sphere_def" 
def_organ SetFileName $fn3 
def_organ SetVisibility 0 
def_organ SetColor 0.0 1.0 0.0 
def_organ SetDeltaT 0.01 
def_organ SetGravity 0.0 0.0 0.0 

def_organ SetSpringCoefficient 100 
def_organ SetDampingCoefficient 5 
def_organ SetDistanceCoefficient 20 
def_organ SetMass 1 
def_organ SetRigidityFactor 2 

vtkScenarioElement el_organ  
el_organ SetName "Sphere"
el_organ SetVisualizationModel vis_organ 
el_organ SetCollisionModel col_organ 
el_organ SetDeformationModel def_organ 

vtkOrgan organ  
organ AddElement el_organ 

#Stomach
vtkVisualizationModel vis_stomach
vis_stomach SetName "stomach_vis"
vis_stomach SetFileName $fn4
vis_stomach SetTextureFileName $fn4t
vis_stomach SetVisibility 1
vis_stomach SetColor 1.0 1.0 1.0

vtkScenarioElement el_stomach
el_stomach SetName "Stomach"
el_stomach SetVisualizationModel vis_stomach
#el_stomach SetCollisionModel col_stomach
#el_stomach SetDeformationModel def_stomach

vtkOrgan stomach
stomach AddElement el_stomach

#/**********  Initialize Scenario  ********/
scenario AddObject leftGrasper
scenario AddObject organ
scenario AddObject stomach
scenario Init

leftGrasper Translate -2 0 -2
leftGrasper RotateX 10

organ Translate 0 0 -4

### Lights ###
[ren1 GetLights] InitTraversal
vtkLight headLight
headLight SetLightTypeToHeadlight
headLight PositionalOn
headLight SetIntensity 0.5
headLight SetConeAngle 20
ren1 AddLight headLight
	
vtkLight ambientLight 
ambientLight SetIntensity 0.8
ambientLight SetLightTypeToHeadlight
ambientLight PositionalOff
ren1 AddLight ambientLight
ren1 SetAmbient 0.5 0.5 0.5
	
### Camera Definitions ###
set camera [ren1 GetActiveCamera]
$camera SetPosition 0 0 2
$camera SetFocalPoint 0 0 -6
$camera Yaw 0
$camera Elevation 20
$camera Pitch -15
$camera Dolly 1
$camera ParallelProjectionOff
$camera SetViewAngle 70

### Simulation Setup ###
### Simulation Setup ###
vtkSimulationInteractorStyle style

vtkSimulation simulation
simulation SetScenario scenario
simulation SetInteractorStyle style
simulation SetRenderTimerRate 0.02
simulation SetSimulationTimerRate 0.01
simulation SetHapticTimerRate 0.001
simulation Init

# render the image
#
iren AddObserver UserEvent {wm deiconify .vtkInteract}
iren Initialize

# prevent the tk window from showing up then start the event loop
wm withdraw .

#sleep

simulation Run

