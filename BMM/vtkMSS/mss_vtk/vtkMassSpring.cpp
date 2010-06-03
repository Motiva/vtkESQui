#include "vtkMassSpring.h"
#include <math.h>
#include <vtksys/ios/iostream>
#include <vtksys/ios/sstream>
#include <vtksys/stl/stdexcept>
#include <vtksys/stl/set>
#include <cstring>
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkType.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"

vtkCxxRevisionMacro(vtkMassSpring, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkMassSpring);

vtkstd::vector<DistanceMSS> distances;

// Default constructor.
vtkMassSpring::vtkMassSpring() {
    this->ControlIds = vtkIdList::New();
    this->ControlPoints = vtkPoints::New();
    this->points = vtkPoints::New();
    this->writer = vtkUnstructuredGridWriter::New();
    this->DeltaT = 0.001;
    this->Steps = 500;
    this->DisplayFrames = 0;
    this->DistanceCoefficient = 1;
    this->DampingCoefficient = 1;
    this->Mass = 1;
    this->Initialized = 0;
}

// Destructor
vtkMassSpring::~vtkMassSpring() {
    delete[] this->tk;
    delete[] this->tj;
    delete[] this->k1;
    delete[] this->k2;
    delete[] this->k3;
    delete[] this->k4;
    delete[] this->j1;
    delete[] this->j2;
    delete[] this->j3;
    delete[] this->j4;
    delete[] this->forces;
    delete[] this->speedNext;
    this->ControlIds->Delete();
    this->ControlPoints->Delete();
    this->points->Delete();
    this->writer->Delete();
    distances.clear();
}

void vtkMassSpring::InitRK4()
{
	vtkUnstructuredGrid * output = vtkUnstructuredGrid::SafeDownCast(this->GetOutput());
    this->gridAccel = new UGridAcceleratorMSS();
    this->gridAccel->SetGrid(output);
	int numCells = output->GetNumberOfCells();
    distances.clear();
    // posNext and speedNext contain positions and speeds of all points.
    // They'll be changed in RK4ComputeNextStep in every step.
    memset(this->speedNext, 0, numPoints*3*sizeof(double));
    memset(this->forces, 0, numPoints*3*sizeof(double));
    this->posNext = this->gridAccel->getPointsArray();
    vtkstd::set<DistanceMSS> setDistances;
    for (vtkIdType cellId = 0; cellId < numCells; cellId++) {
        vtkIdType p0 = this->gridAccel->getCellPointId(cellId, 0);
        vtkIdType p1 = this->gridAccel->getCellPointId(cellId, 1);
        vtkIdType p2 = this->gridAccel->getCellPointId(cellId, 2);
        vtkIdType p3 = this->gridAccel->getCellPointId(cellId, 3);
        // all six edges of a tetrahedron are created
        setDistances.insert(DistanceMSS(p0, p1, this->DistanceCoefficient, this->DampingCoefficient));
        setDistances.insert(DistanceMSS(p0, p2, this->DistanceCoefficient, this->DampingCoefficient));
        setDistances.insert(DistanceMSS(p0, p3, this->DistanceCoefficient, this->DampingCoefficient));
        setDistances.insert(DistanceMSS(p1, p2, this->DistanceCoefficient, this->DampingCoefficient));
        setDistances.insert(DistanceMSS(p1, p3, this->DistanceCoefficient, this->DampingCoefficient));
        setDistances.insert(DistanceMSS(p2, p3, this->DistanceCoefficient, this->DampingCoefficient));
    }
    // copy the remaining unique edges
    // into the vector for convenient access
    vtkstd::set<DistanceMSS>::iterator iterD;
    for (iterD = setDistances.begin(); iterD != setDistances.end(); iterD++) {
        ((DistanceMSS &)*iterD).initDistanceOriginal(this->posNext);
        distances.push_back(*iterD);
    }
    // reset control points' positions
    for (int i=0; i<this->ControlIds->GetNumberOfIds(); i++) {
        int curId = this->ControlIds->GetId(i);
        double pos[3];
        this->ControlPoints->GetPoint(i, pos);
        posNext[curId*3]        = pos[0];
        posNext[curId*3+1]    = pos[1];
        posNext[curId*3+2]    = pos[2];
    }
}


