
#ifndef __vtkObjectTemplate_h
#define __vtkObjectTemplate_h

#include "vtkESQuiCommonWin32Header.h"
#include "vtkObject.h"
#include "vtkObjectFactory.h"

class VTK_ESQUI_COMMON_EXPORT vtkObjectTemplate : public vtkObject {
public:
	vtkTypeRevisionMacro(vtkObjectTemplate,vtkObject);

	static vtkObjectTemplate *New();
	void PrintSelf(ostream& os, vtkIndent indent);
	const char *GetClassName() {return "vtkObjectTemplate";};

protected:
	vtkObjectTemplate();
	~vtkObjectTemplate();

private:

	vtkObjectTemplate (const vtkObjectTemplate &);//NotImplemented
	void operator =(const vtkObjectTemplate &);//Not Implemented
}

#endif
