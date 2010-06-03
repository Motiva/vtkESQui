# $Id: Tex_VTKBlender.py,v1.1
#
# Copyright (c) 2006, Center for Technology in Medicine, 
# University of Las Palmas de Gran Canaria.
#
# Copyright (c) 2005, Chris Want, Research Support Group,
# Computing & Network Services, University of Alberta. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions are met:
#
# 1) Redistributions of source code must retain the above copyright 
#    notice, this list of conditions and the following disclaimer.
# 2) Redistributions in binary form must reproduce the above copyright 
#    notice, this list of conditions and the following disclaimer in the 
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
# THE POSSIBILITY OF SUCH DAMAGE.

"""
VTK inside Blender module.

    This module provides code so that polydata from vtk can
    be used inside of blender.

    Python needs to find the vtk stuff and this module in order
    for this to work, and you can either a) set your PYTHONPATH
    in your environment, or you can b) hardcode your vtk path's
    in your script, e.g.,

    a) at the prompt, before starting blender, type:
       PYTHONPATH=$VTK_ROOT/Wrapping/Python:${LIBRARY_OUTPUT_PATH}
	   PYTHONPATH=$PYTHONPATH:${PATH_TO_THIS_MODULE}
	   export PYTHONPATH

    b) add the following to your script near the beginning, before
	   importing vtk or VTKBlender:
	   
       import sys
       sys.path.append($VTK_ROOT/Wrapping/Python)
       sys.path.append(${LIBRARY_OUTPUT_PATH})
       sys.path.append(${PATH_TO_VTKBlender_MODULE})

    Be sure to replace $VTK_ROOT and ${LIBRARY_OUTPUT_PATH} with
    values that are relevant to your system. These values can be
    found by starting vtkpython with no arguments and typing:

    import sys
    print sys.path

	Usually the first two items reported are the ones you want.

	Also replace ${PATH_TO_VTKBlender_MODULE} with wherever you have
	put the VTKBlender module.
"""

import vtk

try:
	import Blender
	from Blender import NMesh, Object, Material
except:
	print "No Blender module found!"

#---------------------------------------------------------------------
# CODIGO INTRODUCIDO PARA PERMITIR LA INCLUSION DE LA                 
# INFORMACION DE LA TEXTURA EN EL POLYDATA GENERADO                   
#                                                                     
# Funcion calc_coords, toma como entrada la malla a convertir         
# polydata y devuelve una lista con las coordenadas de la textura     
# para cada vertice de la malla.                                      
                                                                      
def calc_coords(me):                                                  
                                                                      
	coorIndex = me.verts                                          
	texcoorlis = vtk.vtkFloatArray()                              
	texcoorlis.SetNumberOfComponents(2)                           
	texcoorlis.SetNumberOfTuples(len(coorIndex))                  
	caras = me.faces                                              
	for vert in coorIndex:                                        
		encontrado = 0                                        
		for cara in caras:                                    
			vertices = cara.v                                   
			i = 0                                               
			for vertice in vertices:                            
				if vertice == vert:                           
					encontrado = 1                          
					t0 = cara.uv[i][0]                      
					t1 = cara.uv[i][1]                      
					indx = coorIndex.index(vert)            
					texcoorlis.SetTuple2(indx, t0, t1)      
					break                                   
				i = i+1                                       
			if encontrado == 1:                                 
				break                                         
	return texcoorlis                                               
#---------------------------------------------------------------------



def BlenderToPolyData(me):

	pcoords = vtk.vtkFloatArray()
	pcoords.SetNumberOfComponents(3)
	pcoords.SetNumberOfTuples(len(me.verts))
	for i in range(len(me.verts)):
		p0 = me.verts[i][0]
		p1 = me.verts[i][1]
		p2 = me.verts[i][2]
		pcoords.SetTuple3(i, p0, p1, p2)

	points = vtk.vtkPoints()
	points.SetData(pcoords)

	polys = vtk.vtkCellArray()
	lines = vtk.vtkCellArray()
	for face in me.faces:
		if len(face.v) == 4:
			polys.InsertNextCell(4)
			polys.InsertCellPoint(face.v[0].index)
			polys.InsertCellPoint(face.v[1].index)
			polys.InsertCellPoint(face.v[2].index)
			polys.InsertCellPoint(face.v[3].index)
		elif len(face.v) == 3:
			polys.InsertNextCell(3)
			polys.InsertCellPoint(face.v[0].index)
			polys.InsertCellPoint(face.v[1].index)
			polys.InsertCellPoint(face.v[2].index)
		elif len(face.v) == 2:
			lines.InsertNextCell(2)
			lines.InsertCellPoint(face.v[0].index)
			lines.InsertCellPoint(face.v[1].index)

	pdata =vtk.vtkPolyData()
	pdata.SetPoints(points)
	pdata.SetPolys(polys)
	pdata.SetLines(lines)

#------------------------------------------------------------------
# CODIGO INTRODUCIDO PARA PERMITIR LA INCLUSION DE LA             #
# INFORMACION DE LA TEXTURA EN EL POLYDATA GENERADO               #

	if me.hasFaceUV():
		pdata.GetPointData().SetTCoords(calc_coords(me))	# Se insertan las coordenadas en el polydata
#------------------------------------------------------------------

	pdata.Update()

	return pdata


	
