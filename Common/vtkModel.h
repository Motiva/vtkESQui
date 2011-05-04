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
class vtkActor;
class vtkSmoothPolyDataFilter;

//!This class represents an abstract model.
/*!
 * It has 2 inputs. By default the input data will be read from a vtp file specified in FileName.
 * There is second optional parameter, source.
 * 1 output.
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

	//!Tool type definition
	enum vtkModelType{
		Visualization = 0,
		Collision = 1,
		Deformation = 2
	};

	//!Enumeration of model status
	enum vtkModelStatus
	{
		Active = 0,
		Visible = 1,
		Hidden = 2,
		Disabled = 3
	};

	//!Set model id
	vtkSetMacro(Id, vtkIdType);
	//!Return model id
	vtkGetMacro(Id, vtkIdType);

	//!Set the object id of the model
	vtkSetMacro(ObjectId, int);
	//!Get the  object id of the model
	vtkGetMacro(ObjectId, int);

	//!Set model type
	vtkSetMacro(ModelType, vtkModel::vtkModelType);
	//!Return model type
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

	//! Assign model scale
	/*!
	 *\sa GetScale()
	 */
	vtkSetVector3Macro(Scale, double);

	//!Return model scale
	/*!
	 *\sa SetScale(double)
	 */
	vtkGetVector3Macro(Scale, double);

	// **** Graphical Purposes Methods **** //
	//! Set the model origin Point.
	/*!
	 * Reference point where the rotation calculus are made
	 */
	vtkSetVector3Macro(Origin, double);

	//! Get the model origin Point.
	/*!
	 * Reference point where the rotation calculus are made
	 */
	vtkGetVector3Macro(Origin, double);

	//! Set model orientation angles.
	/*!
	 * (yaw, pitch, roll) (WXYZ)
	 */
	vtkSetVector3Macro(Orientation, double);

	//! Get model orientation angles. (yaw, pitch, roll) (WXYZ)
	/*!
	 * (yaw, pitch, roll) (WXYZ)
	 */
	vtkGetVector3Macro(Orientation, double);

	//!Set the model position (WXYZ)
	vtkSetVector3Macro(Position, double);
	//!Get the model position (WXYZ)
	vtkGetVector3Macro(Position, double);

	//! Set model mesh color
	vtkSetVector3Macro(Color, double);

	//! Get model mesh color
	vtkGetVector3Macro(Color, double);

	//! Set model opacity
	vtkSetMacro(Opacity, double);

	//! Get model opacity
	vtkGetMacro(Opacity, double);

	//! Set model visibility
	vtkSetMacro(Visibility, bool);

	//! Get model visibilty
	vtkGetMacro(Visibility, bool);

	vtkBooleanMacro(Visibility, bool);

	//!Get transform function
	/*!
	 * Transform function for displaying purposes
	 */
	vtkTransform * GetTransform();

	//!Get dataSet mapper
	/*!
	 * Dataset mapper of the piece actor
	 */
	vtkPolyDataMapper * GetMapper();

	//!Get model actor
	/*!
	 * The model actor will be used for displaying purposes
	 */
	vtkActor * GetActor();

	//! Specify the source object for mesh synchronization purposes
	virtual void SetSource(vtkPolyData *source);

	//! Return the source object for mesh synchronization purposes
	virtual vtkPolyData *GetSource();

	//! Get synchronization hash map
	vtkIdList * GetHashMap();

	void Init();

	virtual void Translate(double x, double y, double z);

	//! Implements the translation of the model (Local coordinate system)
	/*!
		\param vector position vector of the translation
	 */
	virtual void Translate(double * vector);

	//! Implements the lateral movements of the model  (Global coordinate system)
	/*!
		The X parameter contains the relative movement in the horizontal axes
		\param a a orientation angle
		\param x x component
		\param y y component
		\param z z component
		Note: this function must be implemented in inheriting classes
	 */
	virtual void RotateWXYZ(double a, double x, double y, double z);

	//! Implements the lateral movements of the model  (Local coordinate system)
	/*!
			The X parameter contains the relative movement in the horizontal axes
			\param x x orientation angle
			Note: this function must be implemented in inheriting classes
	 */
	virtual void RotateX(double x);

	//! Implements the lateral movements of the model  (Local coordinate system)
	/*!
			The Y parameter contains the relative movement in the vertical axes
			\param y y orientation angle
			Note: this function must be implemented in inheriting classes
	 */
	virtual void RotateY(double y);

	//! Rotate the model on its own axes  (Local coordinate system)
	/*!
			This function rotate the model on its own axis the value of an angle given
			by the "Rotation" variable the rotation is produced acting on the actors who compose the model.
			\param rotation rotation angle (radians)
			Note: this function must be implemented in inheriting classes
	 */
	virtual void RotateZ(double rotation);

	virtual void Reset();
	virtual void Restore();

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
	bool IsVisible(){ return this->Status == Visible;};

	//! Check if model is hidden
	/*!
	 * When an model is disabled it will NOT:
	 * - be visible.
	 * - be computed in collision detection.
	 * It will be used in simulation.
	 */
	bool IsHidden(){ return this->Status == Hidden;};

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

	//! Transform function of the model actor
	vtkTransform * Transform;

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

	//! Scale factor (size)
	double Scale[3];
	//! Origin point for transforms
	double Origin[3];
	//! Object Position
	double Position[3];
	//! Object orientation: Yaw, Pitch, Roll angles
	double Orientation[3];

	double Color[3];

	double Opacity;

	bool Visibility;

private:

	vtkModel (const vtkModel &);//NotImplemented
	void operator =(const vtkModel &);//Not Implemented

};
#endif
