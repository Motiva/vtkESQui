#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkPoints.h"
#include "vtkIdList.h"
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"
#include "vtkSmartPointer.h"

#include "vtkCollisionModel.h"

int TestvtkCollisionModel(int argc, char * argv[]){

	const char * fnc ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball_col.vtp";

	vtkSmartPointer<vtkCollisionModel> model = vtkSmartPointer<vtkCollisionModel>::New();
	model->SetFileName(fnc);
	model->SetPosition(1.0, 2.0, 0.5);
	model->SetOrientation(10,0,0);
	model->SetOpacity(0.5);
	model->SetColor(0,1,1);
	model->SetRadius(0.05);
	model->Init();

	// A call to update method is made to force the model to be at its last state
	model->Update();

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->SetSize(500,500);
	renWin->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	renderer->AddActor(model->GetActor());
	renderer->SetBackground(1,1,1);

	renderer->ResetCamera();
	iren->Initialize();

	renWin->Render();

	iren->Start();

	return 0;
}

