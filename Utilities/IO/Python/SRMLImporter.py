"""@package SRMLExporter
SRML file importer to be used in the vtkESQui surgical simulation platform.
Imports a SRML (XML-based) file containing all the required info to 
generate a surgical scenario in vtkESQui.

XML parsing is made using the lxml package. For more info on 
this package check its website http://lxml.de & http://lxml.de/parsing.html.
"""

import sys

import vtk
import vtkesqui

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
          
class SRMLImporter:
    def __init__(self):
        self.filename = "";
        self.srml = "";
                #Initialize rendering objects
        self.ren = vtk.vtkRenderer();
        #ren1.SetBackground(1.0,1.0,1.0);

        self.renWin = vtk.vtkRenderWindow();
        self.renWin.AddRenderer(self.ren);
        #renWin->SetSize(840,480);

        self.iren = vtk.vtkRenderWindowInteractor();
        self.iren.SetRenderWindow(self.renWin);
        
        #Initialize vtkesqui simulation
        self.simulation = vtkesqui.vtkSimulation()
        #Initialize vtkesqui scenario
        self.scenario = vtkesqui.vtkScenario()
        self.scenario.SetRenderWindow(self.renWin)
        
        self.simulation.SetScenario(self.scenario)
        
    def setFileName(self, name):
        self.filename = name;
        
    def parse(self):
        if(len(self.filename) > 0):
            self.srml = etree.parse(self.filename)
            #print etree.tostring(self.srml.getroot())
            
    def importCamera(self, c):
        print 'importing camera...'
        cam = vtk.vtkCamera()
        
        a = str(c.get('Position')).split(" ")
        cam.SetPosition(float(a[0]), float(a[1]), float(a[2]));
        a = str(c.get('FocalPoint')).split(" ")
        cam.SetFocalPoint(float(a[0]), float(a[1]), float(a[2]));
        cam.SetViewAngle(float(c.get('ViewAngle')))
        cam.Yaw(float(c.get('Yaw')))
        cam.Elevation(float(c.get('Elevation')))
        cam.Pitch(float(c.get('Pitch')))
        cam.Dolly(float(c.get('Dolly')))
        cam.SetParallelProjection(float(c.get('ParallelProjection')))
        
        return cam
            
    def importLight(self, l):
        print 'importing light...'
        light = vtk.vtkLight()
        
        a = str(l.get('AmbientColor')).split(" ")
        light.SetAmbientColor(float(a[0]), float(a[1]), float(a[2]));
        a = str(l.get('SpecularColor')).split(" ")
        light.SetSpecularColor(float(a[0]), float(a[1]), float(a[2]));
        a = str(l.get('DiffuseColor')).split(" ")
        light.SetDiffuseColor(float(a[0]), float(a[1]), float(a[2]));
        light.SetIntensity(float(l.get('Intensity')))
        light.SetPositional(int(l.get('Positional')))
        light.SetConeAngle(float(l.get('ConeAngle')))
        
        if(l.get('Type') == "Scene"):
            light.SetLightTypeToSceneLight()
        elif(l.get('Type') == "Head"):
            light.SetLightTypeToHeadlight()
        elif(l.get('Type') == "Camera"):
            light.SetLightTypeToCameraLight()
        
        return light
        
    def importObject(self, o):
        print 'importing object: '+o.get('Name')+'...'
        
        if(o.get('Type') == 'Tool'):
            if(o.get('Model') == 'Probe'):
                object = vtkesqui.vtkToolProbe()
            elif(o.get('Model') == 'Grasper'):
                object = vtkesqui.vtkToolGrasper()
            else:
                object = vtkesqui.vtkToolDummy()
        elif(o.get('Type') == 'Organ'):
            object = vtkesqui.vtkOrgan()
        
        object.SetName(o.get('Name'))
        object.SetStatus(1)
        
        #Import object elements
        for child in o:
            if(child.tag == "Elements"):
                for e in child:
                    if (e.tag == "Element"):
                        element = self.importElement(e)
                        object.AddElement(element)
        
        return object
        
    def importElement(self, e):
        print '\telement: '+e.get('Name')+'...'
        element = vtkesqui.vtkScenarioElement()
        element.SetName(e.get('Name'))
        element.SetStatus(e.get('Status'))
        
        a = str(e.get('Scale')).split(" ")
        element.SetScale(float(a[0]), float(a[1]), float(a[2]));
        a = str(e.get('Origin')).split(" ")
        element.SetOrigin(float(a[0]), float(a[1]), float(a[2]));
        a = str(e.get('Position')).split(" ")
        element.SetPosition(float(a[0]), float(a[1]), float(a[2]));
        a = str(e.get('Orientation')).split(" ")
        element.SetOrientation(float(a[0]), float(a[1]), float(a[2]));
        #Import element models
        for child in e:
            if(child.tag == "Models"):
                for m in child:
                    if (m.tag == "Model"):
                        model = self.importModel(m)
                        if (m.get('Type') == 'Visualization'):
                            element.SetVisualizationModel(model)
                        elif (m.get('Type') == 'Collision'):
                            element.SetCollisionModel(model)
                        elif (m.get('Type') == 'Deformation'):
                            element.SetDeformationModel(model)
        return element
        
    def importModel(self, m):
        print '\t\tmodel: '+m.get('Name')+'...'
        
        if(m.get('Type') == 'Visualization'):
            model = vtkesqui.vtkVisualizationModel()
            model.SetTextureFileName(m.get('TextureFileName'))
        elif(m.get('Type') == 'Collision'):
            model = vtkesqui.vtkCollisionModel()
        elif(m.get('Type') == 'Deformation'):
            i = m.children[0]
            #model = vtkesqui.vtkVisualizationModel()
        else:
            return
        
        model.SetStatus(int(m.get('Status')))
        model.SetName(m.get('Name'))
        model.SetFileName(m.get('FileName'))
        
        a = str(m.get('Color')).split(" ")
        model.SetColor(float(a[0]), float(a[1]), float(a[2]))
        model.SetVisibility(float(m.get('Visibility')))
        model.SetOpacity(float(m.get('Opacity')))

        return model
    
    def process(self):
        if(self.srml == ""):
            self.parse()
        
        #process SRML file content
        root = self.srml.getroot()

        if(root.tag == 'Simulation'):
            print "\nImporting Simulation...\n"
            sim = self.simulation
            sim.SetUseHaptic(int(root.get('UseHaptic')))
            sim.SetRenderTimerRate(float(root.get('RenderRate')))
            sim.SetHapticTimerRate(float(root.get('HapticRate')))
            sim.SetSimulationTimerRate(float(root.get('SimulationRate')))
        
        for child in root:
            print "Importing "+child.tag+": "+child.get('Name')+"...\n"
            if(child.tag == 'Haptic'):
                if(child.get('Type') == 'IHP'):
                    haptic = vtkIHP()
                    haptic.SetNumberOfTools(child.get('NumberOfTools'))
            #elif(child.tag == 'Collision'):
                #collision = vtkCollisionDetection()
            elif(child.tag == 'Scenario'):
                sc = self.scenario
                #Set rendering parameters
                for el in child:
                    if(el.tag == 'Environment'):
                        #print el.tag
                        env = el;
                        bg = str(env.get('Background')).split(" ")
                        self.ren.SetBackground(float(bg[0]), float(bg[1]), float(bg[2]));
                        for e in env:
                            if(e.tag == 'Cameras'):
                                for c in e:
                                    cam = self.importCamera(c)
                                    self.ren.SetActiveCamera(cam)
                            if(e.tag == 'Lights'):
                                for l in e:
                                    light = self.importLight(l)
                                    self.ren.AddLight(light)
                    elif(el.tag == 'Objects'):
                        for o in el:
                            object = self.importObject(o)
                            sc.AddObject(object)
                            

def main():
    importer = SRMLImporter()
    importer.setFileName('/home/jballesteros/Workspace/src/vtkESQui/Utilities/IO/Python/out.srml')
    importer.parse()
    
    importer.process()
    
if __name__ == "__main__":
    main()