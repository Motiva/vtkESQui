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
#ifndef __vtkModel_h
#define __vtkModel_h

#include "vtkESQuiCommonWin32Header.h"
#include "vtkPolyDataAlgorithm.h"

class vtkXMLPolyDataReader;
class vtkPolyData;
class vtkPolyDataMapper;
class vtkTransform;
class vtkMatrix4x4;
class vtkActor;
class vtkSmoothPolyDataFilter;

//!This class represents an abstract model on a scenario element.
/*!
 * It acts as a base class for visualization, collision and deformation models.
 * vtkModel class has 2 inputs. By default the input data will be read from a
 * vtp file specified in FileName attribute.
 * A second optional parameter is available, the source mesh. Main input will
 * be adapted to the source mesh (point positions will be synchronized)
 * The output displays the transformed mesh according to transformation
 * matrix values.
 */

class VTK_ESQUI_COMMON_EXPORT vtkModel : public vtkPolyDataAlgorithm {

public:
	//! Type revision macro
	vtkTypeRevisionMacro(vtkModel,vtkPolyDataAlgorithm);

	//! Create new model that will be used as a data container
	static vtkModel *New();
	//! Print model info
	void PrintSelf(ostream& os, vtkIndent indent);
	//! Return class name
	const char *GetClassName() {return "vtkModel";};

	//!Tool type definitiion
	//BTX
	enum vtkModelType{
		Visualization = 0,
		Collision = 1,
		Deformation = 2
	};

	//!Enumeration of model status
	enum vtkModelStatus
	{
		Enabled = 0,
		Disabled = 1
	};
	//ETX

	//!Set model id
	/*!
	 * \sa vtkIdType GetId()
	 */
	vtkSetMacro(Id, vtkIdType);
	//!Return model id
	/*!
	 * \sa SetInput(vtkIdType)
	 */
	vtkGetMacro(Id, vtkIdType);

	//!Set the object id of the model
	/*!
	 * \sa int GetObjectId()
	 */
	vtkSetMacro(ObjectId, int);
	//!Get the  object id of the model
	/*/
	 * \sa SetObjectId(int)
	 */
	vtkGetMacro(ObjectId, int);

	//!Set model type
	/*!
	 * \sa vtlModelType GetModelType()
	 */
	vtkSetMacro(ModelType, vtkModel::vtkModelType);
	//!Return model type
	/*!
	 * \sa SetModelType(vtkModelType)
	 */
	vtkGetMacro(ModelType, vtkModel::vtkModelType);

	//! Assign the model state
	/*!
	 *\sa GetStatus()
	 */
	vtkSetMacro(Status, vtkModel::vtkModelStatus);

	//! Returns the state of the model
	/*!
	 *\sa SetStatus(vtkModelStatus)
	 */
	vtkGetMacro(Status, vtkModel::vtkModelStatus);

	//! Assign model name
	/*!
	 *\sa GetName()
	 */
	vtkSetStringMacro(Name);

	//!Return model name
	/*!
	 *\sa SetName(const char * name)
	 */
	vtkGetStringMacro(Name);

	//! Assign model filename
	/*!
	 *\sa GetFileName()
	 */
	vtkSetStringMacro(FileName);

	//!Return model filename
	/*!
	 *\sa SetFileName(const char * name)
	 */
	vtkGetStringMacro(FileName);

	//! Set model mesh color
	vtkSetVector3Macro(Color, double);

	//! Get model mesh color
	vtkGetVector3Macro(Color, double);

	//! Set model opacity
	/*!
	 * \sa double GetOpacity()
	 */
	vtkSetMacro(Opacity, double);

	//! Get model opacity
	/*!
	 * \sa SetOpacity(double)
	 */
	vtkGetMacro(Opacity, double);

	//! Set model visibility
	/*!
	 * \sa double GetVisibility()
	 */
	vtkSetMacro(Visibility, bool);

	//! Get model visibilty
	/*!
	 * \sa double GetVisibility()
	 */
	vtkGetMacro(Visibility, bool);

	vtkBooleanMacro(Visibility, bool);

