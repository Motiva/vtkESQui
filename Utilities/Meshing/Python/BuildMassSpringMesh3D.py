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

if (len(sys.argv) == 1):
    print "This script should at least contain 1 argument.\nUsage: vtkpython BuildMassSpringMesh3D.py $inputFile"
    sys.exit()
    
path = sys.argv[1]

opath = "/tmp/out_def.vtp"
if len(sys.argv) > 2:
    opath = sys.argv[2]
    
if (str(path).endswith("vtk")):
    reader = vtk.vtkUnstructuredGridReader();
else:
    reader = vtk.vtkXMLUnstructuredGridReader();
    
reader.SetFileName(path)
reader.Update()

input = reader.GetOutput()

print "Input Mesh: "+path+" \nPoints: "+str(input.GetNumberOfPoints())+"\nCells: "+str(input.GetNumberOfCells())

lines = vtk.vtkCellArray()

pIds = vtk.vtkIdList()
cIds = vtk.vtkIdList()
cpIds = vtk.vtkIdList()

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
    

mesh = vtk.vtkPolyData()
mesh.SetPoints( input.GetPoints() )
mesh.SetLines( lines )

writer = vtk.vtkXMLPolyDataWriter()
writer.SetInput(mesh)
writer.SetFileName( opath )
writer.SetDataModeToAscii()
writer.Write()

print "Output Mesh: "+opath+"\nSuccesfully imported...";
