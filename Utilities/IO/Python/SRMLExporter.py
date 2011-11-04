
"""@package SRMLExporter
SRML file exporter to be used in the vtkESQui surgical simulation platform.
It generates a SRML (XML-based) file containing all the required info to 
generate a surgical scenario in vtkESQui.

XML Parsing and generation is made using the lxml package. For more info on 
this package check its website http://lxml.de.
"""

import sys
import StringIO

try:
  from lxml import etree
  #print("running with lxml.etree")
except ImportError:
  try:
    # Python 2.5
    import xml.etree.cElementTree as etree
    #print("running with cElementTree on Python 2.5+")
  except ImportError:
    try:
      # Python 2.5
      import xml.etree.ElementTree as etree
      #print("running with ElementTree on Python 2.5+")
    except ImportError:
      try:
        # normal cElementTree install
        import cElementTree as etree
        #print("running with cElementTree")
      except ImportError:
        try:
          # normal ElementTree install
          import elementtree.ElementTree as etree
          #print("running with ElementTree")
        except ImportError:
          print("Failed to import ElementTree from any known place")


class SRMLExporter:
  def __init__(self):
    self.filename = ""
    self.root = ""
    
  def base(self):
    """Initialize SRML file base structure
     *  returns XML file root element
    """
    if (self.filename == ""):
      self.filename = "tmp.srml"
    
    # Set DOCTYPE and DTD
    s = """<?xml version="1.0" encoding="utf-8"?><!DOCTYPE Simulation SYSTEM "test" [ <!ENTITY tasty "cheese"><!ENTITY eacute "&#233;"> ]><root><a>&tasty; souffl&eacute;</a></root>"""
    tree = etree.parse(StringIO.StringIO(s))
    
    # Define root element
    # <Simulation> 
    self.root = etree.Element("Simulation")
    self.root.set('Name', 'default')
    self.root.set('Type', 'default')
    self.root.set('Interaction', '0')
    self.root.set('Collision', '0')
    self.root.set('RenderRate', '0.04') #25fps
    self.root.set('SimulationRate', '0.01') #100hz
    self.root.set('InteractionRate', '0.001') #1Khz
    self.root.set('Gravity', '0.0 -9.81 0.0')
    self.root.set('DataPath', '')
    tree._setroot(self.root)
    
    # Simulation children
    # <Interaction>
    interaction = etree.Element("Interaction")
    interaction.set('Name', 'default')
    interaction.set('Type', 'default')
    self.root.append(interaction)
    
    # <Collision>
    collision = etree.Element("Collision")
    collision.set('Name', 'default')
    collision.set('Type', 'default')
    self.root.append(collision)
    
    # <Scenario>
    scenario = etree.Element("Scenario")
    scenario.set('Name', 'default')
    self.root.append(scenario)
    
    # Scenario children
    # <Environment>
    env = etree.Element("Environment")
    env.set('Name', 'default')
    env.set('Ambient', '0.0 0.0 0.0')
    env.set('Background', '0.0 0.0 0.0')
    scenario.append(env)
    
    # Environment children
    # <Cameras>
    cams = etree.Element("Cameras")
    cams.set('Name', 'default')
    env.append(cams)

    # <Lights>
    lights = etree.Element("Lights")
    lights.set('Name', 'default')
    env.append(lights)
    
    # <Objects>
    objs = etree.Element("Objects")
    objs.set('Name', 'default')
    scenario.append(objs)
  
  # Get elements from simulation
  def getElement(self, name):
    """Get an XML element from the simulation
    """
    return self.root.find(name)

  # Get elements from simulation
  def getElementAttribute(self, name, key):
    """Get the attribute value an XML element from the simulation
    """
    e = self.root.find(name)
    if (e == 0):
      print "Simulation has no element with name: "+name+" "
    return e.get(key)

  # Generic methods for attribute definition
  # Set standard element to attribute
  def setAttribute(self, obj, key, value):
    """Set an XML attribute of an object
     * obj: XML object to be modified
     * key: Attribute name
     * value: Attribute value
    """
    if obj.attrib.get(key):
      s = str(value)
      s.encode('utf-8')
      obj.set(key, s)
    else:
      print "Object "+str(obj.tag)+" has no "+str(key)+" attribute"
  
  # Set vector element to attribute
  def setVector3Attribute(self, obj, key, x, y, z):
    """Set an XML vector type attribute of an object
     * obj: XML object to be modified
     * key: Attribute name
     * x: first component of the vector
     * y: second component of the vector
     * z: third component of the vector 
    """
    value = str(x)+" "+str(y)+" "+str(z)
    self.setAttribute(obj, key, value)
  
  # Add a new haptic
  def addHaptic(self, type):
    """Add a haptic to the simulation
     * root: XML file root element
     * returns the new XML camera element
    """
    interaction = self.root.find('.//Interaction')
    
    # Object default attributes
    haptic = etree.Element("Haptic")
    haptic.set('Name', 'default')
    haptic.set('Type', type)
    haptic.set('NumberOfTools', str(0))
    interaction.append(haptic)
    
    return haptic
    
  # Scenario environment methods
  # Add a Camera
  def addCamera(self):
    """Add a camera to the scenario
     * root: XML file root element
     * returns the new XML camera element
    """
    cams = self.root.find('.//Cameras')
    
    # Object default attributes
    cam = etree.Element('Camera')
    cam.set('Id', str(len(cams)))
    cam.set('Name', 'default')
    cam.set('Position', '0.0 0.0 0.0')
    cam.set('Orientation', '0.0 0.0 0.0')
    cam.set('FocalPoint', '0.0 0.0 0.0')
    cam.set('ViewAngle', '60')
    cam.set('ParallelProjection', '0')
    cam.set('Yaw', '0.0')
    cam.set('Elevation', '0.0')
    cam.set('Pitch', '0.0')
    cam.set('Dolly', '0.0')
    cams.append(cam)

    return cam

  # Add a light
  def addLight(self):
    """Add a light to the scenario
     * root: XML file root element
     * returns the new XML light element
    """
    lights = self.root.find('.//Lights')
    
    # Object default attributes
    l = etree.Element('Light')
    l.set('Id', str(len(lights)))
    l.set('Name', 'default')
    l.set('LightType', 'Scene')
    l.set('Positional', '0')
    l.set('Intensity', '1.0')
    l.set('AmbientColor', '0.0 0.0 0.0')
    l.set('DiffuseColor', '0.0 0.0 0.0')
    l.set('SpecularColor', '0.0 0.0 0.0')
    l.set('ConeAngle', '60.0')
    lights.append(l)

    return l
  
  # Scenario object methods
  # Add an object to the simulation
  def addObject(self, type):
    objs = self.root.find('.//Objects')
    
    # Object default attributes
    obj = etree.Element('Object')
    obj.set('Id', str(len(objs)))
    obj.set('Name', 'default')
    obj.set('Type', type)
    obj.set('Class', 'default')
    obj.set('Model', 'default')
    obj.set('Status', '1')
    objs.append(obj)
    
    # Object elements
    elements = etree.Element("Elements")
    obj.append(elements)
    
    return obj

  # Add an element to a scenario object
  def addElement(self, obj):
    if obj.tag != "Object":
      print "Object is not a scenario object"
      return 0
    
    elements = obj.find('Elements')
    if elements.tag == 'Elements':
      e = etree.Element('Element')
      e.set('Id', str(len(elements)))
      e.set('Name', 'default')
      e.set('Status', '1')
      e.set('Type', obj.attrib.get('Type'))
      e.set('Scale', '1.0 1.0 1.0')
      e.set('Origin', '0.0 0.0 0.0')
      e.set('Position', '0.0 0.0 0.0')
      e.set('Orientation', '0.0 0.0 0.0')
      elements.append(e)
      
      models = etree.Element('Models')
      e.append(models)
      return e
    else:
      print "Element does not contain an element collection"
      return 0

  # Add a model to a scenario element
  def addModel(self, e, type):
    """Add a camera to the scenario object
     * root: XML file root element
      * returns the new XML model element
    """
    if e.tag != 'Element':
      print "Object is not an element"
      return 0
    
    models = e.find('Models')
    if models.tag == 'Models': 
      m = etree.Element('Model')
      m.set('Id', str(len(models)))
      m.set('Name', 'default')
      m.set('Type', str(type))
      m.set('Status', '1')
      m.set('Color', '1.0 1.0 1.0')
      m.set('Visibility', '1.0')
      m.set('Opacity', '1.0')
      m.set('FileName', "pathto/file")
      
      #Add extra attributes
      if type == 'Visualization':
        m.set('TextureFileName', "pathto/file")
        
      models.append(m)
      return m
    else:
      print "Element does not contain a model collection"
      return 0
    
  def addInterface(self, m, type):
    if m.tag != 'Model':
      print "Object is not a Model"
      return 0
    i = etree.Element('Interface')
    i.set('Type', str(type))
    i.set('TimeStep', '0.0')
    i.set('Gravity', '0.0 0.0 0.0')
    if (type == "ParticleSpring"):
      i.set('Spring', '0.0')
      i.set('Damping', '0.0')
      i.set('Distance', '0.0')
      i.set('Mass', '0.0')
      i.set('Solver', 'Euler')
      i.set('FileName', "pathto/file")
    elif (type == "Explicit"):
      i.set('Iterations','0.0')
      i.set('WarpScaleFactor','0.0')
    else:
      print "Interface type not valid."
      return 0
    
    m.append(i)
    
    return i

  def write(self):
    """Write the SRML file to disk
    """
    f = open(self.filename, 'w')
    f.write(etree.tostring(self.root.getroottree(), pretty_print=True, xml_declaration=True, encoding='utf-8'))
    f.close()

