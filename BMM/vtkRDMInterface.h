#ifndef __vtkRDMInterface_h
#define __vtkRDMInterface_h

#include "vtkESQuiBMMWin32Header.h"

#include "vtkTimerLog.h"

#include "vtkBioMechanicalModel.h"
#include "vtkRobustDeformationModel.h"


//! Implementation of the generic Biomechanical Model interface for a rdm system

class VTK_ESQUI_BMM_EXPORT vtkRDMInterface : public vtkBioMechanicalModel
{
public:
	vtkTypeRevisionMacro(vtkRDMInterface, vtkBioMechanicalModel);
	static vtkRDMInterface* New();
	const char *GetClassName() {return "vtkRDMInterface";};
	//! Print class object values
	void PrintSelf(ostream& os, vtkIndent indent);
	
	//!Set input data (organ mesh) to the mass-spring deformation system
	/*!
	 * A vtkUnstructuredGrid is required due to 3D deformation
	 * \param data vtkUnstructured 3d organ mesh data
	 */
	//virtual void SetInput(vtkUnstructuredGrid * data);

	//!Perform an update of the mass-spring system
	/*!
	 * Once collision detection process has been performed, if any contact between a tool and organ has occured the organ must be updated, so mass-spring system.
	 */
	//virtual void Update();

	virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

	//!Initialize the Biomechanical Model
	virtual void Init();

	//! Return updated organ mesh (data) after updating the biomechanical model
	//virtual vtkUnstructuredGrid * GetOutput();
	
	//! Set the RDM Distance coefficient
	void SetDistanceCoefficient(double value);
	//! Set the RDM Damping coefficient
	void SetDampingCoefficient(double value);//Friction
	//! Set the RDM Surface coefficient
	void SetSurfaceCoefficient(double value);
	//! Set the RDM Volume coefficient
	void SetVolumeCoefficient(double value);
	//! Set the RDM Mass value
	void SetMass(double value);
	//! Set the RDM time step
	void SetDeltaT(double value);
	//! Set the RDM number of steps
	void SetSteps(int value);

protected:
  vtkRDMInterface();
  ~vtkRDMInterface();

private:
  vtkRDMInterface(const vtkRDMInterface&);  // Not implemented.
  void operator=(const vtkRDMInterface&);  // Not implemented.

  //!Mass-spring system mesh
  vtkRobustDeformationModel * RDMMesh;

  // RDM specific parameters
  //! Distance coefficient.
  double DistanceCoefficient;

  //! Damping coefficient.
  /*!
   * < 1 Under-damped. The system oscillates (with a slightly different frequency than the undamped case) with the amplitude gradually decreasing to zero
   * = 1 Critically Damped. The system returns to equilibrium as quickly as possible without oscillating
   * > 1 Over-Damped. The system returns (exponentially decays) to equilibrium without oscillating
   */
  double DampingCoefficient;

  //! Surface Coefficient
  double SurfaceCoefficient;

  //! Surface Coefficient
  double VolumeCoefficient;

  //! Mass value on each point
  double Mass;

  //! calculation time step
  double DeltaT;

  //! Number of steps
  int Steps;

};

#endif
