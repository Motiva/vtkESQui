
#include "vtkSRMLReader.h"
#include "vtkObjectFactory.h"

#include "vtkCamera.h"
#include "vtkLight.h"

#include "vtkDefaultInteractorStyle.h"
#include "vtkSingleChannelInteractorStyle.h"

#include "vtkToolCatheter.h"
#include "vtkToolDummy.h"
#include "vtkToolEndovascular.h"
#include "vtkToolGrasper.h"
#include "vtkToolProbe.h"
#include "vtkToolScissors.h"
#include "vtkToolSingleChannel.h"

#include "vtkOrgan.h"
#include "vtkScenarioElement.h"
#include "vtkVisualizationModel.h"
#include "vtkCollisionModel.h"
#include "vtkDeformationModel.h"
#include "vtkParticleSpringSystemInterface.h"

#ifdef VTKESQUI_USE_LSW
	#include "vtkLSW.h"
	#include "vtkLSWTool.h"
#endif
#ifdef VTKESQUI_USE_IHP
	#include "vtkIHP.h"
#endif
#ifdef VTKESQUI_USE_VSP
	#include "vtkVSP.h"
#endif
#ifdef VTKESQUI_USE_SBM
	#include "vtkSBM.h"
#endif

vtkCxxRevisionMacro(vtkSRMLReader, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkSRMLReader);

//----------------------------------------------------------------------------
vtkSRMLReader::vtkSRMLReader()
{
  this->Simulation = NULL;
  this->Scenario = NULL;
  this->Root = NULL;
}

//----------------------------------------------------------------------------
vtkSRMLReader::~vtkSRMLReader()
{
  // It really shouldn't delete anything. The user may need it.
}

//----------------------------------------------------------------------------
vtkSimulation * vtkSRMLReader::ConstructSimulation()
{
	if (!this->Simulation && this->FileName){
		if (this->Parse()){
			if (this->CheckStructure()){
				this->InternalConstructSimulation();
			}
			else{
				vtkErrorMacro("The SRML file is bad constructed");
			}
		}
		else{
			vtkErrorMacro("This is not a readable SRML file");
		}
	}
	return this->Simulation;
}

//----------------------------------------------------------------------------
void vtkSRMLReader::PrintSelf(ostream& os,vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  //os << indent << "AnyField: " << this->AnyField << endl;
}

