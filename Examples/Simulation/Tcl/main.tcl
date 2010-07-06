
#--------------------
#Program: ESQUI
#Info and Bugs: {marf,rsantana}@ctm.ulpgc.es 
#-------------------------------------------
#
#Copyright (c) 2006, Center for Technology in Medicine, University of Las Palmas
#de Gran Canaria (Canary Islands, Spain).
#All rights reserved.
#
#This program is free software; you can redistribute it and/or modify it under 
#the terms of the Creative Commons' "by-attribution" license 
#(http://creativecommons.org/licenses/by/2.5).
#
#Redistribution and use in source and binary forms, with or without
#modification, are permitted provided that the following conditions are met:
#
#1) Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
#2) Redistributions in binary form must reproduce the above copyright 
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
#
#
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
#AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
#ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
#DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
#SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
#OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#=============================================================================



#Especifica la libreria necesaria

global Scenario
global Simulation
global SimulationManager
global renWin

puts "\t Loading vtk in progress ...\n"
package require vtk
puts "\t vtk loaded.\n"
puts "\t Loading vtkInteraction in progress ...\n"
package require vtkinteraction
puts "\t vtkInteraction loaded.\n"
puts "\t Loading vtkESQui in progress ...\n"
package require vtkesqui
puts "\t vtkESQui loaded.\n"


# IF A LSW HAPTIC DEVICE FROM IMMERSION CORPORATION IS AVAILABLE, UNCOMMENT THE FOLLOWING LINE TO USE IT
catch {package require vtkesquiHaptics}
catch {::vtk::load_component vtkesquiHapticsTCL}

#Carga las fuentes de los procedimientos para poder utilizarlos
puts "\t Loading TCL modules in progress ...\n"

source Window.tcl
source Simulation.tcl
source Events.tcl  
#source LoadSceneComponents.tcl
#source LoadSceneTools.tcl
#source LoadAndRunExerciseFromSRML.tcl
#source SRMLParser.tcl

#Este script contiene los procedimientos desarrollados
#para el control de los dispositivos h�pticos
#source HapticDeviceControl.tcl 
puts "\t TCL modules loaded.\n"

puts "Setting up the system, done. \n\n"

# Initialize and Load GUI
InitWindow
LoadWindow
