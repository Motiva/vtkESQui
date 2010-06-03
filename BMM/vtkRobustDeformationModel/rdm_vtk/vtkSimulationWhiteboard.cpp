/*=========================================================================
==========================================================================
=========================================================================*/
#include "vtkSimulationWhiteboard.h"
#include <vtkObjectFactory.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkUnstructuredGrid.h>
#include <vtkCommand.h>
#include <vtkCellArray.h>
#include <vtkTransform.h>

const unsigned long vtkSimulationWhiteboard::UpstreamChangedEvent =
        vtkCommand::UserEvent + 1000;
const unsigned long vtkSimulationWhiteboard::PointsProtectedEvent =
        vtkCommand::UserEvent + 1001;
const unsigned long vtkSimulationWhiteboard::PointsUnProtectedEvent =
        vtkCommand::UserEvent + 1002;
const unsigned long vtkSimulationWhiteboard::PositionsModifiedEvent =
        vtkCommand::UserEvent + 1003;
const unsigned long vtkSimulationWhiteboard::CollisionDetectedEvent =
        vtkCommand::UserEvent + 1004;
const unsigned long vtkSimulationWhiteboard::TetraSubdividedEvent =
        vtkCommand::UserEvent + 1005;
const unsigned long vtkSimulationWhiteboard::PointLocationInVolumeChangedEvent =
        vtkCommand::UserEvent + 1006;
const unsigned long vtkSimulationWhiteboard::ActorModifiedEvent =
        vtkCommand::UserEvent + 1007;

static const char* eventNames[] = {
    "UpstreamChangedEvent",
    "PointsProtectedEvent",
    "PointsUnProtectedEvent",
    "PositionsModifiedEvent",
    "CollisionDetectedEvent",
    "TetraSubdividedEvent",
    "PointLocationInVolumeChangedEvent",
    "ActorModifiedEvent"
};
using namespace std;

vtkStandardNewMacro(vtkSimulationWhiteboard);
//----------------------------------------------------------------------------
vtkSimulationWhiteboard::vtkSimulationWhiteboard() {
    lastInputCopyTime = 0;
    geometricData = vtkUnstructuredGrid::New();
    updating = false;
    eventParameters.push(NULL);
    eventName.push("");
    protectedPoints = vtkIdList::New();
    propagateDownstream = true;
    
    pointSpeeds = NULL;
    pointMasses = NULL;
	this->mass = 1;
    
	this->collisionData = NULL;
	this->geometricDataOld = vtkUnstructuredGrid::New();
	//this->geometricDataOld->Initialize();
	this->geometricDataTemp = vtkUnstructuredGrid::New();
	//this->geometricDataTemp->Initialize();
	this->saveOldGeometryData = true;
}
//----------------------------------------------------------------------------
vtkSimulationWhiteboard::~vtkSimulationWhiteboard() 
{
    geometricData->Delete();
    protectedPoints->Delete();
}
//----------------------------------------------------------------------------
void vtkSimulationWhiteboard::PrintSelf(ostream& os, vtkIndent indent)
{
    vtkUnstructuredGridAlgorithm::PrintSelf(os,indent);
}
//----------------------------------------------------------------------------
// make sure that the output data structure is initialized!
int vtkSimulationWhiteboard::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
    updating = true;
    // get the info objects
    vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
    vtkInformation *outInfo = outputVector->GetInformationObject(0);
    
    // get the input and ouptut
    vtkUnstructuredGrid *input = vtkUnstructuredGrid::SafeDownCast(
        inInfo->Get(vtkDataObject::DATA_OBJECT()));
    vtkUnstructuredGrid *output = vtkUnstructuredGrid::SafeDownCast(
        outInfo->Get(vtkDataObject::DATA_OBJECT()));
    
    unsigned long inputModificationTime = input->GetMTime();
    
    if (inputModificationTime > lastInputCopyTime) {
        // remember that we copied the data
        lastInputCopyTime = inputModificationTime;
        
        // copy the input to the output
        geometricData->DeepCopy(input);
	//========allocating space for additional points and cells for cutting===========
        //copy the cells and allocating additional cells
        vtkCellArray* inputArray= input->GetCells();
        vtkCellArray* dataArray= geometricData->GetCells();
        
        dataArray->Reset();
        dataArray->Allocate(inputArray->GetNumberOfCells()+10000, inputArray->GetNumberOfCells()+10000);
        inputArray->InitTraversal();
		
        for(int i=0; i<inputArray->GetNumberOfCells(); i++)
        {
          vtkIdType* ids;
          vtkIdType id;
         
          inputArray->GetNextCell(id, ids);
			
		 // inputArray->GetCell(i, id, ids);
          // cout << "Cell: "<< inputArray->GetNextCell(id, ids) << endl;
			
          dataArray->InsertNextCell(id, ids);
	  //cout <<id << ", "<< ids[0]<<", "<< ids[1]<<", "<< ids[2]<<", "<< ids[3]<<endl;
        }
        
        //copy points and allocate additional points
        vtkPoints* inputPnts= input->GetPoints();
        vtkPoints* dataPnts= geometricData->GetPoints();
        dataPnts->Reset();
        dataPnts->Allocate(inputPnts->GetNumberOfPoints()+10000, inputPnts->GetNumberOfPoints()+10000);
        
        for(int j=0; j<inputPnts->GetNumberOfPoints(); j++)
        {
          double pnt[3]= {-99999999, -99999999, -99999999};
          
          inputPnts->GetPoint(j, pnt);
          dataPnts->InsertNextPoint(pnt);
	}
        //================================================================
	
        
        // convert the point data type if needed
        if (geometricData->GetPoints()->GetDataType() != VTK_DOUBLE) {
            geometricData->GetPoints()->SetDataTypeToDouble();
            geometricData->GetPoints()->DeepCopy(input->GetPoints());
        }
		// already provide the "old" data from the beginning on
		// in the first step of a simulation:
		// geometricDataOld = geometricData
		this->geometricDataOld->DeepCopy(this->geometricData);
        
        if (pointSpeeds != NULL)
            delete[] pointSpeeds;
        if (pointMasses != NULL)
            delete[] pointMasses;
        
        pointSpeeds = new double[3 * geometricData->GetNumberOfPoints()];
        pointMasses = new double[geometricData->GetNumberOfPoints()];
        double massPerPoint = this->mass / geometricData->GetNumberOfPoints();
        
        for (int i = 0; i < geometricData->GetNumberOfPoints(); i++) {
            pointSpeeds[i * 3 + 0] = 0;
            pointSpeeds[i * 3 + 1] = 0;
            pointSpeeds[i * 3 + 2] = 0;
            pointMasses[i] = massPerPoint;
        }
        
        // notify all observers that we have a new grid
        InvokeWhiteboardEvent(UpstreamChangedEvent);
    }
    output->ShallowCopy(geometricData);
    
    updating = false;
    return 1;
}