//----------------------------------------------------------------------------
bool vtkSRMLReader::CheckStructure()
{	
	// Simulation
	this->Root = this->GetRootElement();
	if(vtkStdString(this->Root->GetName()) != "Simulation"){
		vtkErrorMacro("Root element must be a 'Simulation' element");
		return false;
	}
	// Required Attributes
	if(!this->Root->GetAttribute("DataPath") ||
		!this->Root->GetAttribute("UseHaptic") ||
		!this->Root->GetAttribute("RenderRate") ||
		!this->Root->GetAttribute("Gravity")){
			vtkErrorMacro("Missing attribute in Simulation element");
			return false;
	}
	// Allowed children:
	// <!ELEMENT Simulation (Scenario, Haptic?)>
	int children = 1;
	if (!this->Root->FindNestedElementWithName("Scenario")){
		vtkErrorMacro("Scenario element not found");
		return false;
	}
	if (this->Root->FindNestedElementWithName("Collision")) children ++;
	if (this->Root->FindNestedElementWithName("Haptic")) children ++;
	if (this->Root->GetNumberOfNestedElements() > children){
		vtkErrorMacro("Found not allowed elements nested in Simulation");
		return false;
	}
	
	// Haptic
	if (vtkXMLDataElement *haptic = this->Root->FindNestedElementWithName("Haptic")){
		// Required Attributes
		if (!haptic->GetAttribute("Type") ||
			!haptic->GetAttribute("NumberOfTools")){
				vtkErrorMacro("Attribute NumberOfTools is missing in Haptic element");
				return false;
		}
	}
	
	// Scenario
	vtkXMLDataElement *sc = this->Root->FindNestedElementWithName("Scenario");
	// Required Attributes
	if (!sc->GetAttribute("Name")){
		vtkErrorMacro("Scenario name is missing");
		return false;
	}
	// Allowed children
	// <!ELEMENT Scenario (Environment, Objects?)>
	children = 1;
	if (!sc->FindNestedElementWithName("Environment")){
		vtkErrorMacro("Environment not found in Scenario");
		return false;
	}
	if (sc->FindNestedElementWithName("Objects")) children ++;
	if (sc->GetNumberOfNestedElements() > children){
		vtkErrorMacro("Found not allowed elements nested in Scenario");
		return false;
	}
	
	// Environment
	vtkXMLDataElement *env = sc->FindNestedElementWithName("Environment");
	// Required Attributes
	if (!env->GetAttribute("Ambient") ||
		!env->GetAttribute("Background")){
			vtkErrorMacro("Missing attribute in Environment element");
			return false;
	}
	// Allowed children
	// <!ELEMENT Environment (Cameras, Lights)>
	if (env->GetNumberOfNestedElements() != 2){
		vtkErrorMacro("Found not allowed elements nested in Environment");
		return false;
	}
	if (!env->FindNestedElementWithName("Cameras") ||
		!env->FindNestedElementWithName("Lights")){
			vtkErrorMacro("Missing elements in Environment");
			return false;
	}
	
	// Cameras
	vtkXMLDataElement *cams = env->FindNestedElementWithName("Cameras");
	// Allowed children
	// <!ELEMENT Cameras (Camera+)>
	if (cams->GetNumberOfNestedElements() < 1){
		vtkErrorMacro("No camera found");
		return false;
	}
	for (int i=0; i<cams->GetNumberOfNestedElements(); i++){
		// Camera
		vtkXMLDataElement *cam = cams->GetNestedElement(i);
		if (vtkStdString(cam->GetName()) != "Camera"){
			vtkErrorMacro("Found not allowed element nested in Cameras");
			return false;
		}
		// Required attributes
		if (!cam->GetAttribute("Id") ||
			!cam->GetAttribute("Position") ||
			!cam->GetAttribute("FocalPoint") ||
			!cam->GetAttribute("Orientation") ||
			!cam->GetAttribute("ViewAngle") ||
			!cam->GetAttribute("ParallelProjection") ||
			!cam->GetAttribute("Yaw") ||
			!cam->GetAttribute("Elevation") ||
			!cam->GetAttribute("Pitch") ||
			!cam->GetAttribute("Dolly")){
				vtkErrorMacro("Missing attributes in Camera");
				return false;
		}
	}
	
	// Lights
	vtkXMLDataElement *lights = env->FindNestedElementWithName("Lights");
	// Allowed children
	// <!ELEMENT Lights (Light+)>
	// At this version, no light element allowed (default light)
	/*
	if (lights->GetNumberOfNestedElements() < 1){
		vtkErrorMacro("No light could be found");
		return false;
	}
	*/
	for (int i=0; i < lights->GetNumberOfNestedElements(); i++){
		// Light
		vtkXMLDataElement *light = lights->GetNestedElement(i);
		if (vtkStdString(light->GetName()) != "Light"){
			vtkErrorMacro("Found not allowed element nested in Lights");
			return false;
		}
		// Required attributes
		if (!light->GetAttribute("Id") ||
			!light->GetAttribute("LightType") ||
			!light->GetAttribute("Positional") ||
			!light->GetAttribute("Intensity") ||
			!light->GetAttribute("AmbientColor") ||
			!light->GetAttribute("DiffuseColor") ||
			!light->GetAttribute("SpecularColor") ||
			!light->GetAttribute("ConeAngle")){
				vtkErrorMacro("Missing attributes in Light");
				return false;
		}
	}
	
	// Objects
	vtkXMLDataElement *objects = sc->FindNestedElementWithName("Objects");
	// No objects? That's also fine
	if (!objects) return true;
	// Allowed children
	// <!ELEMENT Objects (Object+)>
	if (objects->GetNumberOfNestedElements() < 1){
		vtkErrorMacro("No objects could be found");
		return false;
	}
	for (int i=0; i < objects->GetNumberOfNestedElements(); i++){
		// Object
		vtkXMLDataElement *object = objects->GetNestedElement(i);
		if (vtkStdString(object->GetName()) != "Object"){
			vtkErrorMacro("Found not allowed element nested in Objects");
			return false;
		}
		// Required attributes
		if (!object->GetAttribute("Type") ||
			!object->GetAttribute("Status") ||
			!object->GetAttribute("Id")){
				vtkErrorMacro("Missing attributes in Object");
				return false;
		}
		// Allowed children
		// <!ELEMENT Object (Elements)>
		if (object->GetNumberOfNestedElements() != 1 ||
			!object->FindNestedElementWithName("Elements")){
				vtkErrorMacro("Found not allowed element nested in Object");
				return false;
		}
		
		// Elements
		vtkXMLDataElement *elements = object->FindNestedElementWithName("Elements");
		// Allowed children
		// <!ELEMENT Elements (Element+)>
		if (elements->GetNumberOfNestedElements() < 1){
			vtkErrorMacro("No element could be found in Elements");
			return false;
		}
		for (int j=0; j < elements->GetNumberOfNestedElements(); j++){
			// Element
			vtkXMLDataElement *element = elements->GetNestedElement(j);
			if (vtkStdString(element->GetName()) != "Element"){
				vtkErrorMacro("Found not allowed element nested in Elements");
				return false;
			}
			// Required attributes
			if (!element->GetAttribute("Type") ||
				!element->GetAttribute("Status") ||
				!element->GetAttribute("Origin") ||
				!element->GetAttribute("Orientation") ||
				!element->GetAttribute("Position") ||
				!element->GetAttribute("Scale") ||
				!element->GetAttribute("Id")){
					vtkErrorMacro("Missing attribute in Element");
					return false;
			}
			// Allowed children
			// <!ELEMENT Element (Models)>
			if (element->GetNumberOfNestedElements() != 1 ||
				!element->FindNestedElementWithName("Models")){
					vtkErrorMacro("No Models group could be found in Element");
					return false;
			}
			
			// Models
			vtkXMLDataElement *models = element->FindNestedElementWithName("Models");
			// Allowed children
			// <!ELEMENT Models (Model+)> :: At least one visualization model
			int IsVisualization = 0;
			int IsCollision = 0;
			int IsDeformation = 0;
			if (models->GetNumberOfNestedElements() < 1 ||
				models->GetNumberOfNestedElements() > 3){
					vtkErrorMacro("Wrong number of models in Element");
					return false;
			}
			for (int k=0; k < models->GetNumberOfNestedElements(); k++){
				// Model
				vtkXMLDataElement *model = models->GetNestedElement(k);
				if (vtkStdString(model->GetName()) != "Model"){
					vtkErrorMacro("Found not allowed element nested in Models");
					return false;
				}
				// Required attributes
				if (!model->GetAttribute("Type") ||
					!model->GetAttribute("Status") ||
					!model->GetAttribute("Id") ||
					!model->GetAttribute("FileName") ||
					!model->GetAttribute("Color") ||
					!model->GetAttribute("Opacity") ||
					!model->GetAttribute("Visibility")){
						vtkErrorMacro("Missing attribute in Model");
						return false;
				}
				// Allowed children
				// <!ELEMENT Model (Interface?)> :: Only if is deformation model
				if (vtkStdString(model->GetAttribute("Type")) == "Visualization"){
					IsVisualization ++;
					if (model->GetNumberOfNestedElements() > 0){
						vtkErrorMacro("Found not allowed element nested in Model");
						return false; 
					}
				}
				else if (vtkStdString(model->GetAttribute("Type")) == "Collision"){
					IsCollision ++;
					if (model->GetNumberOfNestedElements() > 0){
						vtkErrorMacro("Found not allowed element nested in Model");
						return false;
					}
				}
				else if (vtkStdString(model->GetAttribute("Type")) == "Deformation"){
					IsDeformation ++;
					// A deformation model must have an interface element
					if (model->GetNumberOfNestedElements() != 1 ||
						!model->FindNestedElementWithName("Interface")){
							vtkErrorMacro("No Interface element could be found in Deformation model");
							return false;
					}
					// Interface
					vtkXMLDataElement *interface = model->FindNestedElementWithName("Interface");
					// Required attributes
					if (!interface->GetAttribute("Type") ||
						!interface->GetAttribute("DeltaT") ||
						!interface->GetAttribute("Gravity") ||
						!interface->GetAttribute("Spring") ||
						!interface->GetAttribute("Damping") ||
						!interface->GetAttribute("Distance") ||
						!interface->GetAttribute("Mass") ||
						!interface->GetAttribute("Rigidity") ||
						!interface->GetAttribute("Solver")){
							vtkErrorMacro("Missing attribute in Interface");
							return false;
					}
				}
			}
			// At least one visualization model. No more than one model of each type.
			if (IsVisualization < 1 || IsVisualization > 1 ||
				IsCollision > 1 || IsDeformation > 1){
					vtkErrorMacro("Missing visualization model or repeated model");
					return false;
			}
		}
	}
	
	// Reached this point everything should be fine!
	return true;
}

