#ifndef __vtkPieceCollection_h
#define __vtkPieceCollection_h

#include "vtkCollection.h"
#include "vtkPiece.h"
#include "vtkESQuiScenarioWin32Header.h"

//!Collection of Pieces
class VTK_ESQUI_SCENARIO_EXPORT vtkPieceCollection : public vtkCollection
{
public:
	vtkTypeRevisionMacro(vtkPieceCollection,vtkCollection);
	static vtkPieceCollection *New() {return new vtkPieceCollection;}
	const char *GetClassName() {return "vtkPieceCollection";}
	virtual void PrintSelf(ostream& os, vtkIndent indent);

	//!Insert a piece into the collection
	/*!
	* Piece will be added at the specified id
	* \param id index of the list where the piece will be added
	* \param Piece vtkPiece object to be inserted
	* \sa InsertNextPiece(vtkPiece * Piece);
	*/
	void InsertPiece(vtkIdType id, vtkPiece * Piece);

	//!Insert piece at the end of the collection
	/*!
	* \param Piece vtkPiece object to be inserted
	* \sa InsertPiece(vtkIdType id, vtkPiece * Piece)
	*/
	void InsertNextPiece(vtkPiece * Piece);

	//!Return piece located the specified id
	/*!
	* Get the piece in the list with the position id. NULL is returned when the collection is exhausted.
	* \param id list index where the piece is stored
	*/
	vtkPiece * GetPiece(vtkIdType id);

	//!Return piece on the collection
	/*!
	* Get the piece at the pointer position. Beware of calling InitTraversal() to init the pointer. NULL is returned when the collection is exhausted.
	*/
	vtkPiece * GetNextPiece();

	//!Return number of pieces in the collection
	vtkIdType GetNumberOfPieces();

protected:
	vtkPieceCollection() {};
	~vtkPieceCollection() {};

private:
	vtkPieceCollection(const vtkPieceCollection&);  // Not implemented.
	void operator=(const vtkPieceCollection&);  // Not implemented.
};


#endif
