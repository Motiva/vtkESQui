#include "vtkRDM.h"

#include <set>
#include <iostream>
#include "EulerSolver.h"
#include "RK2Solver.h"
#include "RK4Solver.h"
#include "VerletSolver.h"
#include "VelocityVerletSolver.h"

#include "vtkInformation.h"
#include "vtkInformationVector.h"

#include <vtkObjectFactory.h>


#ifdef MEASURE_PERFORMANCE
#include <windows.h>
#endif

vtkStandardNewMacro(vtkRDM);

vtkRDM::vtkRDM(void) {

	// we can not initialize our dynamic data here.
	// Only when we know the vtkUnstrucutredGrid to work on
	// we know all necessary sizes and can construct our arrays

	this->kd = 0.5;
	this->ks = 0;
	this->kv = 0.5;
	this->damping = 0.1;

	this->solver = NULL;
	this->speedsCopy = NULL;
	this->positionsCopy = NULL;
	this->gridAccel = NULL;
}

vtkRDM::~vtkRDM(void) {

	delete this->gridAccel;
	delete[] this->forces;
	delete[] this->collisionForces;
	delete[] this->positionsCopy;
	delete[] this->speedsCopy;

}


void vtkRDM::evalSpeed(double* result, double* positions, double* speeds) {

	// TODO: copying unnecessary, remove it and chekc if ok
	unsigned int numPoints = this->gridAccel->getNumberOfPoints();

	for (unsigned int i = 0; i < numPoints; i++) {
		result[i*3+0] = speeds[i*3+0];
		result[i*3+1] = speeds[i*3+1];
		result[i*3+2] = speeds[i*3+2];
	}
}


void vtkRDM::evalAcceleration(double* result, double* positions, double* speeds) {

	using namespace std;


	// we only have one whiteboard per simulator, so use the first (and only) one 
	vtkUnstructuredGrid *input = vtkUnstructuredGrid::SafeDownCast(this->GetInput());
	vtkIdType numPoints = input->GetNumberOfPoints();

	// zero out the forces array, maybe use something faster than a loop here!
	for (vtkIdType i = 0; i < numPoints*3; i++) this->forces[i] = 0;

	double force[3];	// buffer to write results into
	vtkIdType pointId;	// vtk-id of current point


	// add up all point forces resulting from the distance preservation
	vector<Distance>::iterator iterD;
	for (iterD = this->distances.begin(); iterD != this->distances.end(); iterD++) {

		for (unsigned int i = 0; i < 2; i++) {
			(*iterD).initForceComputation(positions, speeds);
			(*iterD).computeForce(force, positions, speeds, i);
			pointId = (*iterD).getPointId(i);
			this->addForce(pointId, force);
		}
	}

	/*
	// add up all point forces resulting from the surface preservation
	vector<Surface>::iterator iterS;
	for (iterS = this->surfaces.begin(); iterS != this->surfaces.end(); iterS++) {
		for (unsigned int i = 0; i < 3; i++) {
			(*iterS).computeForce(force, positions, i);
			pointId = (*iterS).getPointId(i);
			this->addForce(pointId, force);
			}
		}
	 */

	// add up all point forces resulting from the volume preservation
	vector<Volume>::iterator iterV;
	for (iterV = this->volumes.begin(); iterV != this->volumes.end(); iterV++) {
		for (unsigned int i = 0; i < 4; i++) {
			(*iterV).initForceComputation(positions);
			(*iterV).computeForce(force, positions, i);
			pointId = (*iterV).getPointId(i);
			this->addForce(pointId, force);
		}
	}



	// add up forces resulting from gravity
	/*	for (vtkIdType pointId = 0; pointId < numPoints; pointId++) {
		force[0] = this->masses[pointId] * this->gravity[0];
		force[1] = this->masses[pointId] * this->gravity[1];
		force[2] = this->masses[pointId] * this->gravity[2];
		this->addForce(pointId, force);
		}*/

	// add up forces resulting from collisions
	for (vtkIdType pointId = 0; pointId < numPoints; pointId++) {
		this->addForce(pointId, this->collisionForces + pointId*3);
	}

	// zero out collision forces since we used them already
	// and need them reinitialized for the next step
	for (vtkIdType pointId = 0; pointId < numPoints; pointId++) {
		this->collisionForces[pointId*3 + 0] = 0;
		this->collisionForces[pointId*3 + 1] = 0;
		this->collisionForces[pointId*3 + 2] = 0;
	}


	// compute accelerations from forces
	for (vtkIdType pointId = 0; pointId < numPoints; pointId++) {
		result[pointId*3 + 0] = this->forces[pointId*3 + 0] / this->masses[pointId];
		result[pointId*3 + 1] = this->forces[pointId*3 + 1] / this->masses[pointId];
		result[pointId*3 + 2] = this->forces[pointId*3 + 2] / this->masses[pointId];
	}
}



