package require vtk

if { $argc != 2 } {
	puts "The command requires 2 arguments: resolution size"
	puts "e.g.: vtk Cuboid.tcl 6.0 2.0"
	puts "Please try again."
	exit
}

set resolution [lindex $argv 0]
set size [lindex $argv 1]

vtkPoints points

set dx $size/$resolution
set dy $size/$resolution
set dz $size/$resolution

set xh $size/2
set yh $size/2
set zh $size/2

for {set x 0} {$x<$resolution} {incr x} {
	for {set y 0} {$y<$resolution} {incr y} {
		for {set z 0} {$z<$resolution} {incr z} {
			set px [expr [expr $x * $dx] - $xh]
			set py [expr [expr $y * $dy] - $yh]
			set pz [expr [expr $z * $dz] - $zh]
			#puts "point: $px, $py, $pz"
			points InsertNextPoint $px $py $pz
		}
	}
}

vtkPolyData pd
pd SetPoints points

vtkDelaunay3D del
del SetInput pd

set mesh [del GetOutput]

vtkXMLUnstructuredGridWriter writer
writer SetDataModeToAscii
writer SetInput $mesh
set filename "cuboid.vtu"
writer SetFileName $filename
writer Update