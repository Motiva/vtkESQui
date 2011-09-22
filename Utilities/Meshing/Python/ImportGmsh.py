import sys
import getfem

if (len(sys.argv) == 1):
    print "This script should at least contain 1 argument.\nUsage: python ImportMsh.py $inputFile"
    sys.exit()
    
path = sys.argv[1]
opath = "/tmp/out.vtk"

m = getfem.Mesh('import', "gmsh", path);

ncvs = m.nbcvs();
npts = m.nbpts();

print "Mesh succesfully imported...";
print "npts: "+str(npts);

m.export_to_vtk(opath,'ascii');