#ifndef __vtkOrganCollection_h
#define __vtkOrganCollection_h

#include "vtkCollection.h"
#include "vtkESQuiScenarioWin32Header.h"
#include "vtkOrgan.h"

//!  Collection of organs
/*! 
vtkOrganCollection represents and provides methods to manipulate a list of
Organs (i.e., vtkOrgan and subclasses). The list is unsorted and duplicate
entries are not prevented.
\sa vtkCollection, vtkOrgan
*/
class VTK_ESQUI_SCENARIO_EXPORT vtkOrganCollection : public vtkCollection
{
public:
	//!Create new organism object
	static vtkOrganCollection *New();
	//!Type Revision Macro
	vtkTypeRevisionMacro(vtkOrganCollection,vtkCollection);
	//!Print Class Values
	virtual void PrintSelf(ostream& os, vtkIndent indent);

	//!Insert an organ to the collection
	/*!
	* The organ will be inserted at the end of the list
	* \param organ vtkOrgan object to be inserted
	* \sa InsertOrgan(vtkIdType id, vtkOrgan *organism)
	*/
	void InsertNextOrgan(vtkOrgan *organ);

	//!Insert an organ at the specified index
	/*!
	* \param id index of the list where the organ will be inserted
	* \param organ vtkOrgan object to be inserted
	* \sa InsertNextOrgan(vtkOrgan *organ)
	*/
	void InsertOrgan(vtkIdType id, vtkOrgan *organ);

	//!Return an organ in specified position of the list.
	/*!
	* 	Get the organ in the list with the position id. NULL is returned when the collection is exhausted.
	* \param id index of the organ to be returned
	*/
	vtkOrgan *GetOrgan(vtkIdType id);

	//!Return the next organism on the list.
	/*!
	* Get the organ at the pointer position. Beware of calling InitTraversal() to init the pointer. NULL is returned when the collection is exhausted.
	*/
	vtkOrgan * GetNextOrgan();


protected:
	vtkOrganCollection() {};
	~vtkOrganCollection() {};

private:
	vtkOrganCollection(const vtkOrganCollection&);  // Not implemented.
	void operator=(const vtkOrganCollection&);					// Not implemented.
};

#endif