//----------------------------------------------------------------------------
vtkUnstructuredGrid* vtkSimulationWhiteboard::GetGeometryData()
{
    if (geometricData == NULL || !updating)
        GetOutput()->Update();
    return geometricData;
}

vtkUnstructuredGrid *vtkSimulationWhiteboard::GetGeometryDataOld() {
    if (geometricDataOld == NULL || !updating)
        GetOutput()->Update();
    return geometricDataOld;
	}

//----------------------------------------------------------------------------
int vtkSimulationWhiteboard::FillInputPortInformation(int, vtkInformation *info)
{
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkUnstructuredGrid");
    return 1;
}

//----------------------------------------------------------------------------
void vtkSimulationWhiteboard::InvokeWhiteboardEvent(
    unsigned long event,
    vtkObject *parameter)
{
    eventParameters.push(parameter);
    eventName.push(eventNames[event - vtkCommand::UserEvent - 1000]);
    InvokeEvent(event);
    InvokeEvent("UserEvent");
    eventName.pop();
    eventParameters.pop();
    
    // propagate the event to vtk?
    if (propagateDownstream) {
        if (event == PositionsModifiedEvent ||
            event == TetraSubdividedEvent ||
            event == PointLocationInVolumeChangedEvent
            )
            Modified();
    }

	// hack to save old geometry data
	/*
	if (event == vtkSimulationWhiteboard::PositionsModifiedEvent) {
		if (this->saveOldGeometryData) {
			this->geometricDataOld->DeepCopy(this->geometricData);
			}
		}
		*/
}
void vtkSimulationWhiteboard::SetCollisionData(vtkCollisionEventData* collisionData) {
	this->collisionData = collisionData;
	
	//this->collisionData->Register(NULL);
	}
vtkCollisionEventData* vtkSimulationWhiteboard::GetCollisionData() {
	return this->collisionData;
	}
void vtkSimulationWhiteboard::SetSaveGeometryData(bool save) {
	this->saveOldGeometryData = save;
	}

//----------------------------------------------------------------------------
void vtkSimulationWhiteboard::AddPointProtection(vtkIdList *list)
{
    // TODO: save in pointProtection
    InvokeWhiteboardEvent(PointsProtectedEvent, list);
}

//----------------------------------------------------------------------------
void vtkSimulationWhiteboard::RemovePointProtection(vtkIdList *list)
{
    // TODO: save in pointProtection
    InvokeWhiteboardEvent(PointsUnProtectedEvent, list);
}

void vtkSimulationWhiteboard::EnableSaveOldData() {
	this->saveOldGeometryData = true;
	}

void vtkSimulationWhiteboard::PreSimulation() {
	this->geometricDataTemp->DeepCopy(this->geometricData);
	}

void vtkSimulationWhiteboard::PostSimulation() {
	this->geometricDataOld->Reset();
	//this->geometricDataOld->Initialize();
	vtkUnstructuredGrid* tmp = this->geometricDataOld;
	this->geometricDataOld = this->geometricDataTemp;
	this->geometricDataTemp = tmp;
	}

void vtkSimulationWhiteboard::DisableSaveOldData() {
	this->saveOldGeometryData = false;
	}
void vtkSimulationWhiteboard::SetMass(double mass) {
	this->mass = mass;
	}

//----------------------------------------------------------------------------
void vtkSimulationWhiteboard::ApplyTransformatino(vtkTransform* transform)
{
    
    
}
//----------------------------------------------------------------------------
double* vtkSimulationWhiteboard::GetTetraPositionDelta(int tetraId)
{
    static double delta[3];
    
    delta[0] = 0;
    delta[1] = 0;
    delta[2] = 0;
    
    if (!saveOldGeometryData)
        return delta;
    
    vtkIdList* list = vtkIdList::New();
    geometricData->GetCellPoints(tetraId, list);
    
    vtkPoints* new_points = geometricData->GetPoints();
    vtkPoints* old_points = geometricDataOld->GetPoints();
    
    vtkIdType id;
    for (int i = 0; i < list->GetNumberOfIds(); i++) {
        id = list->GetId(i);
        delta[0] += new_points->GetPoint(id)[0] - old_points->GetPoint(id)[0];
        delta[1] += new_points->GetPoint(id)[1] - old_points->GetPoint(id)[1];
        delta[2] += new_points->GetPoint(id)[2] - old_points->GetPoint(id)[2];
    }
    delta[0] /= 4.0;
    delta[1] /= 4.0;
    delta[2] /= 4.0;
    
    list->Delete();
    
    return delta;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
