/*==========================================================================
--------------------------------------------------
Program: ESQUI
Info and Bugs: {marf,jballesteros}@itccanarias.org
url: http://motivando.me 
--------------------------------------------------

Copyright (c) 2006-2007, Center for Technology in Medicine (CTM), 
University of Las Palmas de Gran Canaria (ULPGC), Canary Islands, Spain.
Copyright (c) 2007-2010, Institute of Technology at CanaryIslands (ITC),
Canary Islands, Spain.

This software is free software; you can redistribute it and/or modify it 
under the terms of the GNU Lesser General Public License (LGPL) as published
by the Free Software Foundation, either version 3 of the License, or (at 
your option) any later version.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1) Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2) Redistributions in binary form must reproduce the above copyright 
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

You should have received a copy of the GNU Lesser General Public License 
along with this program.  If not, see <http://www.gnu.org/licenses/>.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.
==========================================================================*/
#include "vtkLSWTool.h"
#include "vtkObjectFactory.h"

#include <iostream>
using namespace std;

vtkCxxRevisionMacro(vtkLSWTool, "$Revision: 0.1 $");
vtkStandardNewMacro( vtkLSWTool);

//--------------------------------------------------------------------------
vtkLSWTool::vtkLSWTool()
{
	this->PointForceOrige.x = this->PointForceOrige.y = this->PointForceOrige.z
			= 0;
	this->ForceIncrement.x = this->ForceIncrement.y = this->ForceIncrement.z
			= 0;
	this->ActForce.x = this->ActForce.y = this->ActForce.z = 0;
	this->PrevForce.x = this->PrevForce.y = this->PrevForce.z = 0;
	this->PrevPoint.x = this->PrevPoint.y = this->PrevPoint.z = 0;

	vtkLSWDataTool* Data;
	Data = new vtkLSWDataTool;
	Data->ContactFlag = false;
	Data->OType = 0;
	Data->InOut = Data->LftRgt = Data->UpDwn = 0;
	Data->CTE_X = Data->CTE_Y = Data->CTE_Z = 0;

	this->Data = Data;
	this->ActualForceValue.x = 0;
	this->ActualForceValue.y = 0;
	this->ActualForceValue.z = 0;

	this->ForceValue.x = 0;
	this->ForceValue.y = 0;
	this->ForceValue.z = 0;

	this->ForceStep.x = 0;
	this->ForceStep.y = 0;
	this->ForceStep.z = 0;
}

//--------------------------------------------------------------------------
vtkLSWTool::~vtkLSWTool()
{
}

//--------------------------------------------------------------------------
void vtkLSWTool::PrintSelf(ostream& os, vtkIndent indent)
{

	//this->Superclass::PrintSelf(os,indent);

}

//--------------------------------------------------------------------------
bool vtkLSWTool::ToolAssign(ImmSWTool* Tool)
{
	if (!Tool)
		return false;
	//	if (!this->Debug)
	this->LSWTool = Tool;
	return true;
}
;

//--------------------------------------------------------------------------
void vtkLSWTool::Initialize()
{
	//	if (!this->Debug){
	//Hay que inicializar la comunicaci�n con el simulador de ESQUI

	//	else
	//Estamos usando el modo de depuraci�n (STAND ALONE)
	//	};
}

//--------------------------------------------------------------------------
double vtkLSWTool::GetToolDepth()
{
	double Depth;
	this->LSWTool->GetInsertionDepth(&Depth);
	return Depth;
}

//--------------------------------------------------------------------------
double vtkLSWTool::GetToolPitch()
{
	double Pitch;
	this->LSWTool->GetPitch(&Pitch);
	return Pitch;
}

//--------------------------------------------------------------------------
double vtkLSWTool::GetToolYaw()
{

	double Yaw;
	this->LSWTool->GetYaw(&Yaw);
	return Yaw;
}

//--------------------------------------------------------------------------
double vtkLSWTool::GetToolOpening()
{
	double Aperture;
	this->LSWTool->GetGrippers(&Aperture);
	return Aperture;
}

//--------------------------------------------------------------------------
double vtkLSWTool::GetToolRoll()
{
	double TwistTip;
	double TwistHandle;
	this->LSWTool->GetHandleTwist(&TwistHandle);
	this->LSWTool->GetTipTwist(&TwistTip);
	// Se suman para obtener la rotacion total
	return (TwistTip + TwistHandle);
}

//--------------------------------------------------------------------------
void vtkLSWTool::Refresh()
{
	if (this->Data->NewData)
	{
		if (this->Data->ContactFlag)
			this->Contact = true;
		else
			this->Contact = false;
		switch (this->Data->OType)
		{
		case vtk_WITHOUTCONTACT: // Sin contacto, las fuerzas de realimentaci�n son nulas
			this->FirsContact = true;
			this->PrevForce = ActForce;
			this->ActForce.x = 0;
			this->ActForce.y = 0;
			this->ActForce.z = 0;
			this->LSWTool->GetPosition(&PrevPoint);
			this->ForceIncrementCalculate();
		case vtk_MSGRESET:
			break;
		case vtk_MSGPAUSE:
			break;
		case vtk_CONTACTDEFORM:
			PrevForce = ActForce; // Almacena el valor en (t-1)
			// Calcula la fuerza que se debe aplicar en funci�n de
			//    la constante k = anPakete[11]
			if (!this->Contact || this->FirsContact)
			{
				this->FirsContact = false; // Desactiva la bandera de control
				this->LSWTool->GetPosition(&PointForceOrige); // Obtiene el punto de origen para los c�lculos
			}
			else
			{
				// Se calcula el aumento en el desplazamiento producido
				this->LSWTool->GetPosition(&PrevPoint);
			}
			this->DisplacementCalculate(); // Calcula el desplazamiento entre punto origen y punto actual
			// Asigna los nuevos valores de fuerza. En tiempo (t)
			this->ForceCalculate(); // Calcula la fuerza en base a la ley de Hooke: F = k�e
			this->ForceIncrementCalculate();
			break;
		case vtk_CONTACTVOX:
			break;
		case vtk_CONTACTSCENE:
			break;
		case vtk_CALIBRATION:
			//activarCalibracion();
			break;
		} // fin del switch(tipo)
		this->Data->NewData = false;
	}
	this->ForceUpdate(); // Actualiza las fuerzas de realimentaci�n aplicadas a las herramientas de la LSW
}

