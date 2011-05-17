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
#include "vtkXMLPolyDataReader.h"
#include "vtkXMLDataParser.h"
#include "vtkXMLDataElement.h"
#include "vtkRenderer.h"
#include "vtkRendererCollection.h"
#include "vtkRenderWindow.h"
#include "vtkDataSet.h"
#include "vtkCamera.h"
#include "vtkLight.h"
#include "vtkLightCollection.h"
#include "vtkActor.h"
#include "vtkActorCollection.h"

#include "vtkSimulation.h"
#include "vtkSimulationInteractorStyle.h"
#include "vtkScenario.h"
#include "vtkScenarioObject.h"
#include "vtkScenarioElement.h"
#include "vtkModel.h"
#include "vtkVisualizationModel.h"
#include "vtkCollisionModel.h"
#include "vtkDeformationModel.h"
#include "vtkOrgan.h"
#include "vtkTool.h"
#include "vtkToolGrasper.h"
#include "vtkToolProbe.h"
#include "vtkToolScissors.h"
#include "vtkPSSInterface.h"
#include "vtkBoundaryCondition.h"

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
void vtkSRMLImporter::SetSimulation(vtkSimulation * simulation){
	this->Simulation = simulation;
}

//----------------------------------------------------------------------------
vtkSimulation * vtkSRMLImporter::GetSimulation(){
	return this->Simulation;
}

//----------------------------------------------------------------------------
void vtkSRMLImporter::SetScenario(vtkScenario * scenario){
	this->Scenario = scenario;
}

//----------------------------------------------------------------------------
vtkScenario * vtkSRMLImporter::GetScenario(){
	return this->Scenario;
}

