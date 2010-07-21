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
#include "vtkSRMLImporter.h"

#include "vtkObjectFactory.h"
#include "vtkObject.h"
#include "vtkXMLDataParser.h"
#include "vtkXMLDataElement.h"
#include "vtkRenderer.h"
#include "vtkRendererCollection.h"
#include "vtkDataSet.h"
#include "vtkCamera.h"
#include "vtkLight.h"
#include "vtkLightCollection.h"
#include "vtkActor.h"
#include "vtkActorCollection.h"

#include "vtkTool.h"
#include "vtkToolCollection.h"
#include "vtkPiece.h"
#include "vtkPieceCollection.h"
#include "vtkToolPincers.h"
#include "vtkOrgan.h"
#include "vtkOrganCollection.h"
#include "vtkMSSInterface.h"
#include "vtkPSSInterface.h"
#include "vtkRDMInterface.h"

#include <sys/stat.h>
#include <assert.h>

vtkCxxRevisionMacro(vtkSRMLImporter, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkSRMLImporter);

//----------------------------------------------------------------------------
vtkSRMLImporter::vtkSRMLImporter()
{
	this->SRMLParser = NULL;
	this->Simulation = NULL;
	this->Scenario = NULL;
	this->RenderWindow = NULL;
	this->Renderer = NULL;
	this->DataPath = NULL;
	this->FileName = NULL;
	this->FileStream = NULL;
	this->Stream = NULL;
	this->DataError = 0;
	this->ReadError = 0;
	this->InformationError = 0;
}

//----------------------------------------------------------------------------
vtkSRMLImporter::~vtkSRMLImporter()
{
	if(this->SRMLParser)
	{
		this->DestroySRMLParser();
	}
}

//----------------------------------------------------------------------------
int vtkSRMLImporter::OpenSRMLFile(){

	if (this->FileStream)
	{
		vtkErrorMacro("File already open.")
		return 1;
	}

	if(!this->Stream && !this->FileName)
	{
		vtkErrorMacro("File name not specified.");
		return 0;
	}

	if(this->Stream)
	{
		return 1;
	}

	// Need to open a file.  First make sure it exists.  This prevents
	// an empty file from being created on older compilers.
	struct stat fs;
	if(stat(this->FileName, &fs) != 0)
	{
		vtkErrorMacro("Error opening file " << this->FileName);
		return 0;
	}

	#ifdef _WIN32
		this->FileStream = new ifstream(this->FileName, ios::binary | ios::in);
	#else
		this->FileStream = new ifstream(this->FileName, ios::in);
	#endif

		if(!this->FileStream || !(*this->FileStream))
	{
		vtkErrorMacro("Error opening file " << this->FileName);
		if(this->FileStream)
		{
			delete this->FileStream;
			this->FileStream = 0;
		}
		return 0;
	}

	// Use the file stream.
	this->Stream = this->FileStream;

	return 1;
}

//----------------------------------------------------------------------------
int vtkSRMLImporter::ReadSRMLFile(vtkXMLDataElement * e)
{
	return (e->GetNumberOfNestedElements() > 0);
}

//----------------------------------------------------------------------------
int vtkSRMLImporter::ReadSRMLData()
{
	//Destroy any old info
	if(this->SRMLParser)
	{
		this->DestroySRMLParser();
	}

	// Create the vtkXMLParser instance used to parse the file.
	this->CreateSRMLParser();

	// Configure the parser for this file.
	this->SRMLParser->SetStream(this->Stream);

	// Parse the input file.
	if(this->SRMLParser->Parse())
	{
		// Let the subclasses read the information they want.
		if(!this->ReadSRMLFile(this->SRMLParser->GetRootElement()))
		{
			// There was an error reading the file.
			this->ReadError = 1;
		}
		else
		{
			this->ReadError = 0;
			this->Element = this->SRMLParser->GetRootElement();
		}
	}
	else
	{
		vtkErrorMacro("Error parsing input file.  ReadSRMLData aborting.");
		// The output should be empty to prevent the rest of the pipeline
		// from executing.
		this->ReadError = 1;
	}

	return !this->ReadError;
}

