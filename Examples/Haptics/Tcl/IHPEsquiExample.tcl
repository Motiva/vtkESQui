
proc UpdateHaptic {} {
	#Update state of the IHP device
	Haptic UpdateDevice

	for {set i 0} {$i<2} {incr i} {

		#Trocar state
		#set state [Haptic GetTrocarState $i]

		#Trocar's direction angles
		set yaw [Haptic GetTrocarYaw $i]
		set pitch [Haptic GetTrocarPitch $i]

		#Tool-in-the-trocar parameters
		set depth [Haptic GetToolDepth $i]
		set roll [Haptic GetToolRoll $i]
		set opening [Haptic GetToolOpening $i]

		set tool Tool$i

		#puts "Yaw: $yaw, Pitch: $pitch, Roll: $roll"
		
		# Haptic coordinate system differs from VTK system
		# | Haptic  |   VTK  	|
		# |    X    |    Y    |
		# |    Y    |    Z    |
		# |    Z    |    X    |
		$tool UpdateOrientation $pitch $yaw 0

		#Set tool's roll angle
		$tool RotateZ $roll

		#Set tool's depth
		set tooldepth [expr 20 * $depth]
		$tool SetDepth $tooldepth

		#Set tool's opening. Modify the color of the tool according to the opening parameter
		set actor [$tool GetActor 1] 
		set prop [$actor GetProperty]
		$prop SetColor 0 $opening 0
		
		$actor SetProperty $prop

		#Display tool buttons/pedal state
		set state [Haptic GetToolButtonState $i] 
		if {$state != 0} {
			puts "Tool($i) Main button is pressed...\n"
		}
		set state [Haptic GetLeftPedalState]
		if {$state != 0} {
			puts "Tool($i) Left pedal is pressed...\n"
		}
		set state [Haptic GetRightPedalState]
		if {$state != 0} {
			puts "Tool($i) Right pedal is pressed...\n"
		}

		#Set Tool Feedback Forces
		
		if { $tooldepth > 3.5} {
			#Z-Component Depth Force
			set force [expr 2 * [expr $tooldepth - 3.5]]
			puts "Feedback Force: $force N applied to the Haptic Tool($i)"
			Haptic SetToolTipForce $i 0 0 1
		}	else {
			Haptic SetToolTipForce $i 0 0 0
		}

		Haptic ApplyForce $i
	}

	Haptic FlushForces
	
	#puts "Scenario Update"
	Scenario Update
}

proc good_idle_proc {} {

	UpdateHaptic
    after idle {after 0 good_idle_proc};  # "Immediately" after idle
    after 10;  # Sleep for 10 ms to simulate a heavily loaded machine

}

puts "\t Loading VTK in progress ...\n"
package require vtk
puts "\t VTK loaded.\n"
puts "\t Loading VTKEsqui in progress ...\n"
package require vtkesqui
puts "\t VTKEsqui loaded.\n"
puts "\t Loading VTKInteraction in progress ...\n"
package require vtkinteraction
puts "\t VTKInteraction loaded.\n"

# IF A HAPTIC DEVICE IS AVAILABLE, UNCOMMENT THE FOLLOWING LINE TO USE IT
puts "\t Loading vtkesquiHaptics in progress ...\n"
package require vtkesquiHaptics
::vtk::load_component vtkesquiHapticsTCL
puts "\t vtkesquiHaptics loaded.\n"

#### Render Window Definitions ####
vtkRenderer ren1
ren1 SetBackground 1.0 1.0 1.0

vtkRenderWindow renWin
renWin AddRenderer ren1
renWin SetSize 840 480

vtkRenderWindowInteractor iren
iren SetRenderWindow renWin

vtkTransform cameraTransform
vtkCamera camera
#camera SetUserTransform cameraTransform

#### Initialize Camera Position ####
camera SetFocalPoint 0 0 -12
camera SetPosition 0 0 0
camera Yaw 0
camera Elevation 20
camera Pitch -20
camera Dolly 1

