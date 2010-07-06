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
#ifndef _vtkLSWTool_h
#define _vtkLSWTool_h

#include "vtkObject.h"
#include "vtkESQuiHapticsWin32Header.h"
#include "ImmSurgicalWorkstation.h"

#define vtk_WITHOUTCONTACT 1   		// sin contacto
#define  vtk_CONTACTDEFORM 2   		// contacto contra un organo deformable
#define  vtk_CONTACTSCENE 3  		// contacto contra un organo del escenario

#define vtk_MSGRESET 4   			// datos utiles, pero se debe actualizar la posicion actual del haptic (reposicionamiento)
#define vtk_MSGPAUSE 5   			// estamos en modo pausa, no interpretar datos
#define vtk_CONTACTVOX 6	 		// Indica que hay contacto con un Voxel del volumen

#define vtk_CALIBRATION 7	 		// Peticion de calibracion por parte del simulador
#define vtk_END 8

#ifndef vtk_FORCE_STEP 
#define vtk_FORCE_STEP 26			// numero de ciclos para suavizar el cambio de
#endif								// las fuerzas provenientes del simulador: de F(t-1) a F(t)

#define vtk_SOFTENING 0.003			// Amortiguaci�n necesaria para suavizar las fuerzas cuando la
typedef struct {
	//! Haptic-API communication state
	bool CutComunication; 		// Flag usado para detectar el cierre de la comunicacion
	//! New data from haptic
	bool NewData;		  		//Flag usado para detectar la llegada de nuevos datos desde
								// el simulador
	//! Tool contact established
	bool ContactFlag;	  		// Flag para establecer el tipo de contacto entre la herramienta
								// virtual y un organo de la escena
	//! Organ elasticity constant
	double CTE_X, CTE_Y, CTE_Z;		// Contiene el valor de la constante de elasticidad del organo
									// con el que ha colisionado la herramienta virtual
	//! Type of action
	int OType;				// Establece el tipo de accion que se esta realizando en el
							// simulador, a partir de la que se calcularan los datos de
							// interes para la realimentacion de fuerzas

	/*
	//	Esta es otra posible forma de pasar los valores de los dispositivos
	int posX, posY, posZ; // Posicion en coordenadas x, y, z		
	double rot,	// rotacion en radianes
				apertura;	// apertura de las pinzas. 0 -> Cerrada, 1 -> Abierta
	 */
	//! axis displacement
	double LftRgt, UpDwn, InOut;	// Valor en radianes del desplazamiento
									// en cada eje
	//! stick rotation
	double StickRotate, RingRotate;		// Valor en radianes del giro de la herramienta entera
									// y de la rosca
	//! Tool aperture
	Aperture;					// Valor de la apertura de la pinza normalizado entre [0,1]
								// 0->Pinza cerrada		1->Pinza abierta
	//! Imm class
	ImmCoord3 Force, Position;
} vtkLSWDataTool; 	// Estructura usada para almacenar datos utiles en el caculo de posiciones
					// y fuerzas de las herramientas.

//! Immersion LSW tool class.

class VTK_ESQUI_HAPTICS_EXPORT vtkLSWTool: public vtkObject
{
public:
	//! Type revision macro
	vtkTypeRevisionMacro(vtkLSWTool,vtkObject);
	//! Create ne vtkLSWTool object
	static vtkLSWTool *New();
	//! Return class name
	const char *GetClassName() { return "vtkLSWTool"; }
	//! Print object info
	void PrintSelf(ostream& os, vtkIndent indent);
	// Funci�n encargada de la inicializaci�n de la comunicaci�n con el simulador
	//! Set up the communication with the simulator
	void Initialize();
	// Funci�n para la asignaci�n al objeto vtkLSWTool de una herramienta de la LSW
	//! Assign to the vtkLSWTool object a tool of the LSW
	bool ToolAssign(ImmSWTool* Tool);
	// Funciones para la lectura desde el exterior del estado de la herramienta
	//! Get the tool's depth
	double GetToolDepth();
	//! Get the tool's yaw
	double GetToolYaw();
	//! Get the tool's pitch
	double GetToolPitch();
	//! Get the tool's aperture
	double GetToolOpening();
	//! Get the tool's twist
	double GetToolRoll();
	//! Set the value of the tool force.
	void SetToolForce(ImmCoord3 Force) {this->LSWTool->SetForce(&Force);};
	//! This function calculates the feedback force in function of the data arrived by the function vtkLSWTool::SetData
	void Refresh();

