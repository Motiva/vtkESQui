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
#include "vtkFeMeshInterface.h"
#include "vtkObjectFactory.h"

vtkCxxRevisionMacro(vtkFeMeshInterface, "$Revision: 1.2 $");
vtkStandardNewMacro(vtkFeMeshInterface);

//----------------------------------------------------------------------------
vtkFeMeshInterface::vtkFeMeshInterface()
{
	this->FeMesh = vtkFeMesh::New();
	this->ClippedMesh = vtkUnstructuredGrid::New();

	this->Bounds = vtkPoints::New();
	this->BoundCells = vtkIdList::New();
	this->Dofs = vtkDoubleArray::New();

	this->SyncIds = vtkIntArray::New();
	this->SyncIds->SetNumberOfComponents(2);
}

//----------------------------------------------------------------------------
vtkFeMeshInterface::~vtkFeMeshInterface()
{

	this->FeMesh->Delete();
	this->ClippedMesh->Delete();

	this->Bounds->Delete();
	this->BoundCells->Delete();
	this->Dofs->Delete();

	this->SyncIds->Delete();
}

//--------------------------------------------------------------------------
void vtkFeMeshInterface::SetInput(vtkUnstructuredGrid * data)
{
	///Set input mesh where deformation will be calculated
	
	//Reset objects that could have been used in previous executions
	this->Mesh->Reset();
	this->ClippedMesh->Reset();
	
	//Copy Data
	this->Mesh->DeepCopy(data);
	this->updated = 0;
	
}

//--------------------------------------------------------------------------
void vtkFeMeshInterface::Init()
{
	//TODO: Init FeMeshInterface
	std::cout << GetClassName() << "::Init" << std::endl;
}

//--------------------------------------------------------------------------
void vtkFeMeshInterface::Update()
{
	std::cout << GetClassName() << "::Update" << std::endl;
	//Clip Mesh to enhance mesh deformation calculus
	this->ClipMesh();

	//Build Tetrahedral mesh for FEM Deformation
	this->GenerateTetrahedralMesh();

	//Calculate synchronization Ids between mesh & clip points (this->syncIds)
	this->ComputePointIds();

	//Calculate clipped mesh cells related to contact points (this->ContactPointIds)
	this->ComputePointCells();

	//Calculate clipped mesh bound cells (this->boundCells)
	this->ComputeBoundCells();

	//Set input mesh to FEM deformation calculus
	this->FeMesh->SetInput(this->ClippedMesh);
	
	//Set Boundary Conditions to the FEM Model
//	this->femMesh->SetBoundaryConditions(this->boundCells, this->dofs);

	//Set external forces to the FEM Model
	this->FeMesh->SetContacts(this->ContactCellIds, this->Forces);

	//Update the original mesh with the resulted deformation
	this->FeMesh->Update();

	//Return deformed mesh
	vtkUnstructuredGrid *deformedGrid = this->FeMesh->GetOutput();

	//Update the mesh with deformed points
	this->ClippedMesh->DeepCopy(deformedGrid);

	//Rebuild mesh with the deformed points
	this->RebuildMesh();

	//Clear previous contacts
	this->Clear();

	this->Updated = 1;
}

//--------------------------------------------------------------------------
vtkUnstructuredGrid * vtkFeMeshInterface::GetOutput()
{
	if(!this->Updated)
	{
		this->Update();
	}
	
	return this->Mesh;
}

