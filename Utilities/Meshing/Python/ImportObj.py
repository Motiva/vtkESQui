#!/usr/bin/env python

import sys
import vtk

if (len(sys.argv) == 1):
    print "This script should at least contain 1 argument.\nUsage: vtkpython ImportObj.py $inputFile"
    sys.exit()
    
path = sys.argv[1]
opath = "/tmp/tmp.vtp"
if len(sys.argv) > 2:
    opath = sys.argv[3]

reader = vtk.vtkOBJReader()
reader.SetFileName(path)
reader.Update()

mesh = reader.GetOutput()

print "Input mesh file:\n"+path+"\n------------------\nnp:"+str(mesh.GetNumberOfPoints())+"\tnc:"+str(mesh.GetNumberOfCells())+"\n------------------\n"

writer = vtk.vtkXMLPolyDataWriter()
writer.SetInput(mesh)
writer.SetFileName( opath )
writer.SetDataModeToAscii()
writer.Write()

print "Output mesh file\n"+opath+"\n------------------\nPoints: "+str( mesh.GetNumberOfPoints() )+"\nCells: "+str( mesh.GetNumberOfCells() )
