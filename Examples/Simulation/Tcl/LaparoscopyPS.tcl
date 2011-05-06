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

#ball identifiers
global ids; set ids {2 3 4}
#active ball identifier index. lindex $ids
global id; set id 0
#round management
global roundid; set roundid 0
global rounds; set ids {1 2 3}
global roundtime; set roundtime 60000
global timeout; set timeout 5000
#movement parameters
global alpha; set alpha 0
global beta; set beta 0.05

# idle loop
proc run {} {	
	global timeout
	scene
	movement
	after 100 collision
}

proc movement {} {
	global id ids alpha
	set j [expr $id + 1]
	set organ ball_${j}
	incr alpha 5
	set rad [expr $alpha * 0.01745311]
	set x [expr 0.1 * sin ($rad)]
	set y [expr 0.025 * cos ($rad)]
	$organ Translate $x $y 0.0
	after 15 movement
}

# Modify scenario
proc scene {} {
	global timeout id ids alpha
	set alpha [expr $alpha * -1]
	incr id
	set id [expr $id % 3]
	set k [expr [lindex $ids $id]]
	foreach j $ids {
		set o ball_${j}
		if {[expr [$o IsDisabled]] == 0} {
			if {[expr [lindex $ids $id]] == $j} {
				#Show ball
				#puts "Show $j"
				$o Show
			} else {
				#Hide ball
				$o Hide
				#puts "Hide $j"
			}
		}
	}
	if {[expr [ball_${k} IsDisabled]] == 1} {
		#If ball is disabled jump to next step immediately 
		after 10 scene
	} else { 
		after $timeout scene 
	}
}

