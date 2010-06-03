#ifndef __vtkToolCollection_h
#define __vtkToolCollection_h

//#include "Macros.h"
#include "vtkCollection.h"
#include "vtkTool.h"
#include "vtkESQuiScenarioWin32Header.h"

//#ifdef _WIN32
#include <vector>
//#else
//#include <vector.h>
//#endif

//! Collection of tools 
class VTK_ESQUI_SCENARIO_EXPORT vtkToolCollection : public vtkCollection
{
public:
	static vtkToolCollection *New() {return new vtkToolCollection;}
	vtkTypeRevisionMacro(vtkToolCollection,vtkCollection);
	const char *GetClassName() {return "vtkToolCollection";}
	virtual void PrintSelf(ostream& os, vtkIndent indent);

	//!Insert tool into the collection at the specified id
	/*!
	* \param tool vtkTool object to be inserted
	*/
	void InsertTool(vtkIdType id, vtkTool * tool);
	//!Insert tool at the last position of the collection
	/*!
	* \param id identifier key of the tool
	* \param tool vtkTool object to be inserted
	*/
	void InsertNextTool(vtkTool * tool);
	//!Return tool at the specified id
	/*!
	* \param id identifier key of the tool
	*/
	vtkTool * GetTool(vtkIdType id);
	//!Return the next tool
	/*!
	* Return the tool located at the pointer position + 1
	* \sa GetTool(vtkIdType id)
	*/
	vtkTool * GetNextTool();

protected:
	//   vtkIdType NumeroDeHerramientas;
	vtkToolCollection() {};
	~vtkToolCollection() {};

private:
	vtkToolCollection(const vtkToolCollection&);  // Not implemented.
	void operator=(const vtkToolCollection&);  // Not implemented.
};


#endif
