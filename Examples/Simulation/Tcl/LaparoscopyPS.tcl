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

global id; set id 0
global ids; set ids {1 2 3}
global roundid; set roundid 0
global rounds; set ids {1 2 3}
global roundtime; set roundtime 60000
global alpha; set alpha 0
global timeout; set timeout 5000

# idle loop
proc run {} {	
	global timeout
	scene
	movement
	after 100 collision
}

proc movement {} {
	global id ids alpha
	set organ [scenario GetOrgan [lindex $ids $id]]
	incr alpha 5
	set rad [expr $alpha * 0.01745311]
	set x [expr 0.1 * sin ($rad)]
	set y [expr 0.1 * cos ($rad)]
	$organ Translate 0.0 $x $y
	after 20 movement
}

# Modify scenario
proc scene {} {
	global timeout id ids
	incr id
	set id [expr $id % 3]
	foreach j $ids {
		set o [scenario GetOrgan $j]
		if {[expr [$o IsDisabled]] == 0} {
			if {[expr [lindex $ids $id]] == $j} {
				#Show ball
				$o Show
			} else {
				#Hide ball
				$o Hide
			}
		}
	}
	if {[expr [ [scenario GetOrgan [lindex $ids $id]] IsDisabled ] ] == 1} {
		#If ball is disabled jump to next step immediately 
		after 10 scene
	} else { 
		after $timeout scene 
	}
}

proc collision {} {
	set contacts [scenario GetContacts]
	set n [$contacts GetNumberOfItems]
	set left 0
	set right 0
	if {$n > 0} {
		for {set i 0} {$i < $n} {incr i} {
			set c [$contacts GetContact $i]
			set toolId [$c GetItemId 0]
			set organId [$c GetItemId 1]
			if {$organId > 0 } {
				set o [scenario GetOrgan $organId]
				set oname [$o GetName]
				if { [string compare $oname "LeftBall"] == 0} {
					#LeftBall -> Left Tool
					if {$toolId == 0} { $o Disable }
				} elseif { [string compare $oname "RightBall"] == 0} {
					#RightBall -> Right Tool
					if {$toolId == 1} { $o Disable }
				} else {
					#BothBall -> Right & Left Tool
					if { $toolId == 0 } then { set left 1 }
					if { $toolId == 1 } then { set right 1 }
					#Both tools contacted
					if {$left == 1 && $right == 1 
					} then { $o Disable }
				}
			}
		}
	}
	set left 0
	set right 0
	after 20 collision
}

#catch {package require vtkesquiHaptics}
#catch {::vtk::load_component vtkesquiHapticsTCL}

set fn0 "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/Stick.vtp"
set fn0t "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/metal.jpg"
set fn1 "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/Tip.vtp"
set fn1t "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/aluminium.jpg"
#set fn2 "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/RightLever.vtp"
set fn3 "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball.vtp"
set fn3t0 "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/leftball.jpg"
set fn3t1 "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/rightball.jpg"
set fn3t2 "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/bothball.jpg"
set fn4 "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/stomach.vtp"
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

###  Load Deformable Model  ###
#Create a cavity organ
vtkOrgan cavity
#Set organ identifier
cavity SetId 0
cavity SetName "Cavity"

#Set source data filename
cavity SetFileName $fn4
cavity SetTextureFileName $fn4t

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

#Create a Organ
vtkOrgan ball
#Set organ identifier
ball SetId 1
ball SetName "LeftBall"

#Set source data filename
ball SetFileName $fn3
ball SetTextureFileName $fn3t0

#Set geometric parameters
ball SetPosition 0.0 0.0 -4.0
ball SetOrientation 0.0 -90.0 0.0
ball SetOrigin 0.0 0.0 -4.0

#Set tool scale  size
ball SetScale 0.5 0.5 0.5

#Add organ to the scenario
scenario AddOrgan ball

#Create a Organ
vtkOrgan ball1
#Set organ identifier
ball1 SetId 2
ball1 SetName "RightBall"

#Set source data filename
ball1 SetFileName $fn3
ball1 SetTextureFileName $fn3t1

#Set geometric parameters
ball1 SetPosition -3.0 0.0 -3.0
ball1 SetOrientation 0.0 -90.0 0.0
ball1 SetOrigin -3.0 0.0 -3.0

#Set tool scale  size
ball1 SetScale 0.5 0.5 0.5

#Add organ to the scenario
scenario AddOrgan ball1

#Create a Organ
vtkOrgan ball2
#Set organ identifier
ball2 SetId 3
ball2 SetName "BothBall"

#Set source data filename
ball2 SetFileName $fn3
ball2 SetTextureFileName $fn3t2

#Set geometric parameters
ball2 SetPosition 3.0 0.0 -3.0
ball2 SetOrientation 0.0 -90.0 0.0
ball2 SetOrigin 3.0 0.0 -3.0

#Set tool scale  size
ball2 SetScale 0.5 0.5 0.5

#Add organ to the scenario
scenario AddOrgan ball2

### Tools ###
#Add new tool To the scenario
#Create a Tool
vtkToolProbe leftProbe
#Set tool identifier
leftProbe SetId 0
#Set source data filename
leftProbe SetStickFileName $fn0
leftProbe SetTipFileName $fn1
#Set texture filename
leftProbe SetStickTextureFileName $fn0t
leftProbe SetTipTextureFileName $fn1t
#Set tip color
leftProbe SetColor 1 0.0 1.0 0.0
#Set geometric parameters
leftProbe SetPosition -3 0 0
leftProbe SetOrientation 0 10 0
leftProbe SetOrigin 0 0 4

#Set tool scale  size
leftProbe SetScale 1.0 1.0 1.0
leftProbe SetDeltaT 0.01

#Add tool to the scenario
scenario AddTool leftProbe

#Create a Tool
vtkToolProbe rightProbe
#Set tool identifier
rightProbe SetId 1
#Set source data filename
rightProbe SetStickFileName $fn0
rightProbe SetTipFileName $fn1
#Set texture filename
rightProbe SetStickTextureFileName $fn0t
rightProbe SetTipTextureFileName $fn1t
#Set tip color
rightProbe SetColor 1 1.0 0.0 0.0
#Set geometric parameters
rightProbe SetPosition 3 0 0
rightProbe SetOrientation 0 -10 0
rightProbe SetOrigin 0 0 4

#Set tool scale  size
rightProbe SetScale 1.0 1.0 1.0
rightProbe SetDeltaT 0.01

#Add tool to the scenario
scenario AddTool rightProbe


###  Load Scene Environment  ###

### Lights ###
[ren1 GetLights] InitTraversal
vtkLight headLight
headLight SetLightTypeToHeadlight
headLight PositionalOn
headLight SetIntensity 0.5
headLight SetConeAngle 40
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

run

simulation Run

