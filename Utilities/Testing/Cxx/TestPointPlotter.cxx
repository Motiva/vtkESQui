#include <iostream>
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkInteractorStyleTrackballCamera.h"

#include "vtkPointPlotter.h"
#include "vtkMath.h"

using namespace std;

#if defined(WIN32)
#include <time.h> 
#endif

//!This test perform a standard execution of the vtkPointPlotter class

int TestPointPlotter(int argc, char * argv[])
{
	/**********  Render Window Definitions  ********/
	vtkRenderer *ren1= vtkRenderer::New();
	ren1->SetBackground(0.90, 0.95, 0.95);

	vtkPointPlotter * plotter = vtkPointPlotter::New();
	plotter->SetRadius(0.025);
	plotter->SetResolution(16);
	plotter->SetRenderer(ren1);
	plotter->Init();

	// Generate some random line and random points to plot
	vtkMath::RandomSeed(time(0));
	double p[3];
	for (int i=0;i<1000;i++)
	{
	    for (int j=0;j<3;j++)
	    {
	    	p[j] = vtkMath::Random(-1,1);
	    }
	    plotter->InsertPoint(p[0],p[1],p[2], 128*p[0], 128*p[1], 128*p[2]);
	}
	plotter->Update();
	
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren1);
	renWin->SetSize(840,480);

	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	renWin->SetInteractor(iren);

	// Create my interactor style
	vtkInteractorStyleTrackballCamera* style = vtkInteractorStyleTrackballCamera::New();
	iren->SetInteractorStyle( style );

	iren->Initialize();
	iren->Start();

	return 0;
}


