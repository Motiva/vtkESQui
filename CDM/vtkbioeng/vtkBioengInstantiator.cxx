#include "vtkBioengInstantiator.h"
  
extern vtkObject* vtkInstantiatorvtkBufferOutputWindowNew();
extern vtkObject* vtkInstantiatorvtkCollisionDetectionFilterNew();
extern vtkObject* vtkInstantiatorvtkRemoveCellsFilterNew();
extern vtkObject* vtkInstantiatorvtkSelectCellsFilterNew();
extern vtkObject* vtkInstantiatorvtkTesselateBoundaryLoopsNew();


  
void vtkBioengInstantiator::ClassInitialize()
{
  
  vtkInstantiator::RegisterInstantiator("vtkBufferOutputWindow", vtkInstantiatorvtkBufferOutputWindowNew);
  vtkInstantiator::RegisterInstantiator("vtkCollisionDetectionFilter", vtkInstantiatorvtkCollisionDetectionFilterNew);
  vtkInstantiator::RegisterInstantiator("vtkRemoveCellsFilter", vtkInstantiatorvtkRemoveCellsFilterNew);
  vtkInstantiator::RegisterInstantiator("vtkSelectCellsFilter", vtkInstantiatorvtkSelectCellsFilterNew);
  vtkInstantiator::RegisterInstantiator("vtkTesselateBoundaryLoops", vtkInstantiatorvtkTesselateBoundaryLoopsNew);

  
}
          
void vtkBioengInstantiator::ClassFinalize()
{ 

  vtkInstantiator::UnRegisterInstantiator("vtkBufferOutputWindow", vtkInstantiatorvtkBufferOutputWindowNew);
  vtkInstantiator::UnRegisterInstantiator("vtkCollisionDetectionFilter", vtkInstantiatorvtkCollisionDetectionFilterNew);
  vtkInstantiator::UnRegisterInstantiator("vtkRemoveCellsFilter", vtkInstantiatorvtkRemoveCellsFilterNew);
  vtkInstantiator::UnRegisterInstantiator("vtkSelectCellsFilter", vtkInstantiatorvtkSelectCellsFilterNew);
  vtkInstantiator::UnRegisterInstantiator("vtkTesselateBoundaryLoops", vtkInstantiatorvtkTesselateBoundaryLoopsNew);

  
}

vtkBioengInstantiator::vtkBioengInstantiator()
{
  if(++vtkBioengInstantiator::Count == 1)
    { 
    vtkBioengInstantiator::ClassInitialize(); 
    }
}

vtkBioengInstantiator::~vtkBioengInstantiator()
{
  if(--vtkBioengInstantiator::Count == 0)
    { 
    vtkBioengInstantiator::ClassFinalize(); 
    }
}

// Number of translation units that include this class's header.
// Purposely not initialized.  Default is static initialization to 0.
unsigned int vtkBioengInstantiator::Count;