//----------------------------------------------------------------------------
void vtkSRMLReader::InternalConstructSimulation()
{
	int i;
	
	this->Simulation = vtkSimulation::New();
	this->Root->GetScalarAttribute("RenderRate",i);
	this->Simulation->SetRenderTimerRate(i);
	
	this->Scenario = vtkScenario::New();
	this->Simulation->SetScenario(this->Scenario);
	
	vtkXMLDataElement *sim = this->Root;
	this->DataPath = sim->GetAttribute("DataPath");

	// Simulation interactor
	if (sim->GetAttribute("Type"))
	{
		vtkInteractorStyle *style = NULL;
		// Put some specific interaction style
		if (vtkStdString(sim->GetAttribute("Type")) == "Laparoscopy")
		{
			style = vtkDefaultInteractorStyle::New();
		}
		else if (vtkStdString(sim->GetAttribute("Type")) == "SingleChannel")
		{
			style = vtkSingleChannelInteractorStyle::New();
		}
		this->Simulation->SetInteractorStyle(style);
	}
	
	// Collisions ON/OFF
	if (sim->FindNestedElementWithName("Collision")){
		this->Simulation->CollisionOn();
	}
	
	#ifdef VTKESQUI_USE_HAPTICS
		sim->GetScalarAttribute("UseHaptic",i);
		if (i == 1)
		{
			// Haptics
			vtkXMLDataElement *haptic = sim->FindNestedElementWithName("Haptic");
			if (haptic){
				vtkHaptic *h = NULL;
				#ifdef VTKESQUI_USE_LSW
				if (vtkStdString(haptic->GetAttribute("Type")) == "LSW") h = vtkLSW::New();
				else if (vtkStdString(haptic->GetAttribute("Type")) == "LSWTool") h = vtkLSWTool::New();
				#endif
				#ifdef VTKESQUI_USE_IHP
				if (vtkStdString(haptic->GetAttribute("Type")) == "IHP") h = vtkIHP::New();
				#endif
				#ifdef VTKESQUI_USE_VSP
				if (vtkStdString(haptic->GetAttribute("Type")) == "VSP") h = vtkVSP::New();
				#endif
				#ifdef VTKESQUI_USE_SBM
				if (vtkStdString(haptic->GetAttribute("Type")) == "SBM") h = vtkSBM::New();
				#endif
				int ntools = 0;
				haptic->GetScalarAttribute("NumberOfTools", ntools);

				if (h){
					h->SetNumberOfTools(ntools);
					this->Simulation->SetHapticDevice(h);
				}
			}
		}
	#endif
	
	// Scenario
	vtkXMLDataElement *sc = sim->FindNestedElementWithName("Scenario");
	if (sc->GetAttribute("Name")){
		this->Scenario->SetName(sc->GetAttribute("Name"));
	}
	
	// Scenario environment
	vtkXMLDataElement *env = sc->FindNestedElementWithName("Environment");
	double *bg = new double[3];
	env->GetVectorAttribute("Background",3,bg);
	this->Scenario->SetBackground(bg[0],bg[1],bg[2]);
	delete[] bg;
	vtkXMLDataElement *camera;
	camera = env->FindNestedElementWithName("Cameras")->GetNestedElement(0);
	this->ImportCamera(camera);
	vtkXMLDataElement *lights = env->FindNestedElementWithName("Lights");
	for (int i=0; i < lights->GetNumberOfNestedElements(); i++){
		vtkXMLDataElement *light = lights->GetNestedElement(i);
		this->ImportLight(light);
	}
	
	// Scenario objects
	vtkXMLDataElement *objs = sc->FindNestedElementWithName("Objects");
	for (int i=0; i < objs->GetNumberOfNestedElements(); i++){
		vtkXMLDataElement *obj = objs->GetNestedElement(i);
		this->ImportObject(obj);
	}
}

