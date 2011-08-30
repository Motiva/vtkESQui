#!/usr/bin/env python

##############################################################################
# This scripts generates a vtp file (*vtkPolyData*) that satisfies the 
# vtkPSSInterface_ input data requirements, by generating a VTK_LINE cell mesh
# from VTK_TRIANGLE cell mesh. Every cell point it will be connected to its 
# n-neighborhood point by a VTK_LINE cell. For example, for n=1, each point is
# connected to every point of the cells it belongs. The biggest the n, the 
# more rigid the mesh will become.
#
# Usage:
# vtkpython BuildRigidityMesh.py $inputFile $rigidityFactor $outputFile
##############################################################################


import sys
import vtk

def containsLine(lines, line):
    ids = vtk.vtkIdList()
    lines.InitTraversal()
    c = lines.GetNextCell( ids )
    while c:
        if ( ( (ids.GetId(0) == line.GetPointId(0)) and
               (ids.GetId(1) == line.GetPointId(1)) ) or
             ( (ids.GetId(1) == line.GetPointId(0)) and
               (ids.GetId(0) == line.GetPointId(1) ) ) ):
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
        #print "s("+str(id0)+","+str(id1)+")"

if (len(sys.argv) == 1):
    print "This script should at least contain 1 argument.\nUsage: vtkpython BuildRigidityMesh.py $inputFile $rigidityFactor $outputFile"
    sys.exit()
    
path = sys.argv[1]
f = 1
c = 0
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

lines = vtk.vtkCellArray()

pIds = vtk.vtkIdList()
cIds = vtk.vtkIdList()
cpIds = vtk.vtkIdList()
pIds1 = vtk.vtkIdList()
cIds1 = vtk.vtkIdList()
cpId1s = vtk.vtkIdList()

for i in range (0, input.GetNumberOfPoints() ):
    # Structural Springs
    # Build a line (spring) to the rest of cell points
    
    input.GetPointCells(i, cIds)
    for j in range (0, cIds.GetNumberOfIds() ):
        cId = cIds.GetId(j)
        input.GetCellPoints(cId, pIds)
        for k in range (0, pIds.GetNumberOfIds()):
            pId  = pIds.GetId(k)
            if((i != pId) and (i < pId) ):
                insertLine(lines, i, pId)
                # Shearing Springs
                if(f == 2):
                    input.GetPointCells(pId, cIds1)
                    for l in range (0, cIds1.GetNumberOfIds() ):
                        cId1 = cIds1.GetId(l)
                        input.GetCellPoints(cId1, pIds1)
                        for m in range (0, pIds1.GetNumberOfIds()):
                            pId1  = pIds1.GetId(k)
                            
                            if((i != pId1) and (i < pId1)):
                                insertLine(lines, i, pId1)

    

mesh = vtk.vtkPolyData()
mesh.SetPoints( input.GetPoints() )
mesh.SetLines( lines )

mesh.GetPointCells(0, cIds)
print "cell[0] Ids: "+str(cIds.GetNumberOfIds())

writer = vtk.vtkXMLPolyDataWriter()
writer.SetInput(mesh)
writer.SetFileName( opath )
writer.SetDataModeToAscii()
writer.Write()

print "Output Mesh \nPoints: "+str( mesh.GetNumberOfPoints() )+"\nCells: "+str( mesh.GetNumberOfCells() )