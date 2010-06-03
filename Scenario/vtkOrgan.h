#ifndef _vtkOrgan_h_
#define _vtkOrgan_h_

#include "vtkESQuiScenarioWin32Header.h"

#include "vtkObject.h"
#include "vtkObjectFactory.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRendererCollection.h"
#include "vtkDataSetMapper.h"
#include "vtkTextureMapToSphere.h"
#include "vtkTransformTextureCoords.h"
#include "vtkXMLUnstructuredGridReader.h"
#include "vtkDelaunay2D.h"
#include "vtkBioMechanicalModel.h"
#include "vtkMSSInterface.h"
#include "vtkContact.h"
#include "vtkContactCollection.h"
#include "vtkScenarioItem.h"
#include "vtkPointPlotter.h"
#include "vtkJPEGReader.h"
#include "vtkTransform.h"
#include "vtkTransformFilter.h"

#include "vtkTimerLog.h"

//! Implementation class for organ definition
class VTK_ESQUI_SCENARIO_EXPORT vtkOrgan: public vtkScenarioItem
{
public:

	vtkTypeRevisionMacro(vtkOrgan, vtkScenarioItem);
	static vtkOrgan * New();
	const char *GetClassName() {return "vtkOrgan";}

	virtual void PrintSelf(ostream &os, vtkIndent indent);

	//!Set input mesh dataset
	/*!
	* \param data vtkUnstructuredGrid object containing mesh data (points, cells...)
	* \sa GetOutput()
	*/
	void SetInput(vtkUnstructuredGrid * data);

	//!Update the object
	void Update();

	//!Regenerate simplified mesh after changes due to transformations/deformations
	void UpdateSimpleMesh();

	//!Return output data as an vtkUnstructuredGrid object
	/*!
	* \sa SetInput(vtkUnstructuredGrid *data)
	*/
	vtkUnstructuredGrid * GetOutput();

	//!Set the Biomechanical Model of the organ
	/*!
	* \param biomechanical model of the organ
	*/
	void SetBioMechanicalModel(vtkBioMechanicalModel * model);


	//!Get the Biomechanical Model of the organ
	/*!
	* \return biomechanical model of the organ
	* \sa SetBioMechModel(vtkBioMechanicalModel * model)
	*/
	vtkBioMechanicalModel * GetBioMechanicalModel();

	//!Set organ name
	/*!
	* \param name string containing organ's name
	* \sa GetName()
	*/
	void SetName(const char * name) {this->Name = name;};

	//!Return the organ name
	/*!
	* \sa SetName(const char * name)
	*/
	const char * GetName() {return this->Name;};

	//!Set organ identifier
	/*!
	* \param id identifying tag of the organ
	* \sa GetId()
	*/
	void SetId(vtkIdType id) {this->Id = id;};

	//!Return the organ identifier
	/*!
	* \sa SetId(vtkIdType id)
	*/
	vtkIdType GetId() {return this->Id;};

	//!Set initial values filename
	/*!
	* \param name path of file that contains organ initialization data
	* \sa GetFileName()
	*/
	void SetFileName(const char* name);

	//!Return initial values filename
	/*!
	* \sa SetFileName(const char* name);
	*/
	const char * GetFileName();

	//!Set texture filename
	/*!
	* \param name path of file that contains organ initialization data
	* \sa GetTextureFileName()
	*/
	void SetTextureFileName(const char* name);

	//!Return texture filename
	/*!
	* \sa SetTextureFileName(const char* name);
	*/
	const char * GetTextureFileName();

	//!Initialize mesh data
	/*!
	* The following parameters should be set before calling this function:
	* Biomechanical Model
	*/
	void Init();

	//!Set gravity parameters,
	/*!
	* where the direction must be 0, 1, or 2 according to x, y, and z direction, and the orientation must have a value of +1 or -1
	*/
	void SetGravityInfo(vtkIdType direction, vtkIdType orientation)
	{
		this->GravityDirection = direction;
		this->GravityOrientation = orientation;
	}

	//!Set maximum cutting distance
	void SetMaxCutDistance( const double distance )  { this->MaxCuttingDistance = distance; }

	//!Return if the organ could be clipped
	/*!
	* \sa SetCanBeClippedOn()
	* \sa SetCanBeClippedOff()
	*/
	vtkIdType GetCanBeClipped() const { return CanBeClipped; }

	//!Disable organ clipping
	/*!
	* \sa SetCanBeClippedOn()
	*/
	void SetCanBeClippedOff() {CanBeClipped = 0;}
	//!Enable organ clipping
	/*!
	* \sa SetCanBeClippedOff()
	*/
	void SetCanBeClippedOn() {CanBeClipped = 1;}

	//!Return whether the organ contains fluids or not
	/*!
	* \sa SetContainsFluidOff()
	* \sa SetContainsFluidOn()
	*/
	vtkIdType ContainsFluid() const { return containsFluid; }

	//!Disable fluid property
	/*!
	* \sa SetContainsFluidOn()
	*/
	void SetContainsFluidOff() {containsFluid = 0;}

	//!Enable fluid property
	/*!
	* \sa SetContainsFluidOff()
	*/
	void SetContainsFluidOn()	{containsFluid = 1;}

	//!Set force factor.
	/*!
	* Used for estimating haptic force process
	* \sa GetForceFactor()
	*/
	void SetForceFactor(float value){forceFactor = value;}

	//!GetfForce factor.
	/*!
	* \sa SetForceFactor(float value)
	*/
	float GetForceFactor() const{ return forceFactor; }

	//!Return if the organ is hooked
	/*!
	* \sa SetHookedOn()
	* \sa SetHookedOff()
	*/
	vtkIdType IsHooked(){ return this->Hooked;};

