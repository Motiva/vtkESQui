# IMPORTANTE: Dar valor a las siguientes variables
#   - SRML_file_name	Ruta del SRML 01-cutter-brush-nodef.srml
#						Cambiar atributo DataPath del archivo SRML.
#
#   - lens_angle		Inclinacion de la lente.
#						0 grados de inclinacion por defecto (recta)

SRML_file_name   = '01-cutter-brush-nodef.srml'
lens_angle = 30

import vtk
from vtkesqui import vtkSRMLReader

# Lectura del objeto simulacion
reader = vtkSRMLReader()
reader.SetFileName(SRML_file_name)
simulation = reader.ConstructSimulation()

# Configuracion especifica del estilo.
# La especificacion del SRML no lo contempla todavia.
simulation.GetInteractorStyle().SetLensAngle(lens_angle)

# En este ejemplo desactivo las colisiones.
simulation.InteractionOn()
simulation.CollisionOff()
simulation.Initialize()

simulation.Run()