#ifndef __vtkContactCollection_h
#define __vtkContactCollection_h

#include "vtkCollection.h"
#include "vtkContact.h"
#include "vtkESQuiCommonWin32Header.h"

//! Collection of contact information
class VTK_ESQUI_COMMON_EXPORT vtkContactCollection: public vtkCollection
{
 public:
  static vtkContactCollection *New();
  vtkTypeRevisionMacro(vtkContactCollection,vtkCollection);
  virtual void PrintSelf(ostream& os, vtkIndent indent);

  //!Perform a deep copy of the vtkContactCollection Object
  /*!
   * \param collection collection of contacts to be copied.
   */
  void DeepCopy(vtkContactCollection *collection);

  //! Add a Contact to the list.
  /*!
   * \param contact contact to be added to the collection
   * \sa InsertContact(vtkIdType id, vtkContact *contact)
   */
  void InsertNextContact(vtkContact *contact);

  //! Replace the Contact on id position.
  /*!
   * \param id contact identifier on the collection
   * \param contact contact to be added to the collection
   * \sa InsertNextContact(vtkContact *contact)
   */
  void InsertContact(vtkIdType id, vtkContact *contact);

  //! Check if the contact exists within the collection
  /*!
   * \param contact contact to be located in the collection
   */
  vtkIdType ContainsContact(vtkContact * contact);

  //! Return the Contact on id position.
  /*!
   * \id identifier of the contact that will be returned
   * \return vtkContact object stored in the collection
   */
  vtkContact * GetContact(vtkIdType id);

  //! Return iteratively each contact
  /*!
   * \return vtkContact object stored in the collection
   */
  vtkContact * GetNextContact();

  //! Remove the Contact at the id position
  /*!
   * This method only removes the contact from the collection.
   * The object itself must be deleted.
   */
  void RemoveContact(vtkIdType id);

  //! Remove All Contacts.
  /*!
   * This method only removes the contact from the collection.
   * The object itself must be deleted.
   */
  void RemoveContacts();

protected:
  vtkContactCollection() {};
  ~vtkContactCollection() {};


private:
  vtkContactCollection(const vtkContactCollection&);  // Not implemented.
  void operator=(const vtkContactCollection&);				// Not implemented.
};


#endif
