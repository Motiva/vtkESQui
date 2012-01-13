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
import math
import vtk
import vtkesqui

if (len(sys.argv) == 1):
    print "This script should at least contain 1 argument.\nUsage: vtkpython BuildCollisionModel.py $inputFile $maxNumberOfPoints $tolerance $outputFile"
    sys.exit()
    
path = sys.argv[1]
f = 20
tol = 0.0
opath = "/tmp/out_col.vtp"
if len(sys.argv) > 2:
    f = int(sys.argv[2])
if len(sys.argv) > 3:
    tol = float(sys.argv[3])
if len(sys.argv) > 4:
    opath = sys.argv[4]
    
reader = vtk.vtkXMLPolyDataReader()
reader.SetFileName( path )
reader.Update()

source = reader.GetOutput()

#writer = vtk.vtkXMLPolyDataWriter()
#writer.SetInput(source)
#writer.SetFileName(path)
#writer.SetDataModeToAscii()
#writer.Write()

#Calculate position and size
b = source.GetBounds()
size = [abs(b[1]-b[0]), abs(b[3]-b[2]), abs(b[5]-b[4])]
center = [0, 0, 0]
for i in range(len(size)):
  center[i] = b[2*i]+size[i]/2
radius = max(size)

# Calculate resolution
res = math.ceil(math.sqrt(f))

#Define model input
sphere = vtk.vtkSphereSource()
sphere.SetPhiResolution(res)
sphere.SetThetaResolution(res)
sphere.SetRadius(radius)
sphere.Update()

input = sphere.GetOutput()

#Synchronize point positions
syncfilter = vtkesqui.vtkSyncPolyDataFilter()
syncfilter.SetInput(input)
syncfilter.SetSource(source)
syncfilter.Update()
o = syncfilter.GetOutput()

#Remove duplicated points
clean = vtk.vtkCleanPolyData()
clean.SetInput(syncfilter.GetOutput())
clean.SetTolerance(tol)
clean.Update()

output = clean.GetOutput()
output.BuildLinks()

#Remove cells that are not triangles
for i in range(output.GetNumberOfCells()):
  if(output.GetCellType(i) != 5):
    output.DeleteCell(i)

output.RemoveDeletedCells()

#for i in range(output.GetNumberOfCells()):
#  print str(i)+":"+str(output.GetCellType(i))

writer = vtk.vtkXMLPolyDataWriter()
writer.SetInput(output)
writer.SetFileName(opath)
writer.SetDataModeToAscii()
writer.Write()

print "Output Mesh:\n"+opath+"\nPoints: "+str( output.GetNumberOfPoints() )+"\nCells: "+str( output.GetNumberOfCells() )+"\nSuccesfully generated..."