//----------------------------------------------------------------------------
int vtkSRMLImporter::OpenSRMLFile(){

	if (this->FileStream)
	{
		vtkErrorMacro("File already open.");
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
	//Import haptic devices functionality
	vtkXMLDataElement * simulation = this->Element;

	int verbose;
	simulation->GetScalarAttribute("Verbose", verbose);
	if(verbose)
	{
		this->Simulation->VerboseOn();
	}

	int useHaptic;
	simulation->GetScalarAttribute("UseHaptic", useHaptic);
	if(useHaptic)
	{
		this->Simulation->UseHapticOn();
		this->ImportHaptic();
	}

	double rate;
	simulation->GetScalarAttribute("SimulationRate", rate);
	this->Simulation->SetSimulationTimerRate(rate);

	simulation->GetScalarAttribute("HapticRate", rate);
	this->Simulation->SetHapticTimerRate(rate);

	simulation->GetScalarAttribute("RenderRate", rate);
	this->Simulation->SetRenderTimerRate(rate);

	double array[3];
	simulation->GetVectorAttribute("Gravity", 3, array);
	this->Simulation->SetGravity(array);

	//Import Actors (Tools, Organs, Extras), Cameras, Lights and Properties
	Superclass::ReadData();
}

//----------------------------------------------------------------------------
int vtkSRMLImporter::ImportBegin ()
{
	//Set simulation properties
	if(this->Simulation)
	{
		if(!this->Scenario)
		{
			vtkDebugMacro("Scenario being set...");
			this->SetScenario(this->Simulation->GetScenario());
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
		this->ImportScenarioObjects();
	}
	else
	{
		vtkErrorMacro("A scenario must be assigned to the importer");
		return;
	}
}

//----------------------------------------------------------------------------
void vtkSRMLImporter::ImportScenarioObjects()
{
	vtkDebugMacro("Importing Objects...");
	vtkXMLDataElement * xmlObjects = this->Element->LookupElementWithName("Objects");
	vtkXMLDataElement * xmlItem;
	for(int i=0; i<xmlObjects->GetNumberOfNestedElements(); i++)
	{
		xmlItem = xmlObjects->GetNestedElement(i);
		const char * type = xmlItem->GetAttribute("Type");
		vtkScenarioObject * object;

		if(strcmp(type, "Tool") == 0)
		{
			const char * model = xmlItem->GetAttribute("Model");
			if(strcmp(model, "Probe") == 0)
			{
				object = vtkToolProbe::New();
			}
			else if(strcmp(model, "Grasper") == 0)
			{
				object = vtkToolGrasper::New();
			}
		}
		else if(strcmp(type, "Organ") == 0)
		{
			object = vtkOrgan::New();
		}

		object->SetName(xmlItem->GetAttribute("Name"));
		this->SetScenarioElements(object, xmlItem);
		this->Scenario->AddObject(object);

		if(this->GetDebug()) object->Print(cout);

		vtkDebugMacro("Object successfully imported.");
	}
}

//----------------------------------------------------------------------------
void vtkSRMLImporter::SetScenarioElements(vtkScenarioObject * object, vtkXMLDataElement * xmlItem)
{
	vtkDebugMacro("Set Object Elements...");
	vtkXMLDataElement * xmlElements = xmlItem->LookupElementWithName("Elements");
	for(int i=0; i<xmlElements->GetNumberOfNestedElements(); i++)
	{
		vtkXMLDataElement * xmlElement = xmlElements->GetNestedElement(i);

		vtkScenarioElement * element = vtkScenarioElement::New();
		element->SetName(xmlElement->GetAttribute("Name"));

		this->SetElementModels(element, xmlElement);
		object->AddElement(element);

		if(this->GetDebug()) element->Print(cout);

		vtkDebugMacro("Element successfully imported.");
	}
}

//----------------------------------------------------------------------------
void vtkSRMLImporter::SetElementModels(vtkScenarioElement * element, vtkXMLDataElement * xmlItem)
{
	vtkDebugMacro("Set Element Models...");
	vtkXMLDataElement * xmlModels = xmlItem->LookupElementWithName("Models");
	for(int i=0; i<xmlModels->GetNumberOfNestedElements(); i++)
	{
		vtkXMLDataElement * xmlModel = xmlModels->GetNestedElement(i);
		//Once the scenario element is obtained
		const char * type = xmlModel->GetAttribute("Type");

		if(strcmp(type, "Visualization") == 0)
		{
			vtkVisualizationModel * vis = vtkVisualizationModel::New();
			this->SetModelData(vis, xmlModel);

			//Specific visualization model parameters
			vis->SetTextureFileName(this->ExpandDataFileName(xmlModel->GetAttribute("TextureFileName")));
			element->SetVisualizationModel(vis);

			if(this->GetDebug()) vis->Print(cout);
		}
		if(strcmp(type, "Collision") == 0)
		{
			vtkCollisionModel * col = vtkCollisionModel::New();
			this->SetModelData(col, xmlModel);
			element->SetCollisionModel(col);
			//Specific collision model parameters

			if(this->GetDebug()) col->Print(cout);
		}
		if(strcmp(type, "Deformation") == 0)
		{
			vtkXMLDataElement * xmlInterface = xmlModel->GetNestedElement(0);
			if(xmlInterface)
			{
				const char * interfaceType = xmlInterface->GetAttribute("Type");
				if(strcmp(interfaceType, "PSS") == 0)
				{
					vtkPSSInterface * def = vtkPSSInterface::New();
					this->SetModelData(def, xmlModel);
					element->SetDeformationModel(def);
					//Specific deformation model parameters

					def->SetDeltaT(this->Simulation->GetSimulationTimerRate());
					int gravity;
					xmlInterface->GetScalarAttribute("EnableGravity",gravity);
					if(gravity && gravity == 1) {
						def->SetGravity(this->Simulation->GetGravity());
					}

					//Set particle-spring system specific parameters
					double coefficient;
					xmlInterface->GetScalarAttribute("Spring", coefficient);
					def->SetSpringCoefficient(coefficient);
					xmlInterface->GetScalarAttribute("Damping", coefficient);
					def->SetDampingCoefficient(coefficient);
					xmlInterface->GetScalarAttribute("Distance", coefficient);
					def->SetDistanceCoefficient(coefficient);
					double mass;
					xmlInterface->GetScalarAttribute("Mass", mass);
					def->SetMass(mass);
					int value;
					xmlInterface->GetScalarAttribute("Rigidity", value);
					def->SetRigidityFactor(value);
					const char * type = xmlInterface->GetAttribute("Solver");
					if(strcmp(type, "RK4") == 0)
					{
						def->SetSolverType(vtkMotionEquationSolver::RungeKutta4);
					}
					else if(strcmp(type, "Euler") == 0)
					{
						def->SetSolverType(vtkMotionEquationSolver::Euler);
					}
					else
					{
						def->SetSolverType(vtkMotionEquationSolver::VelocityVerlet);
					}

					if(this->GetDebug()) def->Print(cout);
				}
			}
		}
		vtkDebugMacro("Model successfully imported.");
	}
}
//----------------------------------------------------------------------------
void vtkSRMLImporter::SetModelData(vtkModel * model, vtkXMLDataElement * xmlItem)
{
	const char * status = xmlItem->GetAttribute("Status");
	if (!strcmp(status, "Hidden"))
	{
		model->SetStatus(vtkModel::Hidden);
	}
	else if (!strcmp(status, "Disabled"))
	{
		model->SetStatus(vtkModel::Disabled);
	}

	model->SetName(xmlItem->GetAttribute("Name"));
	model->SetFileName(this->ExpandDataFileName(xmlItem->GetAttribute("FileName")));

	double array[3];
	xmlItem->GetVectorAttribute("Scale", 3, array);
	model->SetScale(array);
	xmlItem->GetVectorAttribute("Position", 3, array);
	model->SetPosition(array[0], array[1], array[2]);
	xmlItem->GetVectorAttribute("Orientation", 3, array);
	model->SetOrientation(array[0], array[1], array[2]);
	xmlItem->GetVectorAttribute("Origin", 3, array);
	model->SetOrigin(array[0], array[1], array[2]);
	xmlItem->GetVectorAttribute("Color", 3, array);
	model->SetColor(array[0], array[1], array[2]);

	int visibility;
	xmlItem->GetScalarAttribute("Visibility", visibility);
	model->SetVisibility(visibility);

	double opacity;
	xmlItem->GetScalarAttribute("Opacity", opacity);
	model->SetOpacity(opacity);

}

//----------------------------------------------------------------------------
void vtkSRMLImporter::ImportCameras (vtkRenderer * renderer)
{
	vtkDebugMacro("Importing Cameras...");
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
	fullName = new char[strlen(this->DataPath) + 2 + strlen(fname)];
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