//--------------------------------------------------------------------------
void vtkFeMeshInterface::ClipMesh()
{
	//Get bounds of collision points
	double bounds[6];
	this->ContactPoints->GetBounds(bounds);
	double center[3] = {(bounds[0]+(bounds[1]-bounds[0])/2),
 											(bounds[2]+(bounds[3]-bounds[2])/2),
 											 (bounds[4]+(bounds[5]-bounds[4])/2)};

	//Original Mesh
	vtkUnstructuredGrid * grid = this->Mesh;

	//Resulting Mesh
	vtkUnstructuredGrid * subGrid;

	//ExtractionFilter
	vtkExtractGeometry * extract = vtkExtractGeometry::New();
	extract->SetInput(grid);
	extract->ExtractBoundaryCellsOn();

	//Extraction Sphere
	vtkSphere * sphere = vtkSphere::New();
	double radius = 0.001;
	sphere->SetCenter(center);

	vtkIdType numberOfPoints = 0;
	vtkIdType numberOfCells = 0;

	//Iterate until a decent mesh for FEM-based deformation is obtained
	while(numberOfPoints < 30 ||
					//numberOfCells != this->Mesh->GetNumberOfCells() ||
				numberOfCells < 60)
	{
		sphere->SetRadius(radius);

		//Clip Mesh to reduce calculation points
		extract->SetImplicitFunction(sphere);
		extract->Update();
		subGrid = extract->GetOutput();

		//Update number of points and cells
		numberOfPoints = subGrid->GetNumberOfPoints();
		numberOfCells = subGrid->GetNumberOfCells();
		//std::cout << "this->clip numberOfPoints: " << numberOfPoints << "\n";
		//std::cout << "this->clip numberOfCells: " << numberOfCells << "\n";

		//Increment clipping range
		radius += 0.005;

	}

	this->ClippedMesh->DeepCopy(subGrid);

	std::cout << "[vtkFeMeshInterface] Clipped grid #points: " << ClippedMesh->GetNumberOfPoints() << std::endl;
	std::cout << "[vtkFeMeshInterface] Clipped grid #cells: " << ClippedMesh->GetNumberOfCells() << std::endl;
/*
	std::cout << "boundary grid #points: " << clean->GetOutput()->GetNumberOfPoints() << std::endl;
	std::cout << "boundary grid #cells: " << clean->GetOutput()->GetNumberOfCells() << std::endl;
	*/

}

//--------------------------------------------------------------------------
void vtkFeMeshInterface::ComputePointCells()
{
	///Calculate cell Ids of the clipped Tetra Mesh, related to the contact points
	vtkPointLocator * locator =  vtkPointLocator::New();
	locator->SetDataSet(this->ClippedMesh);

	vtkIdList * cellIds = vtkIdList::New();
	vtkIdType cellId;

	//Search the points in the clipped Mesh
	for(vtkIdType j=0; j<this->ContactPoints->GetNumberOfPoints(); j++)
	{
		double * contact = this->ContactPoints->GetPoint(j);
		vtkIdType pointId = locator->FindClosestPoint(contact);
		std::cout << "[vtkFeMeshInterface] (Clipped Tetra mesh) Contact pointId[" << pointId << "]: " << contact[0] << ", " << contact[1] << ", " << contact[2] << "\n";

		this->ClippedMesh->GetPointCells(pointId, cellIds);
		cellId = cellIds->GetId(0);
		std::cout <<"[vtkFeMeshInterface] (Clipped mesh) Contact cellId: " << cellId << "\n";

		this->ContactPointIds->InsertNextId(cellId);
	}

	locator->Delete();
	cellIds->Delete();
}

//--------------------------------------------------------------------------
void vtkFeMeshInterface::ComputeBoundCells()
{
	///Calculate cell Ids of the clipped Mesh bounds
	vtkPointLocator * locator =  vtkPointLocator::New();
	locator->SetDataSet(this->ClippedMesh);

	vtkIdList * cellIds = vtkIdList::New();
	vtkIdType cellId;

	double dof[3];
	dof[0] = 0.0;
	dof[1] = 0.0;
	dof[2] = 0.0;

	//Search the points in the clipped Mesh
	for(vtkIdType j=0; j<this->Bounds->GetNumberOfPoints(); j++)
	{
		double * bound = this->Bounds->GetPoint(j);
		vtkIdType pointId = locator->FindClosestPoint(bound);
		//std::cout << "boundId: " << pointId << "\n";

		this->ClippedMesh->GetPointCells(pointId, cellIds);
		cellId = cellIds->GetId(0);
		this->BoundCells->InsertNextId(cellId);
		this->Dofs->InsertNextTupleValue(dof);
	}

	locator->Delete();
	cellIds->Delete();
}