void vtkRDM::Step(double deltaT) {

	//Compute the next step
	unsigned int numPoints = this->gridAccel->getNumberOfPoints();
	this->solver->ComputeNextStep(this->positions, this->speeds, numPoints, deltaT);
}

// VTK specific method: This method is called when the pipeline is calculated.
int vtkRDM::RequestData(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector) {
	// get the info objects
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);
	// get the input and ouptut
	vtkUnstructuredGrid *input = vtkUnstructuredGrid::SafeDownCast(
			inInfo->Get(vtkDataObject::DATA_OBJECT()));
	vtkUnstructuredGrid *output = vtkUnstructuredGrid::SafeDownCast(
			outInfo->Get(vtkDataObject::DATA_OBJECT()));
	output->DeepCopy(input);
	this->Step(0.001);
	return 1;
}

void vtkRDM::Init()
{
	this->Bootstrap();
}

// -----------------------------------------------
// Working with our data structures
// -----------------------------------------------


void vtkRDM::Bootstrap() {

	vtkUnstructuredGrid *input = vtkUnstructuredGrid::SafeDownCast(this->GetInput());
	this->initDataStructures(input);

}

void vtkRDM::initDataStructures(vtkUnstructuredGrid* grid) {


	// delete all the old stuff
	// => hack for cutting, just reinit everything
	if (this->solver != NULL) delete this->solver;
	if (this->positionsCopy != NULL) delete[] this->positionsCopy;
	if (this->speedsCopy != NULL) delete[] this->speedsCopy;
	if (this->gridAccel != NULL) delete this->gridAccel;

	this->distances.clear();
	this->surfaces.clear();
	this->volumes.clear();



	// usual stuff

	using namespace std;

	this->gridAccel = new UGridAccelerator();
	this->gridAccel->SetGrid(grid);

	// he needs to be able to call getNumberOfPoints() successfully
	// on the vtkRDM
	//this->solver = new EulerSolver(this);
	//this->solver = new RK2Solver(this);
	//this->solver = new RK4Solver(this);
	//this->solver = new VerletSolver(this);
	this->solver = new VelocityVerletSolver(this);

	vtkIdType numPoints = grid->GetNumberOfPoints();
	vtkIdType numCells = grid->GetNumberOfCells();

	// we need at least one tetrahedron to work on...
	if (numPoints < 4 || numCells == 0) {
		cerr << "Error in RDM::initDataStructures(vtkUnstructuredGrid* grid): vtkUnstructuredGrid contains only " << numPoints << " points!" << endl;
	}


	this->forces = new double[numPoints * 3];
	this->collisionForces = new double[numPoints * 3];
	for (int i = 0; i < numPoints * 3; i++) {
		this->forces[i] = 0;
		this->collisionForces[i] = 0;
	}

	this->positionsCopy = new double[numPoints * 3];
	this->speedsCopy = new double[numPoints * 3];

	this->masses = new double[numPoints];
	this->speeds = new double[numPoints * 3];
	this->positions = this->gridAccel->getPointsArray();


	// build the distances data structure

	// the set is used to only keep unique edges
	// all duplicate edges are eliminated
	// this is necessary since we are processing all tetrahedrons
	// each tetrahedrons shares its edges with its neighbours
	set<Distance> setDistances;
	set<Surface> setSurfaces;

	for (vtkIdType cellId = 0; cellId < numCells; cellId++) {

		vtkIdType p0 = this->gridAccel->getCellPointId(cellId, 0);
		vtkIdType p1 = this->gridAccel->getCellPointId(cellId, 1);
		vtkIdType p2 = this->gridAccel->getCellPointId(cellId, 2);
		vtkIdType p3 = this->gridAccel->getCellPointId(cellId, 3);


		// all six edges of a tetrahedron are created
		setDistances.insert(Distance(p0, p1, this->kd, this->damping));
		setDistances.insert(Distance(p0, p2, this->kd, this->damping));
		setDistances.insert(Distance(p0, p3, this->kd, this->damping));
		setDistances.insert(Distance(p1, p2, this->kd, this->damping));
		setDistances.insert(Distance(p1, p3, this->kd, this->damping));
		setDistances.insert(Distance(p2, p3, this->kd, this->damping));


		// all four surfaces of a tetrahedron are created
		setSurfaces.insert(Surface(p0, p1, p2, this->ks));
		setSurfaces.insert(Surface(p0, p1, p3, this->ks));
		setSurfaces.insert(Surface(p0, p2, p3, this->ks));
		setSurfaces.insert(Surface(p1, p2, p3, this->ks));

		// build the volumes data structure
		// since they are already unique we do not need to filter them with a set
		Volume v = Volume(p0, p1, p2, p3, this->kv);
		v.initVolumeOriginal(positions);
		this->volumes.push_back(v);
	}

	// copy the remaining unique edges
	// into the vector for convenient access
	set<Distance>::iterator iterD;
	for (iterD = setDistances.begin(); iterD != setDistances.end(); iterD++) {

		// Note under GCC4.0 iterD->initDistanceOriginal() does not work - why?
		((Distance&)*iterD).initDistanceOriginal(this->positions);
		this->distances.push_back(*iterD);
	}

	// copy the remaining unique surfaces
	// into the vector for convenient access
	set<Surface>::iterator iterS;
	for (iterS = setSurfaces.begin(); iterS != setSurfaces.end(); iterS++) {
		((Surface&)*iterS).initSurfaceOriginal(positions);
		this->surfaces.push_back(*iterS);
	}


	// information output
	//cout << "Number of mass-points: " << numPoints << endl;
	//cout << "Number of tetrahedrons: " << numCells << endl;
	//cout << "Number of distance forces: " << setDistances.size() * 2 << endl;
	//cout << "Number of volume forces: " << numCells * 4 << endl;
}