//--------------------------------------------------------------------------
void vtkSRMLImporter::CloseSRMLFile()
{
  if(!this->Stream)
    {
    vtkErrorMacro("File not open.");
    return;
    }
  if(this->Stream == this->FileStream)
    {
    // We opened the file.  Close it.
    this->FileStream->close();
    delete this->FileStream;
    this->FileStream = 0;
    this->Stream = 0;
    }
}

//----------------------------------------------------------------------------
void vtkSRMLImporter::CreateSRMLParser()
{
  if(this->SRMLParser)
    {
    vtkErrorMacro("CreateXMLParser() called with existing XMLParser.");
    this->DestroySRMLParser();
    }
  this->SRMLParser = vtkXMLDataParser::New();
}

//----------------------------------------------------------------------------
void vtkSRMLImporter::DestroySRMLParser()
{
  if(!this->SRMLParser)
    {
    vtkErrorMacro("DestroyXMLParser() called with no current XMLParser.");
    return;
    }
  this->SRMLParser->Delete();
  this->SRMLParser = 0;
}

//----------------------------------------------------------------------------
//SRML Importer specific import.
void vtkSRMLImporter::ReadData()
{
	//Import Actors (Tools, Organs, Extras), Cameras, Lights and Properties
	Superclass::ReadData();

	//Import haptic devices functionality
	vtkXMLDataElement * simulation = this->Element;
	std::cout << "== Simulation ==\n" <<
			"Name: " << simulation->GetAttribute("Name") << "\n" <<
			"Type: " << simulation->GetAttribute("Type") << "\n" <<
			"UseHaptic: " << simulation->GetAttribute("UseHaptic") << std::endl;
	int useHaptic;
	simulation->GetScalarAttribute("UseHaptic", useHaptic);
	if(useHaptic)
	{
		this->Simulation->UseHapticOn();
		this->ImportHaptic();
	}

	//Initlialize Simulation
	this->Simulation->Init();
}

//----------------------------------------------------------------------------
int vtkSRMLImporter::ImportBegin ()
{
	//Set simulation properties
	if(this->Simulation)
	{
		if(!this->Scenario)
		{
			cout << "Scenario being set...\n";
			this->SetScenario(this->Simulation->GetSimulationManager()->GetScenario());
			this->SetRenderWindow(this->Scenario->GetRenderWindow());
			this->Renderer = this->RenderWindow->GetRenderers()->GetFirstRenderer();
		}
	}
	else
	{
		//Simulation must have a renderWindow and a renderer predefined
		cout << "Simulation has not been properly defined" << endl;
		return 0;
	}
	vtkDebugMacro(<< "Opening import file as binary");
	// Open the input file.  If it fails, the error was already
	// reported by OpenSRMLFile.
	if(!this->OpenSRMLFile())
	{
		std::cout << "File has not been opened..." << std::endl;
		return 0;
	}
	return this->ReadSRMLData();
}

//----------------------------------------------------------------------------
void vtkSRMLImporter::ImportEnd ()
{
	this->CloseSRMLFile();
}

//----------------------------------------------------------------------------
void vtkSRMLImporter::ImportActors (vtkRenderer * renderer)
{
	vtkDebugMacro("Importing Actors...");
	if(this->Scenario)
	{
		//File Info will be imported using esqui classes (Tools & Organs)
		ImportTools();
		ImportOrgans();
		ImportExtras();
	}
	else
	{
		//File Info will be imported using vtk classes
		if(this->RenderWindow)
		{
			//TODO: Import without the ESQUI structure
		}
		else return;
	}
}

//----------------------------------------------------------------------------
void vtkSRMLImporter::ImportTools()
{
	vtkDebugMacro("Importing Tools...");
	vtkXMLDataElement * tools = this->Element->LookupElementWithName("Tools");
	vtkXMLDataElement * item;
	for(int i=0; i<tools->GetNumberOfNestedElements(); i++)
	{
		item = tools->GetNestedElement(i);
		const char * type = item->GetAttribute("Type");

		if(strcmp(type, "Pincers") == 0)
		{
			vtkToolPincers * tool = vtkToolPincers::New();
			this->SetToolPincersData(tool, item);
		}
		//TODO: Complete with the rest of the tools
	}
}