void vtkSRMLReader::ImportCamera(vtkXMLDataElement *c){
	vtkCamera *cam = vtkCamera::New();
	double *v = new double[3];
	int i;
	double d;
	
	c->GetVectorAttribute("Position",3,v);
	cam->SetPosition(v[0],v[1],v[2]);
	c->GetVectorAttribute("FocalPoint",3,v);
	cam->SetFocalPoint(v[0],v[1],v[2]);
	delete[] v;
	
	c->GetScalarAttribute("ViewAngle",d);
	cam->SetViewAngle(d);
	c->GetScalarAttribute("Yaw",d);
	cam->Yaw(d);
	c->GetScalarAttribute("Elevation",d);
	cam->Elevation(d);
	c->GetScalarAttribute("Pitch",d);
	cam->Pitch(d);
	c->GetScalarAttribute("Dolly",d);
	cam->Dolly(d);
	c->GetScalarAttribute("ParallelProjection",i);
	cam->SetParallelProjection(i);
	
	this->Scenario->SetCamera(cam);
}

void vtkSRMLReader::ImportLight(vtkXMLDataElement *l){
	vtkLight *light = vtkLight::New();
	double *v = new double[3];
	int i;
	double d;
	
	l->GetVectorAttribute("AmbientColor",3,v);
	light->SetAmbientColor(v[0],v[1],v[2]);
	l->GetVectorAttribute("SpecularColor",3,v);
	light->SetSpecularColor(v[0],v[1],v[2]);
	l->GetVectorAttribute("DiffuseColor",3,v);
	light->SetDiffuseColor(v[0],v[1],v[2]);
	delete[] v;
	
	l->GetScalarAttribute("Intensity",d);
	light->SetIntensity(d);
	l->GetScalarAttribute("ConeAngle",d);
	light->SetConeAngle(d);
	l->GetScalarAttribute("Positional",i);
	light->SetPositional(i);
	
	vtkStdString ltype = vtkStdString(l->GetAttribute("LightType"));
	if (ltype == "Scene") light->SetLightTypeToSceneLight();
	else if (ltype == "Head") light->SetLightTypeToHeadlight();
	else if (ltype == "Camera") light->SetLightTypeToCameraLight();
	
	this->Scenario->AddLight(light);
}