//--------------------------------------------------------------------------
void vtkLSWTool::ForceUpdate()
{
	// Calcula la fuerza a aplicar en este instante
	//   Intenta suavizar el valor calculado entre dos instante (t-1) y (t) aplicando
	//   un incremento gradual entre los dos valores enviados por el simulador, para 
	//   evitar discontinuidades en las fuerzas de realimentaci�n aplicadas
	this->PrevForce.x += this->ForceIncrement.x;
	this->PrevForce.y += this->ForceIncrement.y;
	this->PrevForce.z += this->ForceIncrement.z;

	// El valor de fuerza anterior se va actualizando continuamente
	ImmCoord3* Force = &this->PrevForce;
	//cout << "FUERZA APLICADA " << Force->x << Force->y << Force->z << endl;
	this->LSWTool->SetForce(Force);
}

//--------------------------------------------------------------------------
void vtkLSWTool::DisplacementCalculate()
{
	ImmCoord3 ActualPoint;
	this->LSWTool->GetPosition(&ActualPoint);
	if (this->Contact)
	{
		this->Distance.x = ActualPoint.x - this->PointForceOrige.x;
		this->Distance.y = ActualPoint.y - this->PointForceOrige.y;
		this->Distance.z = ActualPoint.z - this->PointForceOrige.z;
	}
	else
	{
		this->Distance.x = ActualPoint.x - this->PrevPoint.x;
		this->Distance.y = ActualPoint.y - this->PrevPoint.y;
		this->Distance.z = ActualPoint.z - this->PrevPoint.z;
	}
}

//--------------------------------------------------------------------------
void vtkLSWTool::ForceIncrementCalculate()
{
	// Calcular el rango de fuerza que hay que aplicar
	this->ForceIncrement.x = this->ActForce.x - this->PrevForce.x;
	this->ForceIncrement.y = this->ActForce.y - this->PrevForce.y;
	this->ForceIncrement.z = this->ActForce.z - this->PrevForce.z;

	// Dividir ese rango entre las rodajas establecidas por vtk_FORCE_STEP, para 
	// aplicar la variaci�n de fuerzas calculada de modo gradual, de esta manera 
	// disminuyen las discontinuidades en las ferzas de realimentaci�n aplicadas a
	// las herramientas de la LSW
	this->ForceIncrement.x /= vtk_FORCE_STEP;
	this->ForceIncrement.y /= vtk_FORCE_STEP;
	this->ForceIncrement.z /= vtk_FORCE_STEP;

	/*	cout << "El incremento de las fuerzas para cada uno de los pasos es de:" << endl;
	 cout << "	Eje horizontal = " << this->ForceIncrement.x << endl;
	 cout << "	Eje vertical = " << this->ForceIncrement.y << endl;
	 cout << "	Profundidad = " << this->ForceIncrement.z << endl;
	 */
}

// Esta funci�n calcula la fuerza de realimentaci�n que debe ser aplicada a las herramientas
// de la LSW en funci�n de los datos llegados desde el simulador, las fuerzas son calculadas
// haciendo uso de la Ley de Hooke
//--------------------------------------------------------------------------
void vtkLSWTool::ForceCalculate()
{
	ImmCoord3 Velocity;
	this->LSWTool->GetVelocity(&Velocity);
	//cout << "La constante de amortiguamiento para el calculo de la fuerza es de" << this->Data->CTE << endl;
	this->ActForce.x = -this->Data->CTE_X * this->Distance.x - vtk_SOFTENING
			* Velocity.x;
	this->ActForce.y = -this->Data->CTE_Y * this->Distance.y - vtk_SOFTENING
			* Velocity.y;
	this->ActForce.z = -this->Data->CTE_Z * this->Distance.z - vtk_SOFTENING
			* Velocity.z;
	/*
	 cout << "La fuerza a aplicar es la siguiente:" << endl;
	 cout << "Eje horizontal = " << this->ActForce.x << endl;
	 cout << "Eje vertical = " << this->ActForce.y << endl;
	 cout << "Profundidad = " << this->ActForce.z << endl;
	 */
}

// Establece los datos necesarios para el c�lculo de las fuerzas de realimentaci�n
// para el caso en que se est�n realizando los c�lculos dentro del m�dulo de control
// del dispositivo LSW
//--------------------------------------------------------------------------
void vtkLSWTool::SetData(int NewData, int OType, double CTE)
{
	if (NewData == 1)
	{
		this->Data->NewData = true;
		this->Data->ContactFlag = true;
	}
	else
	{
		this->Data->NewData = false;
		this->Data->ContactFlag = false;
	}
	this->Data->OType = OType;
	this->Data->CTE_X = CTE;
	this->Data->CTE_Y = CTE;
	this->Data->CTE_Z = CTE;

}

//--------------------------------------------------------------------------
const char * vtkLSWTool::GetDeviceName()
{
	return this->LSWTool->GetDeviceName();
}