// Initialize RungeKutta 4
void vtkMassSpring::Init(vtkUnstructuredGrid* output) {
    this->points->DeepCopy(output->GetPoints());
    this->numPoints = output->GetNumberOfPoints();
    int numCells = output->GetNumberOfCells();
    if (numPoints < 4 || numCells == 0) {
        cerr << "Error in RDM::initDataStructures(vtkUnstructuredGrid* grid): vtkUnstructuredGrid contains only " << numPoints << " points!" << endl;
    }
    this->tk = new double[numPoints*3];
    this->tj = new double[numPoints*3];
    this->k1 = new double[numPoints*3];
    this->k2 = new double[numPoints*3];
    this->k3 = new double[numPoints*3];
    this->k4 = new double[numPoints*3];
    this->j1 = new double[numPoints*3];
    this->j2 = new double[numPoints*3];
    this->j3 = new double[numPoints*3];
    this->j4 = new double[numPoints*3];
    this->speedNext = new double[numPoints*3];
    this->forces = new double[numPoints * 3];

    this->Initialized = 1;
}

// VTK specific method: This method is called when the pipeline is calculated.
int vtkMassSpring::RequestData(
    vtkInformation *vtkNotUsed(request),
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector) {
    // get the info objects
    vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
    vtkInformation *outInfo = outputVector->GetInformationObject(0);
    // get the input and ouptut
    vtkUnstructuredGrid *input = vtkUnstructuredGrid::SafeDownCast(
                                     inInfo->Get(vtkDataObject::DATA_OBJECT()));
    vtkUnstructuredGrid *output = vtkUnstructuredGrid::SafeDownCast(
                                      outInfo->Get(vtkDataObject::DATA_OBJECT()));
    output->DeepCopy(input);
    // Init all data structures like speeds, positions
    if(!this->Initialized) this->Init(output);
    this->InitRK4();
    // Using Runge Kutta 4 to iterately calculate displacements
    if (this->DisplayFrames == 0) {
        for (int i=0; i<this->Steps; i++) {
            this->RK4ComputeNextStep(this->posNext, this->speedNext);
        }
    }
    // write intermediate results to text files (vtkUnstructuredGrid)
    else if (this->DisplayFrames >= 1) {
        int frame = 0;
        for (; frame<this->DisplayFrames; frame++) {
            for (int i=0; i<this->Steps / this->DisplayFrames; i++) {
                this->RK4ComputeNextStep(this->posNext, this->speedNext);
            }
            for (int i=0; i<this->numPoints; i++) {
                this->points->SetPoint(i, posNext[3*i], posNext[3*i+1], posNext[3*i+2]);
            }
            vtkUnstructuredGrid* tempGrids = vtkUnstructuredGrid::New();
            tempGrids->DeepCopy(output);
            tempGrids->SetPoints(this->points);
            this->writer->SetInput(tempGrids);
            char fn[10];
            this->filename(frame, fn);
            this->writer->SetFileName(fn);
            this->Modified();
            this->writer->Write();
            tempGrids->Delete();
        }
    } else {
        cerr << "Display Frame should be greater than 0" << endl;
        return -1;
    }
    // For final output, points => output
    for (int i=0; i<this->numPoints; i++) {
        this->points->SetPoint(i, posNext[3*i], posNext[3*i+1], posNext[3*i+2]);
    }
    output->SetPoints(this->points);
    return 1;
}