//----------------------------------------------------------------------------
void vtkSRMLImporter::SetToolData(vtkTool * tool, vtkXMLDataElement * item)
{
	tool->SetName(item->GetAttribute("Name"));
	tool->SetToolType(item->GetAttribute("Type"));

	double array[3];
	item->GetVectorAttribute("Position", 3, array);
	tool->SetPosition(array[0], array[1], array[2]);
	item->GetVectorAttribute("Orientation", 3, array);
	tool->SetOrientation(array[0], array[1], array[2]);
	item->GetVectorAttribute("Origin", 3, array);
	tool->SetOrigin(array[0], array[1], array[2]);

	double scale;
	item->GetScalarAttribute("Scale", scale);
	tool->SetScale(scale);
}

//----------------------------------------------------------------------------
void vtkSRMLImporter::SetToolPincersData(vtkToolPincers * tool, vtkXMLDataElement * item)
{
	//Set common tool data
	this->SetToolData(tool, item);

	//Set specific vtkToolPincers parameters
	for (int j=0; j<tool->GetNumberOfPieces();j++)
	{
		vtkXMLDataElement * child;
		child = item->GetNestedElement(j);
		int id;
		child->GetScalarAttribute("Id", id);
		tool->SetFileName(id, ExpandDataFileName(child->GetAttribute("FileName")));
	}

	//tool->Print(cout);
	this->Scenario->InsertNextTool(tool);
}

//----------------------------------------------------------------------------
void vtkSRMLImporter::ImportOrgans()
{
	vtkDebugMacro("Importing Organs...");
	vtkXMLDataElement * organs = this->Element->LookupElementWithName("Organs");
	vtkXMLDataElement * item;
	for(int i=0; i<organs->GetNumberOfNestedElements(); i++)
	{
		item = organs->GetNestedElement(i);
		vtkOrgan * organ = vtkOrgan::New();
		this->SetOrganData(organ, item);
	}
}

//----------------------------------------------------------------------------
void vtkSRMLImporter::SetOrganData(vtkOrgan * organ, vtkXMLDataElement * item)
{
	organ->SetName(item->GetAttribute("Name"));
	organ->SetFileName(ExpandDataFileName(item->GetAttribute("FileName")));
	if (item->GetAttribute("TextureFile"))
	{
		organ->SetTextureFileName(ExpandDataFileName(item->GetAttribute("TextureFile")));
	}

	double array[3];
	item->GetVectorAttribute("Position", 3, array);
	organ->SetPosition(array[0], array[1], array[2]);
	item->GetVectorAttribute("Orientation", 3, array);
	organ->SetOrientation(array[0], array[1], array[2]);

	double scale;
	item->GetScalarAttribute("Scale", scale);
	organ->SetScale(scale);

	//Biomechanical Model Import
	vtkXMLDataElement * bmm = item->LookupElementWithName("BMM");
	vtkBioMechanicalModel * model;
	const char * name = bmm->GetAttribute("Name");
	if(!strcmp(name, "vtkMSS"))
	{
		model = vtkMSSInterface::New();
	}
	else if (!strcmp(name, "vtkPSS"))
	{
		cout << "vtkParticleSpringSystem BioMech Model" << endl;
		model = vtkPSSInterface::New();
	}
	else if (!strcmp(name, "vtkFeMesh"))
	{
		cout << "vtkFeMesh BioMech Model" << endl;
		//model = vtkFeMeshInterface::New();
	}
	else if (!strcmp(name, "vtkRDM"))
	{
		cout << "Robust Deformation Model" << endl;
		model = vtkRDMInterface::New();
	}
	model->SetName(bmm->GetAttribute("Name"));
	this->SetBioMechanicalModelData(model ,bmm);
	organ->SetBioMechanicalModel(model);

	this->Scenario->InsertNextOrgan(organ);
}