void vtkSRMLReader::ImportObject(vtkXMLDataElement *o){
	vtkScenarioObject *object = NULL;
	
	// Create object
	if (vtkStdString(o->GetAttribute("Type")) == "Tool"){
		// Class
		if (o->GetAttribute("Class")){
			vtkStdString tclass = vtkStdString(o->GetAttribute("Class"));
			vtkStdString tmodel = "";
			if (o->GetAttribute("Model")) tmodel = o->GetAttribute("Model");

			if (tclass == "Laparoscopy")
			{
				// Laparoscopy object
				if (tmodel == "Probe") object = vtkToolProbe::New();
				else if (tmodel == "Grasper") object = vtkToolGrasper::New();
				else if (tmodel == "Scissors") object = vtkToolScissors::New();
				//else if (tmodel == "Hook") object = vtkToolHook::New(); // TODO ?
				else if (tmodel == "Dummy") object = vtkToolDummy::New();
				else object = vtkToolLaparoscopy::New();
			}
			else if (tclass == "Endovascular")
			{
				// Endovascular object
				if (tmodel == "Catheter") object = vtkToolCatheter::New();
				else object = vtkToolEndovascular::New();
			}
			//else if (tclass == "Arthroscopy");
			//else if (tclass == "Camera");
			else if (tclass == "SingleChannel")
			{
				// Single channel object
				// Just one model by now
				vtkToolSingleChannel *tool = vtkToolSingleChannel::New();
				if (tmodel == "Cauterizer") tool->SetToolModel(vtkToolSingleChannel::Cauterizer);
				else if (tmodel == "Brush") tool->SetToolModel(vtkToolSingleChannel::Brush);
				else if (tmodel == "Cutter") tool->SetToolModel(vtkToolSingleChannel::Cutter);
				else if (tmodel == "Camera") tool->SetToolModel(vtkToolSingleChannel::Camera);
				object = tool;
			}
		}
		else object = vtkTool::New();
	}
	else object = vtkOrgan::New();
	
	if (o->GetAttribute("Name")){
		object->SetName(o->GetAttribute("Name"));	
	}
	object->SetStatus(1);
	
	// Import elements
	vtkXMLDataElement *elements = o->FindNestedElementWithName("Elements");
	for (int i=0; i < elements->GetNumberOfNestedElements(); i++){
		vtkXMLDataElement *element = elements->GetNestedElement(i);
		this->ImportElement(object, element);
	}
	
	// Add object
	this->Scenario->AddObject(object);
}

