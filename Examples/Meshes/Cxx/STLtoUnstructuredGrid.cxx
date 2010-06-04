/*==========================================================================
--------------------------------------------------
Program: ESQUI
Info and Bugs: {marf,jballesteros}@itccanarias.org
url: http://motivando.me 
--------------------------------------------------

Copyright (c) 2006-2007, Center for Technology in Medicine (CTM), 
University of Las Palmas de Gran Canaria (ULPGC), Canary Islands, Spain.
Copyright (c) 2007-2010, Institute of Technology at CanaryIslands (ITC),
Canary Islands, Spain.

This software is free software; you can redistribute it and/or modify it 
under the terms of the GNU Lesser General Public License (LGPL) as published
by the Free Software Foundation, either version 3 of the License, or (at 
your option) any later version.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1) Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2) Redistributions in binary form must reproduce the above copyright 
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

You should have received a copy of the GNU Lesser General Public License 
along with this program.  If not, see <http://www.gnu.org/licenses/>.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.
==========================================================================*/
#include "vtkUnstructuredGrid.h"
#include "vtkUnstructuredGridReader.h"
#include "vtkUnstructuredGridWriter.h"
#include "vtkPolyData.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyDataWriter.h"
#include "vtkPolyDataNormals.h"

#include "vtkSTLReader.h"
#include "vtkDelaunay3D.h"

#include <iostream>

const char * ExpandDataFileName(const char * fname)
{
	//const char * path ="C:/Workspace/data/vtkESQuiData";
	const char * path ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Meshes";
	char * fullName;
	fullName = new char[strlen(path) + 1 + strlen(fname)];
	fullName[0] = 0;
	strcat(fullName, path);
	size_t len = strlen(fullName);
	fullName[len] = '/';
	fullName[len+1] = 0;
	strcat(fullName, fname);

	return fullName;
}

int main(int ac, char* av[])
{
	
	vtkSTLReader * stl = vtkSTLReader::New();
	stl->SetFileName(ExpandDataFileName("sphere2.stl"));
	stl->Update();

	vtkPolyData * pd = stl->GetOutput();

	vtkPolyDataNormals *normals = vtkPolyDataNormals::New  ();
	normals->SetInput(pd);
	normals->SetFeatureAngle(60.0);
	normals->FlipNormalsOff();
	normals->Update();

	vtkPolyDataWriter * pdWriter = vtkPolyDataWriter::New();
	pdWriter->SetFileName(ExpandDataFileName("sphere2.vtk"));
	pdWriter->SetInput(normals->GetOutput());
	pdWriter->Update();

	vtkDelaunay3D * del = vtkDelaunay3D::New();
	del->SetInput(stl->GetOutput());
	del->Update();

	vtkUnstructuredGrid * grid = del->GetOutput();

	vtkUnstructuredGridWriter * ugWriter = vtkUnstructuredGridWriter::New();
	ugWriter->SetInput(grid);
	ugWriter->SetFileName(ExpandDataFileName("sphere2Tetra.vtk"));
	ugWriter->Update();

	return 0;
}