//----------------------------------------------------------------------------
void vtkSRMLImporter::SetBioMechanicalModelData(vtkBioMechanicalModel * model, vtkXMLDataElement * item)
{
	const char * name = model->GetName();
	if(!strcmp(name, "vtkMSS"))
	{
		vtkMSSInterface * bmm = vtkMSSInterface::SafeDownCast(model);
		double coefficient;
		item->GetScalarAttribute("DistanceCoefficient", coefficient);
		bmm->SetDistanceCoefficient(coefficient);
		item->GetScalarAttribute("DampingCoefficient", coefficient);
		bmm->SetDampingCoefficient(coefficient);
		double mass;
		item->GetScalarAttribute("Mass", mass);
		bmm->SetMass(mass);
		double delta;
		item->GetScalarAttribute("DeltaT", delta);
		bmm->SetDeltaT(delta);
		int value;
		item->GetScalarAttribute("Steps", value);
		bmm->SetSteps(value);
	}
	else if(!strcmp(name, "vtkPSS"))
	{
		vtkPSSInterface * bmm = vtkPSSInterface::SafeDownCast(model);
		double coefficient;
		item->GetScalarAttribute("SpringCoefficient", coefficient);
		bmm->SetSpringCoefficient(coefficient);
		item->GetScalarAttribute("DampingCoefficient", coefficient);
		bmm->SetDampingCoefficient(coefficient);
		item->GetScalarAttribute("DistanceCoefficient", coefficient);
		bmm->SetDistanceCoefficient(coefficient);
		double mass;
		item->GetScalarAttribute("Mass", mass);
		bmm->SetMass(mass);
		double delta;
		item->GetScalarAttribute("DeltaT", delta);
		bmm->SetDeltaT(delta);
		int value;
		item->GetScalarAttribute("RigidityFactor", value);
		bmm->SetRigidityFactor(value);
	}
	else if (!strcmp(name, "vtkFeMesh"))
	{
		cout << "vtkFeMesh BioMech Model Data" << endl;
		//vtkFeMeshInterface * bmm = vtkFeMeshInterface::SafeDownCast(model);
	}
	else if (!strcmp(name, "vtkRDM"))
	{
		cout << "vtkRDM BioMech Model Data" << endl;
		vtkRDMInterface * bmm = vtkRDMInterface::SafeDownCast(model);

		double coefficient;
		item->GetScalarAttribute("DistanceCoefficient", coefficient);
		bmm->SetDistanceCoefficient(coefficient);
		item->GetScalarAttribute("DampingCoefficient", coefficient);
		bmm->SetDampingCoefficient(coefficient);
		item->GetScalarAttribute("SurfaceCoefficient", coefficient);
		bmm->SetSurfaceCoefficient(coefficient);
		item->GetScalarAttribute("VolumeCoefficient", coefficient);
		bmm->SetVolumeCoefficient(coefficient);
		double mass;
		item->GetScalarAttribute("Mass", mass);
		bmm->SetMass(mass);
		double delta;
		item->GetScalarAttribute("DeltaT", delta);
		bmm->SetDeltaT(delta);
	}
}

//----------------------------------------------------------------------------
void vtkSRMLImporter::ImportExtras()
{
	vtkDebugMacro("Importing Extras...");
}

//----------------------------------------------------------------------------
void vtkSRMLImporter::ImportCameras (vtkRenderer * renderer)
{
	vtkDebugMacro("Importing Extras...");
	vtkXMLDataElement * cameras = this->Element->LookupElementWithName("Cameras");
	vtkXMLDataElement * item;
	if(this->Scenario || this->RenderWindow || renderer)
	{
		if(cameras->GetNumberOfNestedElements() > 0)
		{
			item = cameras->GetNestedElement(0);
			vtkCamera * camera = this->Renderer->GetActiveCamera();

			this->SetCameraData(camera, item);
			this->Renderer->SetActiveCamera(camera);
		}
	}
	else return;
}

//----------------------------------------------------------------------------
void vtkSRMLImporter::SetCameraData(vtkCamera * camera, vtkXMLDataElement * item)
{
	double array[3];
	item->GetVectorAttribute("Position", 3, array);
	camera->SetPosition(array[0], array[1], array[2]);
	item->GetVectorAttribute("FocalPoint", 3, array);
	camera->SetFocalPoint(array[0], array[1], array[2]);

	double angle;
	item->GetScalarAttribute("ViewAngle", angle);
	camera->SetViewAngle(angle);

	item->GetScalarAttribute("Yaw", angle);
	camera->Yaw(angle);

	item->GetScalarAttribute("Elevation", angle);
	camera->Elevation(angle);

	item->GetScalarAttribute("Pitch", angle);
	camera->Pitch(angle);

	item->GetScalarAttribute("Dolly", angle);
	camera->Dolly(angle);

	int projection;
	item->GetScalarAttribute("ParallelProjection", projection);
	camera->SetParallelProjection(projection);
}