void vtkSRMLReader::ImportElement(vtkScenarioObject *object, vtkXMLDataElement *e){
	vtkScenarioElement *element = vtkScenarioElement::New();
	double *v = new double[3];
	
	// Attributes
	if (e->GetAttribute("Name")) element->SetName(e->GetAttribute("Name"));
	e->GetVectorAttribute("Scale",3,v);
	element->SetScale(v[0],v[1],v[2]);
	e->GetVectorAttribute("Origin",3,v);
	element->SetOrigin(v[0],v[1],v[2]);
	e->GetVectorAttribute("Position",3,v);
	element->SetPosition(v[0],v[1],v[2]);
	e->GetVectorAttribute("Orientation",3,v);
	element->SetOrientation(v[0],v[1],v[2]);
	delete[] v;
	
	// Import models
	vtkXMLDataElement *models = e->FindNestedElementWithName("Models");
	for (int i=0; i < models->GetNumberOfNestedElements(); i++){
		vtkXMLDataElement *model = models->GetNestedElement(i);
		this->ImportModel(element, model);
	}
	
	// Add element
	object->AddElement(element);
}

void vtkSRMLReader::ImportModel(vtkScenarioElement *element, vtkXMLDataElement *m){
	vtkModel *model = NULL;
	double *v = new double[3];
	double d;
	int i;
	
	// Create model
	if (vtkStdString(m->GetAttribute("Type")) == "Visualization"){
		vtkVisualizationModel *vis = vtkVisualizationModel::New();
		if (m->GetAttribute("TextureFileName")){
			vtkStdString tfn = this->DataPath + m->GetAttribute("TextureFileName");
			vis->SetTextureFileName(tfn);
		}
		model = vis;
	}
	else if (vtkStdString(m->GetAttribute("Type")) == "Collision"){
		model = vtkCollisionModel::New();
	}
	else{ // Deformation
		vtkXMLDataElement *dm = m->FindNestedElementWithName("Interface");
		vtkParticleSpringSystemInterface *pss = vtkParticleSpringSystemInterface::New();
		dm->GetVectorAttribute("Gravity",3,v);
		pss->SetGravity(v[0],v[1],v[2]);
		dm->GetScalarAttribute("DeltaT",d);
		pss->SetTimeStep(d);
		dm->GetScalarAttribute("Spring",d);
		pss->SetSpring(d);
		dm->GetScalarAttribute("Damping",d);
		pss->SetDamping(d);
		dm->GetScalarAttribute("Distance",d);
		pss->SetDistance(d);
		dm->GetScalarAttribute("Mass",d);
		pss->SetMass(d);
		model = pss;
	}
	
	// Set general attributes
	if (m->GetAttribute("Name")) model->SetName(m->GetAttribute("Name"));
	vtkStdString fn = this->DataPath + m->GetAttribute("FileName");
	model->SetFileName(fn);
	m->GetScalarAttribute("Visibility",i);
	model->SetVisibility(i != 0);
	m->GetScalarAttribute("Opacity",d);
	model->SetOpacity(d);
	m->GetVectorAttribute("Color",3,v);
	model->SetColor(v[0],v[1],v[2]);
	delete[] v;
	
	// Add model
	if (vtkStdString(m->GetAttribute("Type")) == "Visualization"){
		element->SetVisualizationModel(vtkVisualizationModel::SafeDownCast(model));
	}
	else if (vtkStdString(m->GetAttribute("Type")) == "Collision"){
		element->SetCollisionModel(vtkCollisionModel::SafeDownCast(model));
	}
	else{
		element->SetDeformationModel(vtkDeformationModel::SafeDownCast(model));
	}
}
