#!/usr/bin/env python

import sys
import vtk

if (len(sys.argv) == 1):
    print "This script should at least contain 1 argument.\nUsage: vtkpython ImportGmsh.py $inputFile"
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
vl = 0
nl = [0,0]
el = [0,0]
v = [0,0,0,0]

for l in lines:
  lc+=1
  lwc=0
  for w in l.split():
    wc+=1
    lwc+=1
    #parse input data
    if vl == 0 and lwc == 1:
      if w == "$MeshFormat":
        vl = lc+1
    elif (nl[0] == 0 or nl[1] == 0) and lwc == 1:
      if w == "$Nodes":
        np = lines[lc].split()[0]
        print np
        nl[0] = lc + 1
        nl[1] = lc + int(np) + 1
    elif (el[0] == 0 or el[1] == 0) and lwc == 1:
      if w == "$Elements":
        nc = lines[lc].split()[0]
        el[0] = lc + 1
        el[1] = lc + int(nc) + 1

print "nl:"+str(nl)+"\nel:"+str(el)+"\n"


#fill in points
for i in range(nl[0], nl[1]):
  lwc=0
  for w in lines[i].split():
    lwc+=1
    if lwc > 1:
      v[int(lwc-2)] = float(w)
  #print str(i)+str(v)
  points.InsertNextPoint(v[0],v[1],v[2])
  
#fill in cells
fill = 0
o = 0
for i in range(el[0], el[1]):
  lwc=0
  fill=0
  for w in lines[i].split():
    lwc+=1
    if lwc == 2:
      if w == "4":
        fill = 1
      else:
        print "["+str(i)+"]"+str(w)
    if lwc == 3:
      o = int(w) + 3
    elif fill and lwc > o:
      v[int(lwc-o-1)] = int(w)
  
  if fill == 1:
    #print str(i+1)+str(v)
    id = cells.InsertNextCell(4)
    for i in range(4):
      cells.InsertCellPoint(v[i])

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
