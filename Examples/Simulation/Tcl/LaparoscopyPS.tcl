puts "----------------------------------------------"
puts "|    vtkESQui Precission & Speed Exercise    |"
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
	#after 100 collision
}

proc movement {} {
	global id ids alpha
	set j [expr $id + 1]
	set organ ball_${j}
	incr alpha 5
	set rad [expr $alpha * 0.01745311]
	set x [expr 0.1 * sin ($rad)]
	set y [expr 0.1 * cos ($rad)]
	$organ Translate $x $y 0.0
	after 20 movement
}

# Modify scenario
proc scene {} {
	global timeout id ids ball_1
	incr id
	set id [expr $id % 3]
	foreach j $ids {
		set o ball_${j}
		if {[expr [$o IsDisabled]] == 0} {
			if {[expr [lindex $ids $id]] == $j} {
				#Show ball
				puts "Show $j"
				$o Show
			} else {
				#Hide ball
				$o Hide
				puts "Hide $j"
			}
		}
	}
	if {[expr [ [scenario GetObject [lindex $ids $id]] IsDisabled ] ] == 1} {
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
set fn0c "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/Stick_col.vtp"
set fn1c "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/Tip_col_lr.vtp"
set fn3 "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball.vtp"
set fn3c "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball_col_bb.vtp"
set fn3tb "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/bothball.jpg"
set fn3tl "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/leftball.jpg"
set fn3tr "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/rightball.jpg"
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
#Left Probe
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

#Second element (tip)
vtkVisualizationModel vis_tip_l 
vis_tip_l SetName "vis_tip_r"
vis_tip_l SetFileName $fn1
vis_tip_l SetTextureFileName $fn0t
vis_tip_l SetOrigin 0 0 6
vis_tip_l SetVisibility 1
vis_tip_l SetColor 1.0 0.0 0.0

vtkCollisionModel col_tip_l 
col_tip_l SetName "col_tip_l"
col_tip_l SetFileName $fn1c
col_tip_l SetOrigin 0 0 6
col_tip_l SetVisibility 0
col_tip_l SetColor 0.0 0.0 1.0

vtkScenarioElement left_tip 
left_tip SetName "tip_left"
left_tip SetVisualizationModel vis_tip_l
left_tip SetCollisionModel col_tip_l

vtkToolProbe leftProbe 
leftProbe SetStick stick
leftProbe SetTip left_tip

###  Load Organs (Static balls)  ###
vtkVisualizationModel vis_ball_1
vis_ball_1 SetName "sphere_vis_1"
vis_ball_1 SetFileName $fn3 
vis_ball_1 SetTextureFileName $fn3tb 
vis_ball_1 SetVisibility 1 
vis_ball_1 SetPosition 0 0 -5
vis_ball_1 SetColor 1.0 1.0 1.0 

vtkCollisionModel col_ball_1
col_ball_1 SetName "sphere_col_1"
col_ball_1 SetFileName $fn3c
col_ball_1 SetVisibility 1
col_ball_1 SetPosition 0 0 -5
col_ball_1 SetColor 0.0 0.0 1.0 

vtkScenarioElement el_ball_1  
el_ball_1 SetName "ball_1"
el_ball_1 SetVisualizationModel vis_ball_1 
el_ball_1 SetCollisionModel col_ball_1 

vtkOrgan ball_1  
ball_1 AddElement el_ball_1 

vtkVisualizationModel vis_ball_2
vis_ball_2 SetName "sphere_vis_2"
vis_ball_2 SetFileName $fn3 
vis_ball_2 SetTextureFileName $fn3tl
vis_ball_2 SetVisibility 1 
vis_ball_2 SetPosition -3 0 -5
vis_ball_2 SetColor 1.0 1.0 1.0 

vtkCollisionModel col_ball_2
col_ball_2 SetName "sphere_col_2"
col_ball_2 SetFileName $fn3c
col_ball_2 SetVisibility 1
col_ball_2 SetPosition -3 0 -5
col_ball_2 SetColor 0.0 0.0 1.0 

vtkScenarioElement el_ball_2  
el_ball_2 SetName "ball_2"
el_ball_2 SetVisualizationModel vis_ball_2 
el_ball_2 SetCollisionModel col_ball_2 

vtkOrgan ball_2  
ball_2 AddElement el_ball_2 

vtkVisualizationModel vis_ball_3
vis_ball_3 SetName "sphere_vis_3"
vis_ball_3 SetFileName $fn3 
vis_ball_3 SetTextureFileName $fn3tr
vis_ball_3 SetVisibility 1 
vis_ball_3 SetPosition 3 0 -5
vis_ball_3 SetColor 1.0 1.0 1.0 

vtkCollisionModel col_ball_3
col_ball_3 SetName "sphere_col_3"
col_ball_3 SetFileName $fn3c
col_ball_3 SetVisibility 1
col_ball_3 SetPosition 3 0 -5
col_ball_3 SetColor 0.0 0.0 1.0 

vtkScenarioElement el_ball_3  
el_ball_3 SetName "ball_3"
el_ball_3 SetVisualizationModel vis_ball_3 
el_ball_3 SetCollisionModel col_ball_3 

vtkOrgan ball_3  
ball_3 AddElement el_ball_3 

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
scenario AddObject leftProbe
scenario AddObject ball_1
scenario AddObject ball_2
scenario AddObject ball_3
scenario AddObject stomach
scenario Init

leftProbe Translate -2 0 -2
leftProbe RotateX 10

#ball_1 RotateY 180
#ball_1 RotateX -60

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
style Init
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

