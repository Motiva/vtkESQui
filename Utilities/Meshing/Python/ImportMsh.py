#!/usr/bin/env python

import sys
import vtk

if (len(sys.argv) == 1):
    print "This script should at least contain 1 argument.\nUsage: vtkpython ImportMsh.py $inputFile"
    sys.exit()
    
path = sys.argv[1]
opath = "/tmp/tmp.vtu"
if len(sys.argv) > 2:
    opath = sys.argv[3]

points = vtk.vtkPoints()
cells = vtk.vtkCellArray()

#Parse file
f = open(path, 'r')
lines = f.readlines()
lc = 0
lwc = 0
wc = 1
np = 0
nc = 0
v = [0,0,0,0]

for l in lines:
  lc+=1
  lwc=0
  for w in l.split():
    wc+=1
    lwc+=1
    #parse input data
    if lc == 1 and lwc == 1:
      np = int(w)
    elif lc == 2 and lwc == 1:
      nc = int(w)
    elif lc > 2:
      v[int(lwc-1)] = float(w)

  #fill in point/cell data    
  if lc > 2 and lc < np+3:
    points.InsertNextPoint(v[0],v[1],v[2])
  elif lc >= np+3:
    id = cells.InsertNextCell(4)
    #print v
    for i in range(4):
      cells.InsertCellPoint(int(v[i]))

print "Input mesh file:\n"+path+"\n------------------\nl:"+str(lc)+"\tw:"+str(wc)+"\nnp:"+str(np)+"\tnc:"+str(nc)+"\n------------------\n"

mesh = vtk.vtkUnstructuredGrid()
mesh.SetPoints(points)
mesh.SetCells(10, cells)

writer = vtk.vtkXMLUnstructuredGridWriter()
writer.SetInput(mesh)
writer.SetFileName( opath )
writer.SetDataModeToAscii()
writer.Write()

print "Output mesh file\n"+opath+"\n------------------\nPoints: "+str( mesh.GetNumberOfPoints() )+"\nCells: "+str( mesh.GetNumberOfCells() )
