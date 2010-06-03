#!BPY
import Blender
from Blender import *

#***************************************************
# Exportador VRML 1.0 ******************************
#***************************************************

class ExpVRML1:

	def __init__(expo, nombrearchivo):
	
		#--- Privado
		expo.nombrearchivo=nombrearchivo
		expo.archivo = open(nombrearchivo, "w")
		expo.indentLevel = 0	# Valor empleado para el tabulador en el archivo .wrl

	#-----------------------------------------------------------------------
	# Funcion calc_coords, toma como entrada la malla de Blender
	# y devuelve una lista con las coordenadas de la textura para cada 
	# vertice de la malla.
	
	def calc_coords(expo, malla):
	
		coorIndex = malla.verts		# Lista con los vertices de la malla
		texcoorlis = []
		caras = malla.faces		# Lista con las caras de la malla
		for vert in coorIndex:
			encontrado = 0
			for cara in caras:
				vertices = cara.v
				i = 0
				for vertice in vertices:
					if vertice == vert:
						encontrado = 1
						texcoorlis.append(cara.uv[i])
						break
					i = i+1
				if encontrado == 1:
					break
		return texcoorlis

	#-----------------------------------------------------------------------
	# Esta funcion detecta si hay caras con suavizado en la malla

	def suavizado(expo, malla):
		caras = malla.faces
		smoo = 0
		for cara in caras:
			if cara.smooth:
				smoo = 1
				break
		return smoo
		
	#-----------------------------------------------------------------------
	# Funcion para corregir la diferencia entre los ejes de coordenadas 
	# de Blender y VRML

	def rotVertex(expo, mm, v):

		lx,ly,lz=v[0],v[1],v[2]
		gx=(mm[0][0]*lx + mm[1][0]*ly + mm[2][0]*lz) + mm[3][0]
		gy=((mm[0][2]*lx + mm[1][2]*ly+ mm[2][2]*lz) + mm[3][2])
		gz=-((mm[0][1]*lx + mm[1][1]*ly + mm[2][1]*lz) + mm[3][1])
		rotatedv=[gx,gy,gz]
		return rotatedv
	
	#-----------------------------------------------------------------------
	def writeId(expo, s, inc=0):

		if inc < 1:
			expo.indentLevel = expo.indentLevel + inc

		spaces=""
		for x in range(expo.indentLevel):
			spaces = spaces + "	"

		expo.archivo.write(spaces + s)
		print(s)

		if inc > 0:
			expo.indentLevel = expo.indentLevel + inc



	def cleanup(expo):
		expo.archivo.close()
		print("cerrado")

	def cabecera(expo):
		
		expo.archivo.write("#VRML V1.0 ascii\n")
		expo.archivo.write("# Disegnado por medio de Blender 3D, http://blender3d.org\n")
		expo.archivo.write("# exportado por el sript de Raul\n\n")
		#expo.cleanup()
	

	def creaforma(expo, nombre, mm):
		
		#objetos = Blender.Object.GetSelected()
		#if not objetos:
		#	Blender.Draw.PupMenu("ERROR%t|Debe seleccionar la malla a exportar")
		#	return		
		#if len(objetos) > 1:
		#	Blender.Draw.PupMenu("ERROR%t|Solo se debe seleccionar un objeto de la escena!")
		#	return
		#objeto = objetos[0]
		#if objeto.getType() != "Mesh":
		#	Blender.Draw.PupMenu("ERROR%t|Solo se deben seleccionar mallas")
		#	return
#
		#mm = objeto.getMatrix()
		#nombre = objeto.getName()
		#expo.cabecera()
		malla = NMesh.GetRaw(nombre)
		vertices = malla.verts		# Vertices de la malla
		str = ""
		expo.writeId("DEF %s%s Separator {\n" % ("G_", nombre),1)

		#----------------------------------Material------------------------------------------------------------

		mats = malla.materials
		if len(mats) > 0:
			expo.writeId("Material {\n",1)
			mat=Blender.Material.Get(mats[0].name)
			diffuseR = mats[0].rgbCol[0]
			diffuseG = mats[0].rgbCol[1]
			diffuseB = mats[0].rgbCol[2]
			specR = mats[0].specCol[0]
			specG = mats[0].specCol[1]
			specB = mats[0].specCol[2]
			shininess = mats[0].hard/255.0
			transp = 1-mats[0].alpha
			expo.writeId("diffuseColor %s %s %s" % 
				(round(diffuseR), round(diffuseG), round(diffuseB)) +
				"\n")
			expo.writeId("specularColor %s %s %s" %
				(round(specR), round(specG), round(specB)) +
				"\n" )
			expo.writeId("shininess %s" %
				(round(shininess)) +
				"\n" )
			expo.writeId("transparency %s" %
				(round(transp)) +
				"\n")
			expo.writeId("} # Material\n",-1)

		
		#--------------------------------Textura-------------------------------------------------------------

		if malla.hasFaceUV():
			expo.writeId("Texture2 {\n",1)
			imag = malla.faces[0].image.name
			expo.writeId("filename %s\n" % imag)
			expo.writeId("}\n",-1)

		#-----------------Coordenadas de la textura-----------------------------------------------------------
			expo.writeId("TextureCoordinate2 {\n",1)
			expo.writeId("point [\n", 1)
			tx_coords = expo.calc_coords(malla)
			for tx_coor in tx_coords:
				U = tx_coor[0]
				V = tx_coor[1]
				expo.writeId("%s %s,\n" % (U, V))
			expo.writeId("]\n", -1)
			expo.writeId("}\n", -1)

		#---------------------------Suavizado de la malla----------------------------------------------------
		
		expo.writeId("ShapeHints {\n", 1)
		if expo.suavizado(malla):
			ang = malla.getMaxSmoothAngle()*0.01745		# paso a radianes aplicando rad=(deg*pi)/180
			expo.writeId("creaseAngle %s\n" % (ang))
		else:
			expo.writeId("creaseAngle 0.0\n")
		expo.writeId("}\n", -1)

		#-----------------------------------------------------------------------------------------------------
		# Se introducen las coordenadas de los vertices de la malla
		expo.writeId("Coordinate3 {\n",1)
		expo.writeId("point [\n",1)
		for vertice in vertices:
			v=expo.rotVertex(mm, vertice)
			expo.writeId("%s %s %s,\n" % (v[0], v[1], v[2]))
		expo.writeId("]\n",-1)
		expo.writeId("}\n",-1)
		#-----------------------------------------------------------------------------------------------------

		expo.writeId("IndexedFaceSet {\n",1)
		expo.writeId("coordIndex [\n", 1)
		  
		for face in malla.faces:
			cordStr=""
			for i in range(len(face)):
				indx=vertices.index(face[i])
				cordStr = cordStr + "%s, " % indx

			expo.writeId(cordStr + "-1,\n")

		expo.writeId("] # coordIndex\n", -1)
		
		if malla.hasFaceUV == True:
			expo.writeId("textureCoordIndex [\n", 1)
			for face in malla.faces:
				cordStr=""
				for i in range(len(face)):
					indx=vertices.index(face[i])
					cordStr = cordStr + "%s, " % indx
	
			expo.writeId(cordStr + "-1,\n")
			expo.writeId("]\n", -1)

		expo.writeId("}\n", -1)

		expo.writeId("}\n", -1)
		expo.cleanup()
		

#wrlexport=ExpVRML1("estomago.wrl")
#wrlexport.cabecera()
#wrlexport.creaforma()
