#ifndef __vtkMSS_h
#define __vtkMSS_h

#include "vtkMSSConfigure.h" // Include configuration header.
#include "vtkMassSpring.h"
#include "vtkIdList.h"
#include "vtkDoubleArray.h"

//! This class inherits vtkMassSpring class contained in the mss_vtk package
class VTK_vtkMSS_EXPORT vtkMSS : public vtkMassSpring
{
public:
  vtkTypeRevisionMacro(vtkMSS, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);
  static vtkMSS* New();

  //! Set the external forces to apply on each point of the mesh according to ids input parameter
  /*!
   * The number of tuples (size=3) of the the forces should match the number of ids
   * \param ids list of organ mesh point ids that has been contacted
   * \param forces vector containing the force in each cartesian component x, y & z
   */
  void SetContacts(vtkIdList * ids, vtkDoubleArray * forces);

protected:
  vtkMSS();
  ~vtkMSS();

private:
  vtkMSS(const vtkMSS&);  // Not implemented.
  void operator=(const vtkMSS&);  // Not implemented.
};

#endif
