=====================
vtkESQui User's Guide
=====================

Introduction
============

vtkESQui is a vtk-based software project. The distribution follows the standard
structure for vtk's projects.

This release includes the following folders:

* BioMechanics: Biomechanical models coded under vtk classes.
* CollisionDetection: Collision detection models coded under vtk classes.
* Common: Project wide vtk classes needed by the project.
* Documentation: Documentation of the platform: Doxygen, user & developer guides.
* Examples: A list of examples implemented in C++ and Tcl.
* Haptics: vtk classes that wrap the haptic's API.
* Scenario: vtk classes that manage the rendered's objects.
* Simulation: vtk classes that manage the software pipeline.
* Utilities: Scripts and C++ classes to share/export/import models from other packages used in vtkESQui (i.e. Blender).
* Wrapping: The vtk's code for wrapping under Tcl.

Installation
============

Requirements
------------

In order to use this software the following packages must be installed:

* VTK. version > 5.6
* Tcl/Tk. version > 8.4
* CMake. version > 2.5

This software has been compiled and tested in:

* Windows (Visual Studio 2005 and Visual Studio 2008)
* Unix (gcc > 4.4).

Download
--------

There are multiple ways to get the software: 
* Download the latest release at http://motivando.me/vtkESqui.html
* Access the git source-code repository. git://aecio/vtkESQui.git

Configuring
-----------

As a vtk-based project, vtkESQui configuration and compilation is done using CMake (http://cmake.org). CMake is a cross-platform, open-source build system that automatically generates makefiles and workspaces for later compilation.

1. Once the source code has been downloaded and extracted/saved into a directory, let's name it VTKESQUI_DIR, a new binary directory has to be created.

 In Unix-like systems:: 

  $ mkdir vtkESQuiBin
  $ cd vtkESQuiBin
  $ ccmake ../VTKESQUI_DIR

 In Windows, the CMake GUI is used to define source and binary dir.

2. Follow the ccmake configuration. Remind to set the VTK_DIR path (the folder where you have compiled vtk and you have your VTKConfig.cmake file), and the Tcl/Tk libraries path. If you would like to use the software with haptics (LWS-Immersion Corp, IHP,etc), please set the CMake variable VTKESQUI_USE_HAPTICS to ON. The wrapping and other configuration are done by default (they are included in the CMakelist.txt file).

.. figure:: figures/cmake.png
   :height: 362px
   :width: 577px
   :scale: 200%
   :align: center

   CMake configuration options

3. Finally, after configuring the CMake options:

 In Unix-like systems::

  Press 'c' to configurate project and then 'g' to generate the makefiles

 In Windows::

  Press Configure button and then Generate button.

Building
--------

Once you have succesfully configured the project, the building process is started by:

 In Unix::

  $ make
 
 If anything went wrong, you may have vtkESQui platform compiled. If you want to install vtkESQui in your operating system you must type::

  $ make install

 In Windows you must import the project in Visual Studio, and compile it as usual.

Setting up the environment
--------------------------

Set the environment variables: LD_LIBRARY_PATH and TCLLIBPATH to
the path where are the (.so/dlls) vtk/wrapping libraries.
If you have other Tcl/Tk versions, you should declare the TK_LIBRARY
and TCL_LIBRARY variables.

 i.e. (under linux)::

  export LD_LIBRARY_PATH=/usr/local/lib/vtk-5.9:/home/user/software/ESQUI/EsquiBin/bin/
  export TCLLIBPATH=/home/user/software/ESQUI/ESQUI/Wrapping/Tcl