	//!Set organ as hooked
	/*!
	* \sa SetHookedOff()
	*/
	void SetHookedOn() {this->Hooked = 1;}

	//!Set organ as not hooked
	/*!
	* \sa SetHookedOn()
	*/
	void SetHookedOff() {this->Hooked = 0;}

	//!Set number of required biomechanical model process iterations.
	/*!
	* This parameter will define the realism of the deformation
	* \param iterations number of iterations
	* \sa GetNumberOfIterations()
	*/
	void SetNumberOfIterations(vtkIdType iterations) {this->numberOfIterations = iterations;};

	//!Return number of required biomechanical model process iterations.
	/*!
	* This parameter will define the realism of the deformation
	* \sa SetNumberOfIterations(vtkIdType iterations)
	*/
	vtkIdType GetNumberOfIterations(){ return this->numberOfIterations;};

	//!Return the number of mesh elements
	vtkIdType GetNumberOfElements()  { return this->NumberOfElements; }

	//!Return a list containing the moved elements on last iteration.
	vtkPoints * GetContactPoints();

	//!Return whether the organ geometry has been changed
	vtkIdType IsDeformed()  { return this->Deformed; }

	//------- Biomechanical model interface methods -------//

	//!Add a contact to the list
	/*!
	* \param contact vtkContact object with the information of the contact( toolId, organId, point, cell... etc)
	* \sa InsertContacts( vtkContactCollection* contacts )
	*/
	void InsertNextContact( vtkContact* contact )  { this->Contacts->InsertNextContact(contact); };

	//!Add several contacts to the list
	/*!
	* \param contact list of vtkContact objects
	* \sa InsertNextContact( vtkContact* contact )
	*/
	void InsertContacts( vtkContactCollection* contacts )  { this->Contacts->DeepCopy(contacts); };

	//!Remove all contacts
	/*!
	* Remove all contacts from the list. Memory is not freed
	*/
	void RemoveContacts() {this->Contacts->RemoveContacts();};

	//!Get organ contacts
	vtkContactCollection * GetContacts() {return this->Contacts;};

	//!Get number of contacts
	vtkIdType GetNumberOfContacts() {return this->Contacts->GetNumberOfItems();};

	//!Return a specific cell of the organ mesh
	/*!
	* \param id identifying key of the mesh cell
	*/
	vtkCell * GetCell(vtkIdType id){ return this->Bmm->GetMeshCell(id);};

	//!Cauterize organ at specified element
	//BTX
	void Cauterize( vtkIdType element );
	//ETX

	//BTX
	//!Cut the organ at specified elements
	void Cut(vtkIdList *ids);
	//ETX

	//!Set the render window for graphical purposes
	/*!
	* \param Renderer render window
	* \sa GetRenderWindow()
	*/
	void SetRenderWindow(vtkRenderWindow *Renderer);

	//!Return the render window of the organ
	/*!
	* \sa SetRenderWindow(vtkRenderWindow *Renderer)
	*/
	vtkRenderWindow* GetRenderWindow();

	//!Set the actor of the organ
	/*!
	* \param Actor vtkActor object
	* \sa GetActor()
	*/
	void SetActor(vtkActor *Actor);

	//!Return the actor that displays the organ
	/*!
	* \sa SetActor(vtkActor *Actor)
	*/
	vtkActor* GetActor();

	//!Set the mapper of the actor
	/*!
	* \param dataset mapper
	* \sa GetMapper()
	*/
	void SetMapper(vtkDataSetMapper *Mapper);

	//!Return the data set mapper
	/*!
	* \sa SetMapper(vtkDataSetMapper *Mapper)
	*/
	vtkDataSetMapper* GetMapper();

	//*****   p r o t e c t e d   m e m b e r s
protected:

	//!Configuration filename
	const char * FileName;

	//!Configuration filename
	const char * TextureFileName;

	//!Texture
	vtkTexture * Texture;

	//!Input Unstructured Grid
	vtkUnstructuredGrid * Input;

	//!Unstructured grid reader
	vtkXMLUnstructuredGridReader * Reader;

	//!BioMechamical Model of the organ
	vtkBioMechanicalModel * Bmm;

	//!Collection of organ contact points
	vtkContactCollection * Contacts;

	//!Number Of Elements/Nodes of the organ
	vtkIdType NumberOfElements;

	//!Number of iterations required to obtain a realistic deformation
	vtkIdType numberOfIterations;

	//!Gravity Direction (0/1/2 for x/y/z)
	vtkIdType GravityDirection;
	//!Gravity Orientation (+/- 1)
	vtkIdType GravityOrientation;

	//!Object has been deformed on last iteration
	vtkIdType Deformed;

	//Maximum cutting distance
	double MaxCuttingDistance;

	//!For report building purposes
	vtkIdType CanBeClipped;   
	vtkIdType containsFluid;

	//!Force estimation
	double forceFactor;     

	//!Organ is hooked
	vtkIdType Hooked;

	//**** Graphical Purposes objects ****//

	//!Render Window of the organ
	vtkRenderWindow *RenderWindow;
	//!Renderer of the organ
	vtkRenderer * Renderer;

	//!Actor of the organ
	vtkActor * Actor;

	//!Dataset mapper of the actor
	vtkDataSetMapper * Mapper;

	//!Transform filter of the organ
	vtkTransformFilter *TransformFilter;

	//!Transform function of the organ
	vtkTransform * Transform;

	//!Bounding Box Mapper
	vtkDataSetMapper * SimpleMeshMapper;

	//!Bounding Box Actor
	vtkActor * SimpleMeshActor;

	//! Default Constructor
	vtkOrgan();
	//! Destructor
	~vtkOrgan();

private:

	vtkOrgan(const vtkOrgan &);
	void operator=(const vtkOrgan &);

};

#endif

