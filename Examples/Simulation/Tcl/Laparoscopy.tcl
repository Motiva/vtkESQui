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

set filename0 "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Pincers/Stick.vtp"
set filename1 "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Pincers/LeftGrasper.vtp"
set filename2 "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Pincers/RightGrasper.vtp"
set filename3 "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball.vtp"
set filename3t "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/muscletexture.jpg"
set filename4 "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/stomach.vtp"
set filename4t "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/stomachtexture.jpg"

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

###  Load Deformable Model  ###
#Create a Organ
vtkOrgan organ
#Set organ identifier
organ SetId 0
organ SetName "Sphere"

#Set source data filename
organ SetFileName $filename3
organ SetTextureFileName $filename3t

#Set geometric parameters
organ SetPosition 0.0 0.0 -3.0
organ SetOrientation 0.0 0.0 0.0
organ SetOrigin 0.0 0.0 -3.0

#Set tool scale  size
organ SetScale 1.0 1.0 1.0

#Set organ type (Deformable)
#organ SetOrganType 1

#Set Deformation Model
vtkPSSInterface particleSpring
particleSpring SetDeltaT 0.01
particleSpring SetGravity 0.0 0.0 0.0

#Set particle-spring system specific parameters
particleSpring SetSpringCoefficient 100
particleSpring SetDampingCoefficient 5
particleSpring SetDistanceCoefficient 20
particleSpring SetMass 1
particleSpring SetRigidityFactor 2
#particleSpring SetSolverType vtkParticleSpringSystem::RungeKutta4

organ SetDeformationModel particleSpring

#Add organ to the scenario
scenario AddOrgan organ

#Create a cavity organ
vtkOrgan cavity
#Set organ identifier
cavity SetId 1
cavity SetName "Cavity"

#Set source data filename
cavity SetFileName $filename4
cavity SetTextureFileName $filename4t

#Set geometric parameters
cavity SetPosition 0.0 0.0 0.0
cavity SetOrientation 0.0 0.0 0.0
cavity SetOrigin 0.0 0.0 0.0

#Set tool scale  size
cavity SetScale 1.0 1.0 1.0

#Set organ type
#cavity SetOrganType vtkOrgan::Deformable

#Set Deformation Model
vtkPSSInterface ps1
ps1 SetDeltaT 0.01
ps1 SetGravity 0.0 0.0 0.0

#Set particle-spring system specific parameters
ps1 SetSpringCoefficient 100
ps1 SetDampingCoefficient 5
ps1 SetDistanceCoefficient 20
ps1 SetMass 1
ps1 SetRigidityFactor 2
#ps1 SetSolverType vtkParticleSpringSystem::RungeKutta4

cavity SetDeformationModel ps1

#Add organ to the scenario
scenario AddOrgan cavity

### Tools ###
#Add new tool To the scenario
#Create a Tool
vtkToolPincers leftPincers
#Set tool identifier
leftPincers SetId 0
leftPincers SetNumberOfPieces 3
#Set source data filename
leftPincers SetStickFileName $filename0
leftPincers SetLeftGrasperFileName $filename1
leftPincers SetRightGrasperFileName $filename2
#Set geometric parameters
leftPincers SetPosition -3 0 0
leftPincers SetOrientation 0 10 0
leftPincers SetOrigin 0 0 4

#Set tool scale  size
leftPincers SetScale 1.0 1.0 1.0
leftPincers SetDeltaT 0.01

#Add tool to the scenario
scenario AddTool leftPincers

#Create a Tool
vtkToolPincers rightPincers
#Set tool identifier
rightPincers SetId 1
rightPincers SetNumberOfPieces 3
#Set source data filename
rightPincers SetStickFileName $filename0
rightPincers SetLeftGrasperFileName $filename1
rightPincers SetRightGrasperFileName $filename2
#Set geometric parameters
rightPincers SetPosition 3 0 0
rightPincers SetOrientation 0 -10 0
rightPincers SetOrigin 0 0 4

#Set tool scale  size
rightPincers SetScale 1.0 1.0 1.0
rightPincers SetDeltaT 0.01

#Add tool to the scenario
scenario AddTool rightPincers

###  Load Scene Environment  ###

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
vtkSimulationInteractorStyle style
style SetScenario scenario
iren SetInteractorStyle style

vtkSimulation simulation
simulation SetScenario scenario
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

