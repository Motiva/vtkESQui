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
	Scenario
	sleep
}

#catch {package require vtkesquiHaptics}
#catch {::vtk::load_component vtkesquiHapticsTCL}

# Path of the importer filename
set path "/home/jballesteros/Workspace/data/vtkESQuiData/"
set name "/home/jballesteros/Workspace/data/vtkESQuiData/lapPS.srml"

# Create renderer window
vtkRenderer ren1
vtkRenderWindow renWin
renWin AddRenderer ren1
renWin SetSize 800 600

# Default window interactor
vtkRenderWindowInteractor iren
iren SetRenderWindow renWin
iren SetInteractorStyle ""

# Create the scenario
vtkScenario Scenario
Scenario SetRenderWindow renWin

# New simulation
vtkSimulation Simulation
Simulation SetScenario Scenario

# Set interactor
vtkSimulationInteractorStyle style
style SetScenario Scenario
iren SetInteractorStyle style

puts "Simulation has been created."

puts "Importing simulation from SRML file..."
vtkSRMLImporter SRMLImporter
SRMLImporter SetDataPath $path
SRMLImporter SetFileName $name
SRMLImporter SetSimulation Simulation
SRMLImporter Read

puts "Simulation has been correctly imported..."

Simulation Init

# render the image
#
iren AddObserver UserEvent {wm deiconify .vtkInteract}
iren Initialize

# prevent the tk window from showing up then start the event loop
wm withdraw .

sleep

Simulation Run

