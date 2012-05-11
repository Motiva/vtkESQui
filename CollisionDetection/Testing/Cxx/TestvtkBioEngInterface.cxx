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
#include "vtkCollisionModel.h"
#include "vtkCollisionCollection.h"
#include "vtkBioEngInterface.h"

class TimerCallback1 : public vtkCommand
{
  public:
    static TimerCallback1 * New()
    {
      TimerCallback1 *cb = new TimerCallback1;
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
         
          double * p0 = this->Model0->GetActor()->GetPosition();
          double * p1 = this->Model1->GetActor()->GetPosition();
          this->Model0->GetActor()->SetPosition(p0[0]+.1,p0[1],p0[2]);
          this->Model1->GetActor()->SetPosition(p1[0]-.1,p1[1],p1[2]);

          this->Filter->Update();

          int n = this->Filter->GetNumberOfCollisionPairs();
          int p[2];
          for (int i = 0; i<n; i++)
          {
            this->Filter->GetCollisionPair(i, p);
            cout << "Model\t: " << p[0] << "\t: " << p[1] << endl;
            cout << "Colls\t: " << this->Model0->GetNumberOfCollisions() << "\t: " << this->Model1->GetNumberOfCollisions() << endl;
            cout << "-----------------------" << endl;
          }
          //if(n>0) 
          //{
            //std::cout << n <<" contactcells"<< endl;
            /*vtkPolyData * o0 = this->Filter->GetOutput(0);
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
            }*/
          //}
           
          iren->Render();
        }
      }
    }

    void SetTimerId(int id){
      this->TimerId = id;
    }

    void SetModel(int id, vtkCollisionModel * a)
    {
      if (id == 0) this->Model0 = a;
      else this->Model1 = a;
    }

    void SetFilter(vtkBioEngInterface *f)
    {
      this->Filter = f;
    }
  
  private:
    vtkCollisionModel * Model0;
    vtkCollisionModel * Model1;
    vtkBioEngInterface * Filter;
    int TimerId;

};

int TestvtkBioEngInterface(int argc, char * argv[]){
  //Data input

  vtkSmartPointer<vtkSphereSource> src0 = vtkSmartPointer<vtkSphereSource>::New();
  src0->SetRadius(1);
  src0->SetThetaResolution(12);
  src0->SetPhiResolution(12);
  src0->Update();

  vtkPolyData * s0 = src0->GetOutput();

  vtkSmartPointer<vtkSphereSource> src1 = vtkSmartPointer<vtkSphereSource>::New();
  src1->SetRadius(0.5);
  src1->SetThetaResolution(12);
  src1->SetPhiResolution(12);
  src1->Update();

  vtkPolyData * s1 = src1->GetOutput();
  
  vtkSmartPointer<vtkCollisionModel> col0 = vtkSmartPointer<vtkCollisionModel>::New(); 
  col0->SetName("col0");
  col0->SetInput(s0);
  col0->SetObjectId(0);
  col0->SetId(0);
  col0->Update();
  col0->GetActor()->SetPosition(0,0,0);

  vtkSmartPointer<vtkCollisionModel> col1 = vtkSmartPointer<vtkCollisionModel>::New(); 
  col1->SetName("col1");
  col1->SetInput(s1);
  col1->SetObjectId(1);
  col1->SetId(0);
  col1->Update();
  col1->GetActor()->SetPosition(3,0,0);

  //Rendering objects
  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

  vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
  renWin->SetSize(500,500);
  renWin->AddRenderer(renderer);

  vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren->SetRenderWindow(renWin);

  renderer->AddActor(col0->GetActor());
  renderer->AddActor(col1->GetActor());

  //Collision Filter
  vtkSmartPointer<vtkBioEngInterface> col = vtkSmartPointer<vtkBioEngInterface>::New();
  col->SetBoxTolerance(0.0);
  col->SetCellTolerance(0.0);
  col->SetNumberOfCellsPerNode(2);
  col->SetMode(vtkCollisionDetection::Full);
  col->Initialize();

  col->AddModel(col0);
  col->AddModel(col1);
  col->Update();
  
  /*vtkSmartPointer<vtkPolyDataMapper> m2 = vtkSmartPointer<vtkPolyDataMapper>::New();
  m2->SetInputConnection(col->GetOutputPort());

  vtkSmartPointer<vtkActor> a2 = vtkSmartPointer<vtkActor>::New();
  a2->SetMapper(m2);
  renderer->AddActor(a2);*/

  renderer->SetBackground(.3,.6,.3);

  renderer->ResetCamera();
  iren->Initialize();
  iren->CreateRepeatingTimer(500);

  TimerCallback1 * cb = TimerCallback1::New();
  iren->AddObserver( vtkCommand::TimerEvent, cb);

  cb->SetModel(0, col0);
  cb->SetModel(1, col1);
  cb->SetFilter(col);

  int tid = iren->CreateRepeatingTimer(500);
  cb->SetTimerId(tid);
  
  renWin->Render();

  iren->Start();

  return 0;
}

