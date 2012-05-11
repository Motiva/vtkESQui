#include "vtkSphereSource.h"
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
#include "vtkCollisionDetectionFilter.h"
#include "vtkCommand.h"
#include "vtkCallbackCommand.h"

class TimerCallback : public vtkCommand
{
  public:
    static TimerCallback * New()
    {
      TimerCallback *cb = new TimerCallback;
      cb->TimerId = 0;
      return cb;
    }

    virtual void Execute(vtkObject* caller, long unsigned int eventId, void* callData )
    {
      if(vtkCommand::TimerEvent == eventId){
        int tid = * static_cast<int *>(callData);
        if (tid == this->TimerId)
        {
          vtkRenderWindowInteractor *iren = 
            static_cast<vtkRenderWindowInteractor*>(caller);
         
          double * p0 = this->Actor0->GetPosition();
          double * p1 = this->Actor1->GetPosition();
          this->Actor0->SetPosition(p0[0]+.1,p0[1],p0[2]);
          this->Actor1->SetPosition(p1[0]-.1,p1[1],p1[2]);

          this->Filter->Modified();
          this->Filter->Update();

          int n = this->Filter->GetNumberOfContacts();
          if(n>0) 
          {
            std::cout << n <<" contactcells"<< endl;
            vtkPolyData * o0 = this->Filter->GetOutput(0);
            vtkPolyData * o1 = this->Filter->GetOutput(1);
            vtkPolyData * contacts = this->Filter->GetContactsOutput();
            
            vtkIdTypeArray * id0 = this->Filter->GetContactCells(0);
            vtkIdTypeArray * id1 = this->Filter->GetContactCells(1);
            
            vtkSmartPointer<vtkIdList> pids0 = vtkSmartPointer<vtkIdList>::New();
            vtkSmartPointer<vtkIdList> pids1 = vtkSmartPointer<vtkIdList>::New();
            
            for (int i = 0; i< id0->GetNumberOfTuples();i++)
            {
              int cid0 = id0->GetValue(i);
              int cid1 = id1->GetValue(i);
              std::cout << cid0 << ":" << cid1 << endl;
              double * p0 = contacts->GetPoint(2*i);
              double * p1 = contacts->GetPoint(2*i+1);
              std::cout <<p0[0]<<","<<p0[1]<<","<<p0[2]<<":"<<p1[0]<<","<<p1[1]<<","<<p1[2]<<"\n";
              std::cout << "------------------\n";

              /*o0->GetCellPoints(cid0,pids0);
              o1->GetCellPoints(cid1,pids1);
              for(int j = 0; j < 3; j++)
              {
                std::cout << pids0->GetId(j) << ":" << pids1->GetId(j) << endl;
              }
              std::cout << "\n";*/
            }
          }
           
          iren->Render();
        }
      }
    }

    void SetTimerId(int id){
      this->TimerId = id;
    }

    void SetActor(int id, vtkActor * a)
    {
      if (id == 0) this->Actor0 = a;
      else this->Actor1 = a;
    }

    void SetFilter(vtkCollisionDetectionFilter *f)
    {
      this->Filter = f;
    }
  
  private:
    vtkActor * Actor0;
    vtkActor * Actor1;
    vtkCollisionDetectionFilter * Filter;
    int TimerId;

};

int TestvtkCollisionDetectionFilter(int argc, char * argv[]){

  //Data input

  vtkSmartPointer<vtkSphereSource> src0 = vtkSmartPointer<vtkSphereSource>::New();
  src0->SetRadius(1);
  src0->SetThetaResolution(12);
  src0->SetPhiResolution(12);
  src0->Update();

  vtkPolyData * s0 = src0->GetOutput();

  vtkSmartPointer<vtkSphereSource> src1 = vtkSmartPointer<vtkSphereSource>::New();
  src1->SetRadius(0.5);
  src1->SetThetaResolution(8);
  src1->SetPhiResolution(8);
  src1->Update();

  vtkPolyData * s1 = src1->GetOutput();

  //Rendering objects

  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

  vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
  renWin->SetSize(500,500);
  renWin->AddRenderer(renderer);

  vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren->SetRenderWindow(renWin);

  //Create mappers and actors for each input

  vtkSmartPointer<vtkPolyDataMapper> m0 = vtkSmartPointer<vtkPolyDataMapper>::New();
  m0->SetInput(s0);

  vtkSmartPointer<vtkActor> a0 = vtkSmartPointer<vtkActor>::New();
  a0->SetMapper(m0);
  
  vtkSmartPointer<vtkPolyDataMapper> m1 = vtkSmartPointer<vtkPolyDataMapper>::New();
  m1->SetInput(s1);

  vtkSmartPointer<vtkActor> a1 = vtkSmartPointer<vtkActor>::New();
  a1->SetMapper(m1);
  a1->SetPosition(2,0,0);

  renderer->AddActor(a0);
  renderer->AddActor(a1);

  //Collision Filter
  vtkSmartPointer<vtkCollisionDetectionFilter> col = vtkSmartPointer<vtkCollisionDetectionFilter>::New();
  col->SetBoxTolerance(0.01);
  col->SetCellTolerance(0);
  col->SetNumberOfCellsPerNode(2);

  col->SetInput(0, s0);
  col->SetInput(1, s1);
  col->SetMatrix(0, a0->GetMatrix());
  col->SetMatrix(1, a1->GetMatrix());
  col->GenerateScalarsOn();
  
  col->Update();

  vtkSmartPointer<vtkPolyDataMapper> m2 = vtkSmartPointer<vtkPolyDataMapper>::New();
  m2->SetInputConnection(col->GetOutputPort());

  vtkSmartPointer<vtkActor> a2 = vtkSmartPointer<vtkActor>::New();
  a2->SetMapper(m2);
  renderer->AddActor(a2);

  renderer->SetBackground(.3,.6,.3);

  renderer->ResetCamera();
  iren->Initialize();
  iren->CreateRepeatingTimer(500);

  TimerCallback * cb = TimerCallback::New();
  iren->AddObserver( vtkCommand::TimerEvent, cb);

  cb->SetActor(0, a0);
  cb->SetActor(1, a1);
  cb->SetFilter(col);

  int tid = iren->CreateRepeatingTimer(500);
  cb->SetTimerId(tid);
  
  renWin->Render();

  iren->Start();

  return 0;
}

