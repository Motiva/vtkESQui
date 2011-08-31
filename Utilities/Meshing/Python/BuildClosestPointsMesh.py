#!/usr/bin/env python

###############################################################################
# This scripts generates a vtp file (vtkPolyData) that satisfies the 
# vtkParticleSpringSystemInterface_ input data requirements, by generating a VTK_LINE cell mesh 
# from VTK_TRIANGLE cell mesh.
#
# Usage:
# vtkpython BuildClosestPointsMesh.py $inputFile $numberOfPoints $outputFile
#
# inputfile      vtp file containing a vtkPolyData object of VTK_TRIANGLE cells
# numberOfPoints Number of closest points for every point.
# outputFile     File path to output vtp file
###############################################################################

import sys
import vtk

def containsLine(lines, line):
    ids = vtk.vtkIdList()
    lines.InitTraversal()
    c = lines.GetNextCell( ids )
    while c:
        if ( ( (ids.GetId(0) == line.GetPointId(0)) and (ids.GetId(1) == line.GetPointId(1)) ) or
             ( (ids.GetId(1) == line.GetPointId(0)) and (ids.GetId(0) == line.GetPointId(1) ) ) ):
            #print "exists"
            return 1
        c = lines.GetNextCell( ids )
    return 0

def insertLine(lines, id0, id1):
    line = vtk.vtkLine()
    lPointIds = line.GetPointIds()
    lPointIds.SetId( 0, id0)
    lPointIds.SetId( 1, id1)
    
    if( containsLine(lines, line) == 0):
        lines.InsertNextCell( line )

if (len(sys.argv) == 1):
    print "This script should at least contain 1 argument.\nUsage: vtkpython BuildClosestPointsMesh.py $inputFile $numberOfPoints $outputFile"
    sys.exit()
    
path = sys.argv[1]
f = 10
opath = "/tmp/tmp_def.vtp"
if len(sys.argv) > 2:
    f = int(sys.argv[2])
if len(sys.argv) > 3:
    opath = sys.argv[3]
    
reader = vtk.vtkXMLPolyDataReader()
reader.SetFileName( path )
reader.Update()

input = reader.GetOutput()

nPoints = input.GetNumberOfPoints()
nCells = input.GetNumberOfCells()

print "Input Mesh \nPoints: "+str(nPoints)+"\nCells: "+str(nCells)

locator = vtk.vtkPointLocator()
locator.SetDataSet( input )
locator.AutomaticOn()
locator.BuildLocator()

lines = vtk.vtkCellArray()

pIds = vtk.vtkIdList()

for i in range (0, input.GetNumberOfPoints() ):
    # Build a line (spring) to the n closest points
    p = input.GetPoint(i)
    locator.FindClosestNPoints(f, p, pIds)
    for j in range (0, pIds.GetNumberOfIds() ):
        pId = pIds.GetId(j)
        if(pId != i):
            #print str(i)+"-"+str(pId)
            insertLine(lines, i, pId)
    

mesh = vtk.vtkPolyData()
mesh.SetPoints( input.GetPoints() )
mesh.SetLines( lines )

writer = vtk.vtkXMLPolyDataWriter()
writer.SetInput(mesh)
writer.SetFileName( opath )
writer.SetDataModeToAscii()
writer.Write()

print "Output Mesh \nPoints: "+str( mesh.GetNumberOfPoints() )+"\nCells: "+str( mesh.GetNumberOfCells() )