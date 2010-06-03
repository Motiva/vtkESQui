#include "UGridAcceleratorMSS.h"
#include "vtkDoubleArray.h"
#include "vtkCellArray.h"
UGridAcceleratorMSS::UGridAcceleratorMSS(void) {
}
UGridAcceleratorMSS::UGridAcceleratorMSS(vtkUnstructuredGrid* grid) {
    this->SetGrid(grid);
}
UGridAcceleratorMSS::~UGridAcceleratorMSS(void) {
}
void UGridAcceleratorMSS::SetGrid(vtkUnstructuredGrid* grid) {
    this->grid = grid;
    this->initPointers(grid);
}
void UGridAcceleratorMSS::initPointers(vtkUnstructuredGrid* grid) {
    // check for using correct data type
    // TODO: maybe we can make this templated such that it works for any datatype
    if (grid->GetPoints()->GetData()->GetDataType() != VTK_DOUBLE) {
        cout << "Error, vtkUnstructuredGrid uses wrong data type, VTK_DOUBLE is required!" << endl;
        return;
    }
    // get direct access to the points of the collide
    vtkPoints* p = this->grid->GetPoints();
    vtkDoubleArray* pointArray = (vtkDoubleArray*)p->GetData();
    this->points = pointArray->GetPointer(0);
    // get direkt access to the cells
    vtkCellArray* cells = this->grid->GetCells();
    this->connectivity = cells->GetPointer();
}

