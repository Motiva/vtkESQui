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
#ifndef __vtkSRMLImporter_h
#define __vtkSRMLImporter_h

#include "vtkESQuiUtilitiesWin32Header.h"
#include "vtkImporter.h"

#include "vtkSimulation.h"
#include "vtkScenario.h"

#ifndef VTKESQUI_USE_NO_HAPTICS
	#include "vtkHaptic.h"
	#include "vtkIHP.h"
	#include "vtkVSP.h"
	#include "vtkLSW.h"
#endif

class vtkXMLDataElement;
class vtkXMLDataParser;
class vtkCamera;
class vtkLight;
class vtkActor;

class vtkTool;
class vtkToolPincers;
class vtkOrgan;
class vtkBioMechanicalModel;

//! Import an SRML scenario file into vtkESQui platform

class VTK_ESQUI_UTILITIES_EXPORT vtkSRMLImporter: public vtkImporter
{
public:

	//! Type revision macro
	vtkTypeRevisionMacro(vtkSRMLImporter,vtkImporter);
	//!Create new vtkSRMLImporter object
	static vtkSRMLImporter *New();
	//!Return class name
	const char *GetClassName() { return "vtkSRMLImporter"; }
	//!Print the attributes value
	void PrintSelf(ostream& os, vtkIndent indent);

	//! Set the name of the input file.
	vtkSetStringMacro(FileName);
	//! Get the name of the input file.
	vtkGetStringMacro(FileName);

	//! Set the data path.
	vtkSetStringMacro(DataPath);
	//! Get the data path.
	vtkGetStringMacro(DataPath);

	//! Set the Simulation
	vtkSetObjectMacro(Simulation, vtkSimulation);
	//! Get the Simulation
	vtkGetObjectMacro(Simulation, vtkSimulation);

protected:	

	vtkSRMLImporter();
	~vtkSRMLImporter();

	// Description:
	//! Set the simulation scenario
	vtkSetObjectMacro(Scenario, vtkScenario);
	//! Get the simulation scenario
	vtkGetObjectMacro(Scenario, vtkScenario);

	//! Starts the Import process
	virtual int ImportBegin ();
	//! Finalize the import process
	virtual void ImportEnd ();
	//! Import all the scenario actors (tools, organs, extras)
	virtual void ImportActors (vtkRenderer * renderer);
	//! Import scenario cameras
	virtual void ImportCameras (vtkRenderer * renderer);
	//! Import scenario lights
	virtual void ImportLights (vtkRenderer * renderer);
	//! Import specific scenario props
	virtual void ImportProperties (vtkRenderer * renderer);

	//! Import tools from the SRML file
	void ImportTools();
	//! Import organs from the SRML file
	void ImportOrgans();
	//! Import the extras from the SRML file
	void ImportExtras();
	//! Import haptic device information
	void ImportHaptic();

	//!A SRML file is a basic XML file, so the parser corresponds to a vtkXMLParser object
	vtkXMLDataParser * SRMLParser;
	//!Basic XMLElement used in XML Parsing
	vtkXMLDataElement * Element;
	//!Input SRML Filename
	char * FileName;
	//!Path of the data files
	char * DataPath;
	//!XML data stream
	istream * Stream;
	//!Read error flag
	int ReadError;
	//!Data error flag
	int DataError;
	//!Information error flag
	int InformationError;

	//! Simulation object to be imported
	vtkSimulation * Simulation;
	//! Simulation Scenario. It is automatically set through the SetSimulation method
	vtkScenario * Scenario;

private:

	vtkSRMLImporter (const vtkSRMLImporter &); //Not Implemented
	void operator =(const vtkSRMLImporter &); //Not Implemented

	//! Opens the SRML file for parsing its content
	int OpenSRMLFile();

	//! Reads the SRML file
	int ReadSRMLFile(vtkXMLDataElement * e);

	//! Closes the SRML file after parsing its content
	void CloseSRMLFile();

	//! Create a new SRML file parser (XML file parser)
	void CreateSRMLParser();

	//! Destroy the SRML file parser after its execution
	void DestroySRMLParser();

	//! Parse SRML information
	int ReadSRMLData();

	//! Read parsed information
	void ReadData();

	//! Set specific camera values
	/*!
	 * \param camera camera object
	 * \param item parsed XMLDataElement that contains element info
	 */
	void SetCameraData(vtkCamera * camera, vtkXMLDataElement * item);
	//! Set specific light values
	/*!
	 * \param light light object
	 * \param item parsed XMLDataElement that contains element info
	 */
	void SetLightData(vtkLight * light, vtkXMLDataElement * item);
	//! Set specific actor values
	/*!
	 * \param actor scenario item actor object
	 * \param item parsed XMLDataElement that contains element info
	 */
	void SetActorData(vtkActor * actor, vtkXMLDataElement * item);
	//! Set specific tool values
	/*!
	 * \param tool scenario tool object
	 * \param item parsed XMLDataElement that contains element info
	 */
	void SetToolData(vtkTool * tool, vtkXMLDataElement * item);
	//! Set specific laparoscopic pincers values
	/*!
	 * \param tool pincers tool object
	 * \param item parsed XMLDataElement that contains element info
	 */
	void SetToolPincersData(vtkToolPincers * tool, vtkXMLDataElement * item);
	//! Set specific organ values
	/*!
	 * \param organ organ object
	 * \param item parsed XMLDataElement that contains element info
	 */
	void SetOrganData(vtkOrgan * organ, vtkXMLDataElement * item);
	//! Set initial biomechanical model properties
	/*!
	 * \param model biomechanical model
	 * \param item parsed XMLDataElement that contains element info
	 */
	void SetBioMechanicalModelData(vtkBioMechanicalModel * model, vtkXMLDataElement * item);

	//! Generate full path of filename
	const char * ExpandDataFileName(const char * fname);

#ifndef VTKESQUI_USE_NO_HAPTICS
	void SetHapticData(vtkHaptic * haptic, vtkXMLDataElement * item);
#endif

	//! File stream to retrieve SRML file content
	ifstream * FileStream;

};
#endif