//--------------------------------------------------------------------------
void vtkFeMeshInterface::ComputePointIds()
{
	vtkPoints * clippedPoints = this->ClippedMesh->GetPoints();
	vtkPointLocator * locator =  vtkPointLocator::New();
	locator->SetDataSet(this->Mesh);
	int tuple[2];

	for(vtkIdType j = 0; j < clippedPoints->GetNumberOfPoints(); j++)
	{
		double * clippedPoint = this->ClippedMesh->GetPoint(j);
		vtkIdType pointId = locator->FindClosestPoint(clippedPoint);
		tuple[0] = pointId;
		tuple[1] = j;
		this->SyncIds->InsertNextTupleValue(tuple);
	}

	locator->Delete();
}

//--------------------------------------------------------------------------
void vtkFeMeshInterface::GenerateTetrahedralMesh()
{

	/*vtkTransform * transform = vtkTransform::New();
	transform->Translate(0,0,-0.1);

	vtkTransformPolyDataFilter * filter = vtkTransformPolyDataFilter::New();
	filter->SetInput(this->clip);
	filter->SetTransform(transform);
	filter->Update();
	vtkPolyData * outputData = filter->GetOutput();

	vtkAppendPolyData * appendFilter = vtkAppendPolyData::New();
	appendFilter->AddInput(this->clip);
	appendFilter->AddInput(outputData);
	appendFilter->Update();
	vtkPolyData * surface = appendFilter->GetOutput();

	vtkDelaunay3D * delaunay = vtkDelaunay3D::New();
	delaunay->SetInput(surface);
	delaunay->Update();
	this->tetraMesh = delaunay->GetOutput();
	this->tetraMesh->BuildLinks();

	//Calculate bounds of the generated mesh
	double bounds[6];
	vtkBox * box = vtkBox::New();

	this->tetraMesh->GetBounds(bounds);
	bounds[0] += 0.01;
	bounds[1] -= 0.01;
	bounds[2] += 0.01;
	bounds[3] -= 0.01;
	bounds[4] -= 0.2;
	bounds[5] += 0.2;
	box->SetBounds(bounds);

	vtkExtractGeometry * ex = vtkExtractGeometry::New();
	ex->SetInput(this->tetraMesh);
	ex->ExtractInsideOff();
	ex->SetImplicitFunction(box);
	ex->Update();

	this->bounds->DeepCopy(ex->GetOutput()->GetPoints());
	std::cout << "[vtkFeMeshInterface] tetraMesh #Bounds: " <<this->bounds->GetNumberOfPoints() << "\n";

	//Calculate Synchronization between meshes
	vtkPoints * clipPoints = this->clip->GetPoints();
	vtkPointLocator * locator =  vtkPointLocator::New();
	locator->SetDataSet(this->tetraMesh);
	int tuple[2];

	//std::cout << "[vtkFeMeshInterface] tetraSyncIds: ";
	for(vtkIdType j = 0; j < clipPoints->GetNumberOfPoints(); j++)
	{
		double * clipPoint = this->clip->GetPoint(j);
		vtkIdType pointId = locator->FindClosestPoint(clipPoint);
		double * tetraPoint = this->tetraMesh->GetPoint(pointId);
		//std::cout <<"cID: " << j << " tId: " << pointId<< "\n";
		for(int i = 0; i<3; i++)
		{
			std::cout << j << " c: " << clipPoint[i]  << " t: " << tetraPoint[i]  << "\n";
		}

		tuple[0] = pointId;
		tuple[1] = j;
		this->tetraSyncIds->InsertNextTupleValue(tuple);
	}

	locator->Delete();*/
}

//--------------------------------------------------------------------------
void vtkFeMeshInterface::RebuildMesh()
{
	///Mesh Reconstruction updating the deformed points
	int ids[2];
	vtkPoints * deformedPoints = this->ClippedMesh->GetPoints();

	//Synchronizing clipped mesh and final mesh
	vtkPoints * points = this->Mesh->GetPoints();

	for(vtkIdType i = 0; i < this->SyncIds->GetNumberOfTuples(); i++)
	{
		this->SyncIds->GetTupleValue(i, ids);
		double * deformedPoint = deformedPoints->GetPoint(ids[1]);

		points->SetPoint(ids[0], deformedPoint);
	}

	this->Mesh->BuildLinks();
	this->Mesh->Update();
}

//--------------------------------------------------------------------------
void vtkFeMeshInterface::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