def PolyDataMapperToBlender(pmapper, me=None):

	newmesh = 0
	if (me == None):
		me = NMesh.GetRaw()	#Este metodo es usado para crear un objeto "malla" en memoria. Este objeto es creado en Buffer
		newmesh = 1
	else:
		me.verts = []
		me.faces = []

	# vtkLookupTable es un objeto usado por los objetos de mapeado para mapear valores escalares en una especificacion de color rgba
	pmapper.Update()

	pdata = pmapper.GetInput()		#Especifica los datos de entrada a mapear
	plut  = pmapper.GetLookupTable()	#
	#print pdata.GetNumberOfCells()

	scalars  = pdata.GetPointData().GetScalars()

	for i in range(pdata.GetNumberOfPoints()):
		point = pdata.GetPoint(i)
		v = NMesh.Vert(point[0],point[1],point[2])
		me.verts.append(v)

	colors = None
	if ( (scalars != None) and (plut != None) ):
		colors = []
		for i in range(scalars.GetNumberOfTuples()):
			color = map(VTKToBlenderColor, plut.GetColor(scalars.GetTuple1(i)))
			colors.append(NMesh.Col(color[0], color[1], color[2]))

	for i in range(pdata.GetNumberOfCells()):
		cell = pdata.GetCell(i)
		#print i, pdata.GetCellType(i)

		# Do lines
		if pdata.GetCellType(i)==3:

			n1 = cell.GetPointId(0)
			n2 = cell.GetPointId(1)

			BlenderAddFace(me, colors, n1, n2)

		# Do poly lines
		if pdata.GetCellType(i)==4:
			for j in range(cell.GetNumberOfPoints()-1):

				n1 = cell.GetPointId(j)
				n2 = cell.GetPointId(j+1)

				BlenderAddFace(me, colors, n1, n2)

		# Do triangles
		if pdata.GetCellType(i)==5:

			n1 = cell.GetPointId(0)
			n2 = cell.GetPointId(1)
			n3 = cell.GetPointId(2)

			BlenderAddFace(me, colors, n1, n2, n3)


		# Do triangle strips
		if pdata.GetCellType(i)==6:
			for j in range(cell.GetNumberOfPoints()-2):
				if (j % 2 == 0):
					n1 = cell.GetPointId(j)
					n2 = cell.GetPointId(j+1)
					n3 = cell.GetPointId(j+2)
				else:
					n1 = cell.GetPointId(j)
					n2 = cell.GetPointId(j+2)
					n3 = cell.GetPointId(j+1)

				BlenderAddFace(me, colors, n1, n2, n3)

		# Do polygon
		if pdata.GetCellType(i)==7:
			# Add a vert at the center of the polygon,
			# and break into triangles
			x    = 0.0
			y    = 0.0
			z    = 0.0
			scal = 0.0
			N = cell.GetNumberOfPoints()
			for j in range(N):
				point = pdata.GetPoint(cell.GetPointId(j))
				x = x + point[0]
				y = y + point[1]
				z = z + point[2]
				if (scalars != None):
					scal = scal + scalars.GetTuple1(j)
			x    = x / N
			y    = y / N
			z    = z / N
			scal = scal / N

			newidx = len(me.verts)
			v = NMesh.Vert(x, y, z)
			me.verts.append(v)
			if (scalars != None):
				color = map(VTKToBlenderColor, plut.GetColor(scal))
				colors.append(NMesh.Col(color[0], color[1], color[2]))

			# Add triangles connecting polynomial sides to new vert
			for j in range(N):
				n1 = cell.GetPointId(j)
				n2 = cell.GetPointId( (j+1) % N )
				n3 = newidx
				BlenderAddFace(me, colors, n1, n2, n3)

		# Do pixel
		if pdata.GetCellType(i)==8:
			n1 = cell.GetPointId(0)
			n2 = cell.GetPointId(1)
			n3 = cell.GetPointId(2)
			n4 = cell.GetPointId(3)

			BlenderAddFace(me, colors, n1, n2, n3, n4)
			
		# Do quad
		if pdata.GetCellType(i)==9:
			n1 = cell.GetPointId(0)
			n2 = cell.GetPointId(1)
			n3 = cell.GetPointId(2)
			n4 = cell.GetPointId(3)

			BlenderAddFace(me, colors, n1, n2, n3, n4)

	if not me.materials:
		newmat = Material.New()
		if (colors != None):
			newmat.mode |= Material.Modes.VCOL_PAINT
		me.materials.append(newmat)

	if (newmesh==0):
		me.update()

	return me

def VTKToBlenderColor(x):
  return int(255*float(x)+0.5)

def BlenderAddFace(me, colors, n1, n2, n3=None, n4=None):

	f = NMesh.Face()

	if (n4 != None):
		f.v = [ me.verts[n1], me.verts[n2], me.verts[n3], me.verts[n4] ]
		if (colors != None):
			f.col = [colors[n1],colors[n2], colors[n3], colors[n4]]
			f.uv =  [(0,0), (0,1), (1,1), (1,0)]  # Needed because of bug
			f.mode = NMesh.FaceModes['SHAREDCOL']

	elif (n3 != None):
		f.v = [ me.verts[n1], me.verts[n2], me.verts[n3] ]
		if (colors != None):
			f.col = [colors[n1],colors[n2], colors[n3]]
			f.uv =  [(0,0), (0,1), (1,1)]  # Needed because of bug
			f.mode = NMesh.FaceModes['SHAREDCOL']

	else:
		f.v = [ me.verts[n1], me.verts[n2] ]
		if (colors != None):
			f.col = [colors[n1],colors[n2]]
			f.uv =  [(0,0), (0,1)]
			f.mode = NMesh.FaceModes['SHAREDCOL']

	me.faces.append(f)

