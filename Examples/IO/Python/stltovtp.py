#!/usr/bin/env python

import sys
import vtk

if (len(sys.argv) == 1):
    print "This script should at least contain 1 argument.\nUsage: vtkpython stltovtp.py $inputFile"
    sys.exit()
    
path = sys.argv[1]
opath = "/tmp/tmp.vtp"
if len(sys.argv) > 2:
    opath = sys.argv[3]
    
reader = vtk.vtkSTLReader()
reader.SetFileName( path )
reader.Update()

input = reader.GetOutput()

nPoints = input.GetNumberOfPoints()
nCells = input.GetNumberOfCells()

print "Input Mesh \nPoints: "+str(nPoints)+"\nCells: "+str(nCells)

writer = vtk.vtkXMLPolyDataWriter()
writer.SetInput(input)
writer.SetFileName( opath )
writer.SetDataModeToAscii()
writer.Write()

print "Output Mesh \nPoints: "+str( input.GetNumberOfPoints() )+"\nCells: "+str( input.GetNumberOfCells() )