	//! Establishes the data needed for the feedback force calculation.
	// OType -> set type of operation that the virtual tool is doing in the scene:
	//		"1": Contacting
	//		"2": withOut contact
	// NewData -> Indicates that new data are available for the force calculation
	// CTE -> Contains the elastic constant value that will be used for the force calculation
	void SetData(int NewData, int OType, double CTE);

	//! Get haptic device name
	const char * GetDeviceName();
private:
	// Objeto de la clase ImmSWTool, es la interfaz con la herramienta de la LSW 
	//! ImmSWTool object
	ImmSWTool* LSWTool;
	// Variable miebro que indica si se est� o no en modo Depuraci�n
	//! fag that indicates if debug mode is on or off
	bool Debug;
	// Contiene datos para enviar a la LSW
	//! Data to send to the LSW
	vtkLSWDataTool* Data;

	// Contiene el valor de la realimentacion de fuerzas aplicada a la LSW
	//! Force feedback to apply to the LSW
	ImmCoord3 ActForce;
	// Contiene el valor de la realimentacion de fuerzas que aplicada en el instante anterior a la LSW
	//! Force feedback appied to the LSW in the previous instant
	ImmCoord3 PrevForce;

	// Contiene el punto de Origen para el calculo de las fuerzas de realimentacion
	//   Se toma la posicion de la herramienta virtual en el momento del
	//   del primer contacto en el que se pince con el modelo deformable
	//! Point of origin for the force feedback calculation
	ImmCoord3 PointForceOrige;

	// Contiene la variacion de la fuerza aplicada a la LSW para cada iteraci�n del bucle de control. Valor 
	// establecido para evitar variaciones bruscas en la realimentaci�n de fuerzas
	//! Contains the increment of the force feedback to apply in this instant
	ImmCoord3 ForceIncrement;

	// Contiene la posici�n anterior, valor usado durante el calculo de las fuerzas de realimentacion
	//   Se toma la posicion de la herramienta virtual en el momento anterior
	//   para el calculo de los contactos sin pinzamiento
	//! Contains the previous position, this value is used for to calculate the force feedback
	ImmCoord3 PrevPoint;

	// Contiene la distancia entre el punto de origen del contacto y
	//   el punto actual de la punta de la pinza
	//! Distance between the point of contact and the actual tip position
	ImmCoord3 Distance;

	//! Flag usado para indicar el primer contacto entre la herramienta y un �rgano
	//   Esta desactiva en cuanto se produce el primer contacto
	//! Flag to indicate if a first contact between the tool and an organ has been produced
	bool FirsContact;

	// Flag para controlar el tipo de accion sobre el tejido
	//   Se utiliza para diferenciar:
	//                  pinzado = fuerzas con ley de hooke
	//                  sin pinzado = fuerzas constante de oposicion
	//! Flag to control the action type on the organ
	/*!
	true -> Hooke law to calculate the forces
	false -> forces applied are constants
	*/
	bool Contact;


	void ForceUpdate();
	void DisplacementCalculate();
	void ForceIncrementCalculate();
	void ForceCalculate();
	//! Contains the value of the feedback force to apply to the tool
	ImmCoord3 ForceValue;
	//! Contains the step of feedback force to apply to the tool in every step
	ImmCoord3 ForceStep;
	//! Contains the actual feedback force applyed to the tool
	ImmCoord3 ActualForceValue;

protected:

	vtkLSWTool();
	~vtkLSWTool();

};

#endif
