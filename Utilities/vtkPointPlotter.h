#ifndef __vtkPointPlotter_h
#define __vtkPointPlotter_h
#include "vtkESQuiUtilitiesWin32Header.h"
#include "vtkObject.h"
#include "vtkObjectFactory.h"
#include "vtkPoints.h"
#include "vtkUnsignedCharArray.h"
#include "vtkDataArray.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkGlyph3D.h"
#include "vtkSphereSource.h"
#include "vtkRenderer.h"

//!This class performs the display of a vtkPoints array
class VTK_ESQUI_UTILITIES_EXPORT vtkPointPlotter : public vtkObject {

public:
	vtkTypeRevisionMacro(vtkPointPlotter,vtkObject);

	static vtkPointPlotter *New();

	const char *GetClassName() {return "vtkPointPlotter";};

	void Init();

	void Update();

	void Reset();

	void SetRadius(double radius = 0.025){this->Radius = radius;};

	void SetResolution(int resolution = 12){this->Resolution = resolution;};

	void SetRenderer(vtkRenderer * renderer){this->Renderer = renderer;};

	vtkActor * GetActor(){return this->Actor;};

	vtkPolyData * GetPolyData(){return this->PolyData;};

	void InsertPoint(double x, double y, double z, unsigned char r, unsigned char g, unsigned char b);

protected:
	vtkPointPlotter();
	~vtkPointPlotter();
private:

	vtkPoints * Points;
	vtkUnsignedCharArray * Scalars;

	vtkPolyData * PolyData;
	vtkGlyph3D * Glyphs;
	vtkSphereSource * Spheres;

	vtkActor * Actor;
	vtkPolyDataMapper * Mapper;

	vtkRenderer * Renderer;

	double Radius;
	vtkIdType Resolution;

	vtkPointPlotter (const vtkPointPlotter &);//NotImplemented
	void operator =(const vtkPointPlotter &);//Not Implemented
};
#endif