void vtkRDM::addForce(vtkIdType pointId, double force[3]) {

	double* currForce = this->forces + pointId * 3;
	currForce[0] += force[0];
	currForce[1] += force[1];
	currForce[2] += force[2];
}


// -----------------------------------------------
// Getters and Setters
// -----------------------------------------------

void vtkRDM::SetDistanceForceCoefficient(double kd) {
	this->kd = kd;
}

void vtkRDM::SetSurfaceForceCoefficient(double ks) {
	this->ks = ks;
}

void vtkRDM::SetVolumeForceCoefficient(double kv) {
	this->kv = kv;
}

void vtkRDM::SetDamping(double damping) {
	this->damping = damping;
}

unsigned int vtkRDM::GetNumberOfPoints() const {

	// TODO: implement some saftey measures here
	// class may maybe not yet know its undelying
	// vtkUnstrucutredGrid...
	return this->gridAccel->getNumberOfPoints();
}

double* vtkRDM::GetOldPositions() const {

	//vtkUnstructuredGrid* gridOld = this->whiteboards[0]->GetGeometryDataOld();
	vtkUnstructuredGrid * gridOld = this->gridAccel->getGrid();
	UGridAccelerator accel;
	accel.SetGrid(gridOld);
	return accel.getPointsArray();
}


void vtkRDM::Rebuild() {
	this->Bootstrap();
}

double* vtkRDM::GetForce(vtkIdType point) const {
	return this->forces + point*3;	
}
