
proc UpdateHaptic {} {
	
	## Pedal Operations ##

	#Update state of the IHP device
	Haptic UpdateDevice

	## Camera Operations ##
	#Get left & right joystick state
	set rightY [Haptic GetRightJoystickYPosition]
	set rightX [Haptic GetRightJoystickXPosition]
	set leftY [Haptic GetLeftJoystickYPosition]
	set leftX [Haptic GetLeftJoystickXPosition]

	set camera [Ren1 GetActiveCamera]
	vtkTransform cameraTransform
	
	set cameraX [expr $rightX/1000.0]
	set cameraY [expr $rightY/1000.0]
	#Translate the camera in accordance with the right joystick state
	cameraTransform Translate $cameraX $cameraY 0

	#Modify camera azimuth/elevation in accordance with the left joystick state
	camera Azimuth [expr $leftX*0.5]
	camera Elevation [expr $leftY*0.5]
	
	#Set the zoom for the camera in function of the VSP buttons state
	set state [Haptic GetZoomState]
	if {$state == 1} {
		camera Dolly 1.01
	} else {
		camera Dolly 0.99
	}
	
	## Catheter Operations ##
	for {set i 0} {$i<3} {incr i} {
		set state [Haptic GetCatheterState $i]
		if {$state == 1} {
			#Modify Sphere radius depending on catheter width (0=8F, 1=5F, 2=2.5F)
			set radius [expr 0.3/[$i+1]]
			Sphere SetRadius radius
		 
			#Get the position of the catheter inside the VSP device
			set depth [Haptic GetCatheterDepth $i]
		 
			#Update the position of the stent & catheter inside of the cylinder
			set factor 0.02;
		
			#double  position[3];
			#CatheterActor->GetPosition(position);
			#CatheterActor->SetPosition(0, factor*depth, 0);
			#StentActor->GetPosition(position);
			#StentActor->SetPosition(0, factor*depth, 0);

			#Get the catheter roll angle
			set roll [Haptic GetCatheterRoll $i]
			CatheterActor RotateY roll
			StentActor RotateY roll
	 	}
 	}
	
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
vtkRenderer Ren1
Ren1 SetBackground 1.0 1.0 1.0

vtkRenderWindow renWin
renWin AddRenderer Ren1
renWin SetSize 840 480

vtkRenderWindowInteractor iren
iren SetRenderWindow renWin

vtkCamera camera

#### Initialize Camera Position ####
camera SetFocalPoint 0 0 -12
camera SetPosition 0 0 0
camera Yaw 0
camera Elevation 20
camera Pitch -20
camera Dolly 1

Ren1 SetActiveCamera camera

#### Creation & Initialization of Scenario ####
vtkScenario Scenario
Scenario SetRenderWindow renWin 

#Instance of vtkCylinderSource and set some of its properties.
vtkCylinderSource cylinder
cylinder SetHeight 13.0
cylinder SetRadius 1
cylinder SetResolution 20
cylinder Update

vtkTextureMapToCylinder textureMapper
textureMapper SetInput [cylinder GetOutput]
textureMapper PreventSeamOn

vtkTransformTextureCoords xForm
xForm SetInput [textureMapper GetOutput]
xForm SetScale 1 8 1

vtkDataSetMapper mapper
mapper SetInput [xForm GetOutput]

#Set The Texture
vtkJPEGReader jpegReader
jpegReader SetFileName "CatheterTexture.jpg"

vtkTexture texture
texture SetInputConnection [jpegReader GetOutputPort]
texture InterpolateOn

vtkActor CylinderActor
CylinderActor SetMapper mapper
CylinderActor SetTexture texture

vtkProperty cylinderProp
cylinderProp SetColor 0.2 0.1 0
cylinderProp SetAmbient 100
cylinderProp SetSpecular 200
cylinderProp SetOpacity 0.5

CylinderActor SetProperty cylinderProp

#A small cylinder will emulate the catheter
vtkCylinderSource cath
cath SetHeight 2.0
cath SetRadius 0.3
cath SetResolution 20
vtkDelaunay3D del2
del2 SetInput [cath GetOutput[]]
del2 SetTolerance 0.01

vtkDataSetMapper cathMapper
cathMapper SetInputConnection [del2 GetOutputPort[]]

vtkActor CatheterActor
CatheterActor SetMapper cathMapper

vtkProperty prop
prop SetColor 0.5 0.7 1
prop SetOpacity 0.8

#A sphere will emulate the stent
vtkSphereSource	Sphere
Sphere SetRadius 0.3
Sphere SetCenter 0 -7 0
Sphere SetPhiResolution 12
Sphere SetThetaResolution 12

vtkPolyDataMapper sphereMapper
sphereMapper SetInput [Sphere GetOutput]

#Global Variable
vtkActor StentActor
StentActor SetMapper sphereMapper

#Add Actors to the renderer
Ren1 AddActor CylinderActor
Ren1 AddActor CatheterActor
Ren1 AddActor StentActor

#Display Axes
vtkTransform transform
transform Translate 1.0 0.0 0.0

vtkAxesActor axes
axes SetTotalLength 2 2 2

#The axes are positioned with a user transform
#axes SetUserTransform transform

#ren1 SetBackground 0.9 0.8 0.8
#ren1 AddActoraxes

#iren Initialize();

### Haptic Initialization ###

#Create an vtkIHP Haptic Object
vtkVSP Haptic
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
