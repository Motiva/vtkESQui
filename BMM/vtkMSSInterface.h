#ifndef __vtkMSSInterface_h
#define __vtkMSSInterface_h

#include "vtkESQuiBMMWin32Header.h"

#include "vtkTimerLog.h"

#include "vtkBioMechanicalModel.h"
#include "vtkMSS.h"


//! Implementation of the generic Biomechanical Model interface for a mass-spring deformation system

class VTK_ESQUI_BMM_EXPORT vtkMSSInterface : public vtkBioMechanicalModel
{
public:
	vtkTypeRevisionMacro(vtkMSSInterface, vtkBioMechanicalModel);
	static vtkMSSInterface* New();
	const char *GetClassName() {return "vtkMSSInterface";};
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
	
	//! Set the MSS Distance coefficient
	void SetDistanceCoefficient(double value);
	//! Set the MSS Damping coefficient
	void SetDampingCoefficient(double value);//Friction
	//! Set the MSS Mass value
	void SetMass(double value);
	//! Set the MSS time step
	void SetDeltaT(double value);
	//! Set the MSS number of steps
	void SetSteps(int value);

protected:
  vtkMSSInterface();
  ~vtkMSSInterface();

private:
  vtkMSSInterface(const vtkMSSInterface&);  // Not implemented.
  void operator=(const vtkMSSInterface&);  // Not implemented.

  //!Mass-spring system mesh
  vtkMSS * MSSMesh;

  // MSS specific parameters
  //! Distance coefficient.
  double DistanceCoefficient;
  //! Damping coefficient.
  /*!
   * < 1 Under-damped. The system oscillates (with a slightly different frequency than the undamped case) with the amplitude gradually decreasing to zero
   * = 1 Critically Damped. The system returns to equilibrium as quickly as possible without oscillating
   * > 1 Over-Damped. The system returns (exponentially decays) to equilibrium without oscillating
   */
  double DampingCoefficient;
  //! Mass value on each point
  double Mass;
  //! calculation time step
  double DeltaT;
  //! Number of steps
  int Steps;

};

#endif
