
set ToolDepth0 0
set ToolDepth1 0

proc UpdateHaptic {} {
	
	global ToolDepth0
	global ToolDepth1
	
	#Update state of the IHP device
	Haptic UpdateDevice

	for {set i 0} {$i<2} {incr i} {

		#Trocar's direction angles
		set yaw [Haptic GetTrocarYaw $i]
		set pitch [Haptic GetTrocarPitch $i]

		#Tool-in-the-trocar parameters
		set depth [Haptic GetToolDepth $i]
		set roll [Haptic GetToolRoll $i]
		set opening [Haptic GetToolOpening $i]

		set tool ToolActor$i
		set itooldepth $ToolDepth0
		if {$i == 1} {set itooldepth $ToolDepth1}
		
		#puts "Yaw: $yaw, Pitch: $pitch, Roll: $roll"
		
		# Haptic coordinate system differs from VTK system
		# | Haptic  |   VTK  	|
		# |    X    |    Y    |
		# |    Y    |    Z    |
		# |    Z    |    X    |
		$tool SetOrientation $pitch $yaw 0

		#Set tool's roll angle
		$tool RotateZ $roll

		#Set tool's depth
		set tooldepth [expr 20 * $depth]
		set step [expr $itooldepth - $tooldepth]
		ToolTransform$i Translate 0 0 $step
		set ToolDepth$i $tooldepth
	
		puts "$i itooldepth: $itooldepth, tooldepth: $tooldepth, step: $step"
		
		#Set tool's opening. Modify the color of the tool according to the opening parameter
		set prop [$tool GetProperty]
		$prop SetColor 0 0 $opening
		
		$tool SetProperty $prop

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
	renWin Render
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

#A Transform will be performed to the polyData to adjust it to VTK coordinate system
# Whole Tool is rotated -90 degrees at x axis. Set in a horizontal plane
vtkTransform transformRotate
transformRotate RotateX 90

vtkTransformPolyDataFilter transformFilter
transformFilter SetTransform transformRotate

transformFilter SetInput [cylinder GetOutput]
transformFilter Update

### Primary (left) Tool ###
#A cylinder will emulate the tool
vtkPolyData cyl0
cyl0 DeepCopy [transformFilter GetOutput]

vtkTransformPolyDataFilter toolFilter0
vtkTransform ToolTransform0

toolFilter0 SetInput cyl0
toolFilter0 SetTransform ToolTransform0

vtkDataSetMapper tool0Mapper
tool0Mapper SetInput [toolFilter0 GetOutput]

vtkActor ToolActor0
ToolActor0 SetMapper tool0Mapper

vtkProperty cylinderProp
cylinderProp SetColor 0 0 0
cylinderProp SetAmbient 100
cylinderProp SetSpecular 200
cylinderProp SetOpacity 0.5
ToolActor0 SetProperty cylinderProp
ToolActor0 AddPosition -3 0.5 0

### Secondary (right) tool ###
#A cylinder will emulate the tool
vtkPolyData cyl1
cyl1 DeepCopy cyl0

vtkTransformPolyDataFilter toolFilter1
vtkTransform ToolTransform1

toolFilter1 SetInput cyl1
toolFilter1 SetTransform ToolTransform1

vtkDataSetMapper tool1Mapper
tool1Mapper SetInput [toolFilter1 GetOutput]

vtkActor ToolActor1
ToolActor1 SetMapper tool1Mapper

vtkProperty cylinderProp1
cylinderProp1 SetColor 0 0 0
cylinderProp1 SetAmbient 100
cylinderProp1 SetSpecular 200
cylinderProp1 SetOpacity 0.5
ToolActor1 SetProperty cylinderProp1
ToolActor1 AddPosition 3 0.5 0

ren1 AddActor ToolActor0
ren1 AddActor ToolActor1

#Display Axes
vtkTransform transform
transform Translate 1.0 0.0 0.0

vtkAxesActor axes
axes SetTotalLength 2 2 2

#The axes are positioned with a user transform
#axes SetUserTransform transform

#ren1 AddActoraxes

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