	//!Set transform matrix
	void SetMatrix(vtkMatrix4x4 * m);

	//!Get transform matrix
	/*!
	 * Transformation matrix for displaying purposes
	 */
	//!
	vtkMatrix4x4 * GetMatrix();

	//void SetTransform(vtkTransform * m);
	//vtkTransform * GetTransform();

	//!Get dataset mapper
	/*!
	 * Dataset mapper of the model actor
	 */
	vtkPolyDataMapper * GetMapper();

	//!Get model actor
	/*!
	 * The model actor will be used for displaying purposes
	 */
	vtkActor * GetActor();

	//! Specify the source object for mesh synchronization purposes
	/*!
	 * This is an optional input. If source is set, the model primary mesh
	 * will be synchronized to this source mesh whenever a change is made on it.
	 * \sa vtkPolyData* GetSource()
	 */
	virtual void SetSource(vtkPolyData *source);

	//! Return the source object for mesh synchronization purposes
	/*!
	 * \sa SetSource(vtkPolyData *)
	 */
	virtual vtkPolyData *GetSource();

	//! Get synchronization hash map
	vtkIdList * GetHashMap();

	//!Virtual initialization function
	/*!
	 * The model has to be initialized in order to be updated. At least one parameter must be previously defined: \n
	 * - Input: vtkPolyData object \n
	 */
	virtual void Init();

	//! Hide scenario model.
	/*!
	 * Must be implemented in inherited classes
	 * Note: this function must be implemented in inheriting classes
	 */
	virtual void Hide();
	//! Show/Display model.
	/*!
	 * Must be implemented in inherited classes
	 * Note: this function must be implemented in inheriting classes
	 */
	virtual void Show();

	//! Enable model.
	/*!
	 * Must be implemented in inherited classes
	 * Note: this function must be implemented in inheriting classes
	 */
	virtual void Enable();

	//! Disable model.
	/*!
	 * Must be implemented in inherited classes
	 * Note: this function must be implemented in inheriting classes
	 */
	virtual void Disable();

	//! Check if model is visible
	/*!
	 * When an model is disabled it will:
	 * - be visible.
	 * - be computed in collision detection.
	 * - be used in simulation.
	 */
	bool IsVisible(){ return this->Visibility == 1;};

	//! Check if model is hidden
	/*!
	 * When an model is disabled it will NOT:
	 * - be visible.
	 * - be computed in collision detection.
	 * It will be used in simulation.
	 */
	bool IsHidden(){ return this->Visibility == 0;};

	//! Check if model is disabled.
	/*!
	 * When an model is disabled it will NOT:
	 * - be visible.
	 * - be computed in collision detection.
	 * - be used in simulation.
	 */
	bool IsDisabled(){ return this->Status == Disabled;};

	//!Check if object has been initialized
	bool IsInitialized() { return this->Initialized;};

protected:
	vtkModel();
	~vtkModel();

	virtual int FillOutputPortInformation( int port, vtkInformation* info );
	virtual int FillInputPortInformation(int port, vtkInformation *info);
	virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

	virtual void BuildHashMap(vtkPolyData * a, vtkPolyData * b);

	//! Model Id
	vtkIdType Id;

	//! Object Id
	vtkIdType ObjectId;

	//! Model name
	char * Name;

	vtkModelType ModelType;

	//! Model Status
	vtkModelStatus Status;

	//! model data file name
	char * FileName;

	//! Model data file reader
	vtkXMLPolyDataReader * Reader;

	//! Transform Matrix
	vtkMatrix4x4 * Matrix;

	//vtkTransform * Transform;

	//! Smoothing filter
	vtkSmoothPolyDataFilter * SmoothFilter;

	//! DataSetMapper of the model actor
	vtkPolyDataMapper * Mapper;

	//! Tool piece actor
	vtkActor * Actor;

	//! Synchronization HashMap
	vtkIdList * HashMap;

	//! Model initialization flag
	bool Initialized;

	double Color[3];

	double Opacity;

	bool Visibility;

private:

	vtkModel (const vtkModel &);//NotImplemented
	void operator =(const vtkModel &);//Not Implemented

};
#endif