// Using RK4 to compute next step
void vtkMassSpring::RK4ComputeNextStep(double* posNext,
                                       double* speedNext) {
    // define some optimizations
    double DeltaT05 = 0.5 * this->DeltaT;
    double DeltaT1_6 = this->DeltaT / 6;
    // evaluate derivatives at time t
    this->evalSpeed(this->k1, posNext, speedNext);
    this->evalAcceleration(this->j1, posNext, speedNext);
    for (int i = 0; i < this->numPoints; i++) {
        vtkIdType i3_0 = i*3;
        vtkIdType i3_1 = i*3 + 1;
        vtkIdType i3_2 = i*3 + 2;
        tk[i3_0] = posNext[i3_0] + DeltaT05 * k1[i3_0];
        tk[i3_1] = posNext[i3_1] + DeltaT05 * k1[i3_1];
        tk[i3_2] = posNext[i3_2] + DeltaT05 * k1[i3_2];
        tj[i3_0] = speedNext[i3_0] + DeltaT05 * j1[i3_0];
        tj[i3_1] = speedNext[i3_1] + DeltaT05 * j1[i3_1];
        tj[i3_2] = speedNext[i3_2] + DeltaT05 * j1[i3_2];
    }
    this->evalSpeed(this->k2, tk, tj);
    this->evalAcceleration(this->j2, tk, tj);
    for (int i = 0; i < this->numPoints; i++) {
        vtkIdType i3_0 = i*3;
        vtkIdType i3_1 = i*3 + 1;
        vtkIdType i3_2 = i*3 + 2;
        tk[i3_0] = posNext[i3_0] + DeltaT05 * k2[i3_0];
        tk[i3_1] = posNext[i3_1] + DeltaT05 * k2[i3_1];
        tk[i3_2] = posNext[i3_2] + DeltaT05 * k2[i3_2];
        tj[i3_0] = speedNext[i3_0] + DeltaT05 * j2[i3_0];
        tj[i3_1] = speedNext[i3_1] + DeltaT05 * j2[i3_1];
        tj[i3_2] = speedNext[i3_2] + DeltaT05 * j2[i3_2];
    }
    this->evalSpeed(this->k3, tk, tj);
    this->evalAcceleration(this->j3, tk, tj);
    for (int i = 0; i < numPoints; i++) {
        vtkIdType i3_0 = i*3;
        vtkIdType i3_1 = i*3 + 1;
        vtkIdType i3_2 = i*3 + 2;
        tk[i3_0] = posNext[i3_0] + DeltaT * k3[i3_0];
        tk[i3_1] = posNext[i3_1] + DeltaT * k3[i3_1];
        tk[i3_2] = posNext[i3_2] + DeltaT * k3[i3_2];
        tj[i3_0] = speedNext[i3_0] + DeltaT * j3[i3_0];
        tj[i3_1] = speedNext[i3_1] + DeltaT * j3[i3_1];
        tj[i3_2] = speedNext[i3_2] + DeltaT * j3[i3_2];
    }
    this->evalSpeed(this->k4, tk, tj);
    this->evalAcceleration(this->j4, tk, tj);
    for (int i = 0; i < this->numPoints; i++) {
        // keep control points unchanged
        if (this->ControlIds->IsId(i) != -1) {
            continue;
        }
        vtkIdType i3_0 = i*3;
        vtkIdType i3_1 = i*3 + 1;
        vtkIdType i3_2 = i*3 + 2;
        posNext[i3_0] = posNext[i3_0] + DeltaT1_6 * (k1[i3_0] + 2*k2[i3_0] + 2*k3[i3_0] + k4[i3_0]);
        posNext[i3_1] = posNext[i3_1] + DeltaT1_6 * (k1[i3_1] + 2*k2[i3_1] + 2*k3[i3_1] + k4[i3_1]);
        posNext[i3_2] = posNext[i3_2] + DeltaT1_6 * (k1[i3_2] + 2*k2[i3_2] + 2*k3[i3_2] + k4[i3_2]);
        speedNext[i3_0] = speedNext[i3_0] + DeltaT1_6 * (j1[i3_0] + 2*j2[i3_0] + 2*j3[i3_0] + j4[i3_0]);
        speedNext[i3_1] = speedNext[i3_1] + DeltaT1_6 * (j1[i3_1] + 2*j2[i3_1] + 2*j3[i3_1] + j4[i3_1]);
        speedNext[i3_2] = speedNext[i3_2] + DeltaT1_6 * (j1[i3_2] + 2*j2[i3_2] + 2*j3[i3_2] + j4[i3_2]);
    }
}

void vtkMassSpring::evalSpeed(double* result, double* positions, double* speeds) {
    memcpy(result, speeds, numPoints*3*sizeof(double));
}

void vtkMassSpring::evalAcceleration(double* result, double* positions, double* speeds) {
    memset(this->forces, 0, numPoints*3*sizeof(double));    // zero out the forces array
    double force[3];    // buffer to write results into
    vtkIdType pointId;    // vtk-id of current point
    // add up all point forces resulting from the distance preservation
    vector<DistanceMSS>::iterator iterD;
    for (iterD = distances.begin(); iterD != distances.end(); iterD++) {
        for (unsigned int i = 0; i < 2; i++) {
            (*iterD).initForceComputation(positions, speeds);
            (*iterD).computeForce(force, positions, speeds, i);
            pointId = (*iterD).getPointId(i);
            this->addForce(pointId, force);
        }
    }
    // compute accelerations from forces
    for (vtkIdType pointId = 0; pointId < numPoints; pointId++) {
        result[pointId*3 + 0] = this->forces[pointId*3 + 0] / this->Mass;
        result[pointId*3 + 1] = this->forces[pointId*3 + 1] / this->Mass;
        result[pointId*3 + 2] = this->forces[pointId*3 + 2] / this->Mass;
    }
}

void vtkMassSpring::addForce(vtkIdType pointId, double force[3]) {
    double* currForce = this->forces + pointId * 3;
    currForce[0] += force[0];
    currForce[1] += force[1];
    currForce[2] += force[2];
}

void vtkMassSpring::filename(int i, char* c) {
    sprintf(c, "%d", i);
}

// VTK specific method:
//      This method prints the contents of this object. The method is needed
//      to conform to the VTK tools.
void vtkMassSpring::PrintSelf(ostream& os, vtkIndent indent) {
    vtkUnstructuredGridAlgorithm::PrintSelf(os,indent);
}