//----------------------------------------------------------------------------
void vtkSRMLImporter::ImportLights (vtkRenderer * renderer)
{
	vtkDebugMacro("Importing Lights...");
	vtkXMLDataElement * lights = this->Element->LookupElementWithName("Lights");
	vtkXMLDataElement * item;

	if(this->Scenario || this->RenderWindow || renderer)
	{
		this->Renderer->RemoveAllLights();
		for(int i=0; i<lights->GetNumberOfNestedElements();i++)
		{
			item = lights->GetNestedElement(i);
			vtkLight * light = vtkLight::New();
			this->SetLightData(light, item);
			this->Renderer->AddLight(light);
		}
	}
	else return;
}

//--------------------------------------------------------------------------
void vtkSRMLImporter::SetLightData(vtkLight * light, vtkXMLDataElement * item)
{
	double array[3];
	item->GetVectorAttribute("AmbientColor", 3, array);
	light->SetAmbientColor(array[0], array[1], array[2]);
	item->GetVectorAttribute("SpecularColor", 3, array);
	light->SetSpecularColor(array[0], array[1], array[2]);
	item->GetVectorAttribute("DiffuseColor", 3, array);
	light->SetDiffuseColor(array[0], array[1], array[2]);

	double intensity;
	item->GetScalarAttribute("Intensity", intensity);
	light->SetIntensity(intensity);

	int positional;
	item->GetScalarAttribute("Positional", positional);
	light->SetPositional(positional);

	double angle;
	item->GetScalarAttribute("ConeAngle", angle);
	light->SetConeAngle(angle);

	const char * type = item->GetAttribute("LightType");
	if(strcmp(type, "Scene") == 0)
	{
		light->SetLightTypeToSceneLight();
	}
	else if(strcmp(type, "Head") == 0)
	{
		light->SetLightTypeToHeadlight();
	}
	else if(strcmp(type, "Camera") == 0)
	{
		light->SetLightTypeToCameraLight();
	}

	//light->Print(cout);
}

//----------------------------------------------------------------------------
void vtkSRMLImporter::ImportProperties (vtkRenderer * renderer)
{
	vtkDebugMacro("Importing Properties...");
	vtkXMLDataElement * environment = this->Element->LookupElementWithName("Environment");

	if(this->Scenario || this->RenderWindow || renderer)
	{
		double array[3];
		environment->GetVectorAttribute("Background", 3, array);
		this->Renderer->SetBackground(array);
	}
	else return;
}

//----------------------------------------------------------------------------
void vtkSRMLImporter::ImportHaptic()
{

#ifndef VTKESQUI_USE_NO_HAPTICS

	vtkHaptic * haptic;
	vtkXMLDataElement * item = this->Element->LookupElementWithName("Haptic");

	const char * type = item->GetAttribute("Type");
	if(strcmp(type, "vtkIHP") == 0)
	{
		haptic = vtkIHP::New();
	}
	else if(strcmp(type, "vtkVSP") == 0)
	{
	}
	else if(strcmp(type, "vtkLSW") == 0)
	{
	}

	int numberOfTools = 0;
	item->GetScalarAttribute("NumberOfTools", numberOfTools);

	haptic->SetName(type);
	haptic->SetNumberOfTools(numberOfTools);

	this->Simulation->SetHapticDevice(haptic);

#endif

}

//--------------------------------------------------------------------------
const char * vtkSRMLImporter::ExpandDataFileName(const char * fname)
{
	char * fullName;
	fullName = new char[strlen(this->DataPath) + 1 + strlen(fname)];
	fullName[0] = 0;
	strcat(fullName, this->DataPath);
	size_t len = strlen(fullName);
	fullName[len] = '/';
	fullName[len+1] = 0;
	strcat(fullName, fname);

	return fullName;
}

//----------------------------------------------------------------------------
void vtkSRMLImporter::PrintSelf(ostream& os,vtkIndent indent) {
	this->Superclass::PrintSelf(os,indent);

	os << indent << "FileName: " << this->FileName << "\n";
	os << indent << "Element: " << this->Element->GetName() << "\n";
	os << indent << "ReadError: " << this->ReadError << "\n";

}