ren1 SetBackground 0.9 0.8 0.8
ren1 SetActiveCamera camera

#### Creation & Initialization of Scenario ####
vtkScenario Scenario
Scenario SetRenderWindow renWin 

#Instance of vtkCylinderSource and set some of its properties.
vtkCylinderSource cylinder
cylinder SetHeight 12.0
cylinder SetRadius 0.3
cylinder SetResolution 20
cylinder Update

vtkCubeSource cube
cube SetXLength 1
cube SetYLength 1.5
cube SetZLength 1
cube Update

#A Transform will be performed to the polyData to adjust it to VTK coordinate system
# Whole Tool is rotated -90 degrees at x axis. Set in a horizontal plane
vtkTransform transformRotate
transformRotate RotateX 90

#Ball is shifted in depth (note negative sign)
vtkTransform transformShift
transformShift Translate 0 0 -6

vtkTransformPolyDataFilter transformFilter
transformFilter SetTransform transformRotate

vtkTransformPolyDataFilter shiftFilter
shiftFilter SetTransform transformShift

transformFilter SetInput [cylinder GetOutput]
transformFilter Update

### Primary (left) Tool ###

#A cylinder + cube  will emulate the tool
vtkToolDummy Tool0
#A new copy of the PolyData is made in tool SetPolyData()
Tool0 SetPolyData 0 [transformFilter GetOutput]

transformFilter SetInput [cube GetOutput]
transformFilter Update

shiftFilter SetInput [transformFilter GetOutput]
shiftFilter Update

#A new copy of the PolyData is made in tool SetPolyData()
Tool0 SetPolyData 1 [shiftFilter GetOutput]

vtkProperty cylinderProp
cylinderProp SetColor 0 0 0
cylinderProp SetAmbient 100
cylinderProp SetSpecular 200
cylinderProp SetOpacity 0.5

set actor [Tool0 GetActor 0]
$actor SetProperty cylinderProp

Tool0 SetPosition -3 0 0
Tool0 SetOrientation 0 -10 0

Scenario InsertNextTool Tool0

### Secondary (right) tool ###
vtkToolDummy Tool1
#A cylinder + cube will emulate the tool
#Instance of vtkCylinderSource and set some of its properties.
transformFilter SetInput [cylinder GetOutput]
transformFilter Update

#A new copy of the PolyData is made in tool SetPolyData()
Tool1 SetPolyData 0 [transformFilter GetOutput]
transformFilter SetInput [cube GetOutput]
transformFilter Update

shiftFilter SetInput [transformFilter GetOutput]
shiftFilter Update

#A new copy of the PolyData is made in tool SetPolyData()
Tool1 SetPolyData 1 [shiftFilter GetOutput]

vtkProperty cylinderProp1
cylinderProp1 SetColor 0 0 0
cylinderProp1 SetAmbient 100
cylinderProp1 SetSpecular 200
cylinderProp1 SetOpacity 0.5
set actor [Tool1 GetActor 0] 
$actor SetProperty cylinderProp1

Tool1 SetPosition 3 0 0
Tool1 SetOrientation 0 10 0

Scenario InsertNextTool Tool1

#Display Axes
vtkTransform transform
transform Translate 1.0 0.0 0.0

vtkAxesActor axes
axes SetTotalLength 2 2 2

#The axes are positioned with a user transform
#axes SetUserTransform transform

#ren1 AddActoraxes

#iren Initialize();

### Haptic Initialization ###

#Create an vtkIHP Haptic Object
vtkIHP Haptic
set connected [Haptic Init]

if {$connected > 0} {
	puts "IHP Device is connected..."
} else {
	puts "IHP Device is not connected..."
	puts "Please startup device and restart the application"
	exit 0
}

# prevent the tk window from showing up then start the event loop
wm withdraw . 

good_idle_proc