def main():
    
    ##### Example of use #####
    
    path = "/tmp/out.srml"
    if len(sys.argv) > 1:
        path = sys.argv[1]
    
    exporter = SRMLExporter()
    exporter.filename = path
    
    # Generate simulation base
    print "\nGenerating document base..."
    exporter.base()
    
    print "\nDefining SRML elements..."
    # Add new Haptic device
    hap = exporter.addHaptic('IHP')
    exporter.setAttribute(hap, 'NumberOfTools', 2)
    
    # Add new camera
    cam = exporter.addCamera()
    exporter.setVector3Attribute(cam, 'FocalPoint', 0.0, 0.0, -6.0)
    exporter.setAttribute(cam, 'Yaw', 20.0)
    
    # Add new light
    l = exporter.addLight()
    exporter.setAttribute(l, 'LightType', 'Scene');
    exporter.setAttribute(l, 'Intensity', 0.5);
    exporter.setVector3Attribute(l, 'DiffuseColor', 1.0, 1.0, 1.0);
    
    # Add new tool (Probe)
    probe = exporter.addObject('Tool')
    exporter.setAttribute(probe, 'Name', 'probe')
    exporter.setAttribute(probe, 'Class', 'Laparoscopy')
    exporter.setAttribute(probe, 'Model', 'Probe')
    
    # Probe element (at least 1)
    stick = exporter.addElement(probe)
    exporter.setAttribute(stick, 'Name', 'probe_stick')
    exporter.setAttribute(stick, 'Position', '0.0 1.0 0.5')
    exporter.setVector3Attribute(stick, 'Position', 0.0, 1.0, 0.5) #....
    
    # Element models (at least 1). For each polydata generate a model.
    m = exporter.addModel(stick, 'Visualization')
    exporter.setAttribute(m, 'FileName', '/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/stick.vtp')
    exporter.setAttribute(m, 'TextureFileName', '/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/steel.jpg')
    
    m = exporter.addModel(stick, 'Collision')
    exporter.setAttribute(m, 'FileName', '/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/stick_col.vtp')
    
    # Grasper element (at least 1)
    tip = exporter.addElement(probe)
    exporter.setAttribute(tip, 'Name', 'probe_tip')
    exporter.setAttribute(tip, 'Position', '0.0 1.0 0.5')
    exporter.setVector3Attribute(tip, 'Position', 0.0, 1.0, 0.5) #....
    
    # Element models (at least 1). For each polydata generate a model.
    m = exporter.addModel(tip, 'Visualization')
    exporter.setAttribute(m, 'FileName', '/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/tip.vtp')
    exporter.setAttribute(m, 'TextureFileName', '/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/steel.jpg')
    
    m = exporter.addModel(tip, 'Collision')
    exporter.setAttribute(m, 'FileName', '/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/tip_col.vtp')
    
    # Add new organ
    organ = exporter.addObject('Organ')
    exporter.setAttribute(organ, 'Name', 'ball')
    exporter.setAttribute(organ, 'Class', 'Deformable')
    
    # Organ element (at least 1)
    e = exporter.addElement(organ)
    exporter.setAttribute(e, 'Name', 'Organ0')
    exporter.setAttribute(e, 'Position', '0.0 1.0 0.5')
    exporter.setVector3Attribute(e, 'Position', 0.0, 1.0, 0.5) #....
    
    # Element models (at least 1). For each polydata generate a model.
    m = exporter.addModel(e, 'Visualization')
    exporter.setAttribute(m, 'FileName', '/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball.vtp')
    exporter.setAttribute(m, 'TextureFileName', '/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/flesh.jpg')
    
    m = exporter.addModel(e, 'Collision')
    exporter.setAttribute(m, 'FileName', '/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball_col.vtp')
     
    m = exporter.addModel(e, 'Deformation')
    exporter.setAttribute(m, 'FileName', '/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball_ball_def_c8.vtp')
    
    i = exporter.addInterface(m, 'ParticleSpring')
    exporter.setAttribute(i, 'TimeStep', '0.001')
    exporter.setAttribute(i, 'Spring', '100')
    exporter.setAttribute(i, 'Damping', '5.0')
    exporter.setAttribute(i, 'Mass', '1.0')
    exporter.setAttribute(i, 'Distance', '1.0')
    
    print "\nSRML file has been successfully generated in \n"+exporter.filename+"\n"
    
    exporter.write()
     
    return 0

if __name__ == "__main__":
    main()
