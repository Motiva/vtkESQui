#ifndef __UGridAccelerator_h__
#define __UGridAccelerator_h__

#include "vtkUnstructuredGrid.h"
// provides some methods for fast point and cell access in an vtkUnstrucutredGrid
// only works with unstructuredGrids that contain a double points array
class UGridAccelerator {
public:
    UGridAccelerator(void);
    UGridAccelerator(vtkUnstructuredGrid* grid);
    ~UGridAccelerator(void);
    void SetGrid(vtkUnstructuredGrid* grid);
    //! super fast array access to the points of the grid
    inline double* getPoint(unsigned int id) const {
        return this->points + (id*3);
    }
    /*!
    * Super fast access to the ith point of the ith cell of the grid
    * \param cellId id of the desired cell
    * \param point the ith point of the cell (range 0 - 3)
    * \return pointer to a double[3] which holds the desired point
    */
    inline const double* getCellPoint(vtkIdType cellId, vtkIdType point) const {
        return getPoint(getCellPointId(cellId, point));
    }
    inline vtkIdType getCellPointId(vtkIdType cellId, vtkIdType point) const {
        return connectivity[cellId * 5 + 1 + point];
    }
    inline vtkIdType getNumberOfPoints() const {
        return this->grid->GetNumberOfPoints();
    }
    inline vtkIdType getNumberOfCells() const {
        return this->grid->GetNumberOfCells();
    }
    inline vtkUnstructuredGrid* getGrid() const {
        return this->grid;
    }
    double* getPointsArray() const {
        return this->points;
    }
private:
    void initPointers(vtkUnstructuredGrid* grid);
    vtkUnstructuredGrid* grid;
    double* points;
    vtkIdType* connectivity;
};

#endif