proc collision {} {
	set left 0
	set right 0
	#Get simulation collisions
	set collisions [simulation GetCollisions]
	set n [$collisions GetNumberOfItems]
	#process collisions
	if {$n > 0} {
		for {set i 0} {$i < $n} {incr i} {
			set c [$collisions GetCollision $i]
			set tid [$c GetObjectId 0]
			set oid [$c GetObjectId 1]
			#puts "$tid $oid"
			set oname [[scenario GetObject $oid] GetName]
			if {$tid == 0} {
				#Left Probe
				set left 1
				if {$oid == 2} {
					#Both ball
					if {$right == 1} {
						$oname Disable
						puts "$oname Disabled"
					}
				} elseif {$oid == 3} {
					$oname Disable
					puts "$oname Disabled"
				}
			} elseif {$tid == 1} {
				#Right Probe
				set right 1
				if {$oid == 2} {
					#Both ball
					if {$left == 1} {
						$oname Disable
						puts "$oname Disabled"
					}
				} elseif {$oid == 4} {
					$oname Disable
					puts "$oname Disabled"
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
set fn0 "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/stick.vtp"
set fn0t "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/metal.jpg"
set fn1 "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/tip.vtp"
set fn0c "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/stick_col.vtp"
set fn1c "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/tip_col_lr.vtp"
set fn3 "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball.vtp"
set fn3c "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball_col_bb.vtp"
set fn3tb "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/bothball.jpg"
set fn3tl "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/leftball.jpg"
set fn3tr "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/rightball.jpg"
set fn4 "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/cavity.vtp"
set fn4c "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/cavity_col.vtp"
set fn4t "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/stomach.jpg"

###  Render Window Definitions  ###
vtkRenderer ren1
ren1 SetBackground 1.0 1.0 1.0

vtkRenderWindow renWin
renWin AddRenderer ren1
renWin SetSize 720 480

vtkRenderWindowInteractor iren
iren SetRenderWindow renWin

###  Scenario Definitions  ###
vtkScenario scenario
scenario SetRenderWindow renWin

### Tools ###
#Add new tool to the scenario 
#Left Probe
vtkVisualizationModel stick_l_vis 
stick_l_vis SetName "stick_l_vis"
stick_l_vis SetFileName $fn0
stick_l_vis SetTextureFileName $fn0t
stick_l_vis SetOrigin 0.0 0.0 6.0
stick_l_vis SetVisibility 1
stick_l_vis SetColor 1.0 1.0 1.0

vtkCollisionModel stick_l_col 
stick_l_col SetName "stick_l_col"
stick_l_col SetFileName $fn0c
stick_l_col SetOrigin 0 0 6
stick_l_col SetVisibility 0
stick_l_col SetColor 0.0 0.0 1.0

vtkScenarioElement stick_l 
stick_l SetName "stick_l"
stick_l SetVisualizationModel stick_l_vis
stick_l SetCollisionModel stick_l_col

#Second element (tip)
vtkVisualizationModel tip_l_vis 
tip_l_vis SetName "vis_tip_r"
tip_l_vis SetFileName $fn1
tip_l_vis SetTextureFileName $fn0t
tip_l_vis SetOrigin 0 0 6
tip_l_vis SetVisibility 1
tip_l_vis SetColor 0.0 1.0 0.0

vtkCollisionModel tip_l_col 
tip_l_col SetName "tip_l_col"
tip_l_col SetFileName $fn1c
tip_l_col SetOrigin 0 0 6
tip_l_col SetVisibility 0
tip_l_col SetColor 0.0 0.0 1.0

vtkScenarioElement tip_l 
tip_l SetName "tip_left"
tip_l SetVisualizationModel tip_l_vis
tip_l SetCollisionModel tip_l_col

vtkToolProbe probe_l 
probe_l SetName "probe_l"
probe_l SetStick stick_l
probe_l SetTip tip_l

#Right Probe
vtkVisualizationModel stick_r_vis 
stick_r_vis SetName "stick_r_vis"
stick_r_vis SetFileName $fn0
stick_r_vis SetTextureFileName $fn0t
stick_r_vis SetOrigin 0.0 0.0 6.0
stick_r_vis SetVisibility 1
stick_r_vis SetColor 1.0 1.0 1.0

vtkCollisionModel stick_r_col 
stick_r_col SetName "stick_r_col"
stick_r_col SetFileName $fn0c
stick_r_col SetOrigin 0 0 6
stick_r_col SetVisibility 0
stick_r_col SetColor 0.0 0.0 1.0

vtkScenarioElement stick_r 
stick_r SetName "stick_r"
stick_r SetVisualizationModel stick_r_vis
stick_r SetCollisionModel stick_r_col

#Second element (tip)
vtkVisualizationModel tip_r_vis 
tip_r_vis SetName "tip_r_vis"
tip_r_vis SetFileName $fn1
tip_r_vis SetTextureFileName $fn0t
tip_r_vis SetOrigin 0 0 6
tip_r_vis SetVisibility 1
tip_r_vis SetColor 1.0 0.0 0.0

vtkCollisionModel tip_r_col 
tip_r_col SetName "tip_r_col"
tip_r_col SetFileName $fn1c
tip_r_col SetOrigin 0 0 6
tip_r_col SetVisibility 0
tip_r_col SetColor 0.0 0.0 1.0

vtkScenarioElement tip_r 
tip_r SetName "tip_r"
tip_r SetVisualizationModel tip_r_vis
tip_r SetCollisionModel tip_r_col

vtkToolProbe probe_r 
probe_r SetName "probe_r"
probe_r SetStick stick_r
probe_r SetTip tip_r

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
col_ball_1 SetVisibility 0
col_ball_1 SetPosition 0 0 -5
col_ball_1 SetColor 0.0 0.0 1.0 

vtkScenarioElement el_ball_1  
el_ball_1 SetName "el_ball_1"
el_ball_1 SetVisualizationModel vis_ball_1 
el_ball_1 SetCollisionModel col_ball_1 

vtkOrgan ball_1
ball_1 SetName "ball_1"
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
col_ball_2 SetVisibility 0
col_ball_2 SetPosition -3 0 -5
col_ball_2 SetColor 0.0 0.0 1.0 

vtkScenarioElement el_ball_2  
el_ball_2 SetName "el_ball_2"
el_ball_2 SetVisualizationModel vis_ball_2 
el_ball_2 SetCollisionModel col_ball_2 

vtkOrgan ball_2
ball_2 SetName "ball_2"
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
col_ball_3 SetVisibility 0
col_ball_3 SetPosition 3 0 -5
col_ball_3 SetColor 0.0 0.0 1.0 

vtkScenarioElement el_ball_3  
el_ball_3 SetName "el_ball_3"
el_ball_3 SetVisualizationModel vis_ball_3 
el_ball_3 SetCollisionModel col_ball_3 

vtkOrgan ball_3
ball_3 SetName "ball_3"
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
scenario AddObject probe_l
scenario AddObject probe_r
scenario AddObject ball_1
scenario AddObject ball_2
scenario AddObject ball_3
scenario AddObject stomach
scenario Init

probe_l Translate -3 0 -2
#probe_l RotateX 10

probe_r Translate  3 0 -2
#probe_r RotateX -10

#ball_1 RotateY 180
#ball_1 RotateX -60

### Lights ###
[ren1 GetLights] InitTraversal
vtkLight headLight
headLight SetLightTypeToHeadlight
headLight PositionalOn
headLight SetIntensity 0.8
headLight SetConeAngle 50
ren1 AddLight headLight
	
vtkLight ambientLight 
ambientLight SetIntensity 0.2
#ambientLight SetLightTypeToHeadlight
ambientLight PositionalOff
ren1 AddLight ambientLight
#ren1 SetAmbient 0.5 0.5 0.5
	
### Camera Definitions ###
set camera [ren1 GetActiveCamera]
$camera SetPosition 0 0 2
$camera SetFocalPoint 0 0 -6
$camera Yaw 0
$camera Elevation 10
$camera Pitch -10
$camera Dolly 1
$camera ParallelProjectionOff
$camera SetViewAngle 60

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

