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



# This script must run from a editor window in Blender. It take a mesh from the scene and 
# export it to a vtk PolyData

# Este script debe ser ejecutado desde una ventada editora de texto de Blender
# se encarga de tomar una malla presente en la escena de Blender (el nombre de la 
# malla debe ser introducido en el propio script) y pasarla a un polydata de vtk

import vtk
import Blender
from Blender import NMesh
import Tex_VTKBlender
reload (Tex_VTKBlender)
#Se incluye de forma manual el nombre de la malla a convertir
me = NMesh.GetRaw("Put the mesh name to export here")
pdata = Tex_VTKBlender.BlenderToPolyData(me)
esc = vtk.vtkPolyDataWriter()
# También se debe indicar la ubicación y nombre del nuevo archivo .vtk creado
esc.SetFileName("the name of the .vtk file")
esc.SetInput(pdata)
vtk.vtkPolyDataWriter.Write(esc)

