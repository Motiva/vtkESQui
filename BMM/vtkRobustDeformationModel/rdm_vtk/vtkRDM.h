#pragma once

#include <vector>

#include "UGridAccelerator.h"
#include "Distance.h"
#include "Surface.h"
#include "Volume.h"
#include "vtkUnstructuredGridAlgorithm.h"

//#define MEASURE_PERFORMANCE

#ifdef MEASURE_PERFORMANCE
	#include "windows.h"
#endif


//BTX
class InitialValueProblemSolver;
//ETX

class VTK_EXPORT vtkRDM : public vtkUnstructuredGridAlgorithm
	{
	public:

		static vtkRDM* New();
        vtkTypeMacro(vtkRDM,vtkUnstructuredGridAlgorithm);

		void SetDistanceForceCoefficient(double kd);
		void SetSurfaceForceCoefficient(double ks);
		void SetVolumeForceCoefficient(double kv);
		void SetDamping(double damping);

		void Init();

		// computes the next timestep
		void Step(double deltaT);

		void evalSpeed(double* result, double* positions, double* speeds);

		void evalAcceleration(double* result, double* positions, double* speeds);

		int RequestData(vtkInformation* request,
		                          vtkInformationVector** inputVector,
		                          vtkInformationVector* outputVector);

		unsigned int GetNumberOfPoints() const;

		double* GetOldPositions() const;

		// returns the current force at a point
		double* GetForce(vtkIdType point) const;

		// returns the array into which you can write
		// forces due to collisions
		// those forces will be used in the next
		// simualation step to compute deformations
		double* GetCollisionForces() const {return this->collisionForces;};


		/*
		 * Special function for cutting
		 * rebuilt all the stuff, slow but should work
		 */
		void Rebuild();


	//BTX
	protected:
		vtkRDM(void);
		~vtkRDM(void);
		void Bootstrap();

	UGridAccelerator* gridAccel;

	private:

		vtkRDM(const vtkRDM&);  // Not implemented.
		void operator=(const vtkRDM&);  // Not implemented.


		double kd;	// stiffness coefficient for the distance preservation force
		double ks;	// stiffness coefficient for the surface area preservation force
		double kv;	// stiffness coefficient for the volume preservation force
		double damping;	// damping coefficient

		double* masses;		// an array of n floats, containing the masses of the n points, data is contained by the whiteboard
		double* speeds;		// an array of n velocities where each velocity occupies three entries in the array, data is contained by the whiteboard
		double* speedsCopy;	// belongs to this class
		double* positions;	// the positions of the uGrid, belongs to the whiteboard
		double* positionsCopy; // belongs to this class

		double* forces;			// array for adding up all resulting forces, data is contained in this class
		double* collisionForces;// stores external forces due to collision


		InitialValueProblemSolver* solver;


		
		// data structure needed to easily access
		// the "objects" needed for constraint force computation	
		std::vector<Distance> distances;
		std::vector<Surface> surfaces;
		std::vector<Volume> volumes;
		

		

		/*!
		 * Initializes internal data structures from the passed grid
		 * \param grid we are extracting the necessary data from the passed grid
		 */
		void initDataStructures(vtkUnstructuredGrid* grid);


		/*!
		 * Adds a force to the already existing force of a point
		 * \param pointId determines the point to which to add the force to
		 * \force the force vector to add
		 */
		void addForce(vtkIdType pointId, double force[3]);


		#ifdef MEASURE_PERFORMANCE
			// performance measurement stuff
			unsigned int iterations;
			LARGE_INTEGER timeSum;
		#endif


		//ETX
	};
