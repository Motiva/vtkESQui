# Configura los AddEvents para interactuar
# con la escena
#
#
proc AddEvents {} {
    global mouseLeft; set mouseLeft 0
    global mouseRight; set mouseRight 0
    global cam; set cam 1
    global ActiveTool; set ActiveTool 0
    set iren [renWin GetInteractor]
    $iren SetInteractorStyle ""
    #
    # Add the observers to watch for particular events. These invoke
    # Tcl procedures.
    #
    $iren AddObserver LeftButtonPressEvent {global mouseLeft; set mouseLeft 1}
    $iren AddObserver LeftButtonReleaseEvent {global mouseLeft; set mouseLeft 0}
    $iren AddObserver RightButtonPressEvent {global mouseRight; set mouseRight 1}
    $iren AddObserver RightButtonReleaseEvent {global mouseRight; set mouseRight 0}
    $iren AddObserver KeyPressEvent KeyPress
    $iren AddObserver MouseMoveEvent UpdateScenario
    $iren Initialize
    #-------------------------------------------------
}

proc UpdateScenario {} {
    set iren [renWin GetInteractor]
    global cam mouseLeft mouseRight ActiveTool
    set lastX [lindex [$iren GetLastEventPosition] 0]
    set lastY [lindex [$iren GetLastEventPosition] 1]
    set x [lindex [$iren GetEventPosition] 0]
    set y [lindex [$iren GetEventPosition] 1]

    if {[expr $mouseLeft | $mouseRight] } {
	if {$cam} {
	    UpdateCamera $mouseLeft $mouseRight $x $y $lastX $lastY
	} else {
	    UpdateTool $ActiveTool $mouseLeft $mouseRight $x $y $lastX $lastY
	}
    }
}
proc UpdateCamera {mouseLeft mouseRight x y lastX lastY} {
    if { $mouseLeft } {
	SetCameraPosition  ren1 [ren1 GetActiveCamera] $x $y $lastX $lastY
    } elseif { $mouseRight } {
	SetCameraDepth ren1 [ren1 GetActiveCamera] $x $y $lastX $lastY
    }
}

proc UpdateTool {ActiveTool mouseLeft mouseRight x y lastX lastY } {

    if { $mouseLeft } {
	SetToolRotation [Scenario GetTool $ActiveTool] $x $y $lastX $lastY
    } elseif { $mouseRight } {
	SetToolDepth [Scenario GetTool $ActiveTool] $x $y $lastX $lastY
    }	

}

proc SetCameraPosition {render camera x y lastX lastY} {
    set mouseLeft [expr [expr $lastX - $x] * 0.05]
    set mouseRight [expr [expr  $lastY - $y ] * 0.05]
    set posicion [$camera GetPosition]
    $camera Yaw $mouseLeft
    $camera Pitch $mouseRight
    renWin Render
}

proc SetCameraAzimuth {camera angle} {
    $camera Azimuth $angle
}

proc SetCameraDepth {render camera x y lastX lastY} {

    if {[expr  $lastY - $y ] < 0} {
	$camera Dolly 0.95
    } else {
	$camera Dolly 1.05
    }
    renWin Render
}

proc SetToolRotation {Tool x y lastX lastY} {

    set moveX [expr [expr $lastX - $x] * 0.1]
    set moveY [expr [expr $y - $lastY] * 0.1]

    puts "$moveX $moveY"

    $Tool RotateY $moveX
    $Tool RotateX $moveY

    renWin Render
}
proc SetToolDepth {Tool x y lastX lastY} {

    global ActiveTool

    set mouseRight [expr [expr  $lastY - $y ] * 0.05]
    set d [$Tool GetDepth]
    $Tool SetDepth [expr $d - $mouseRight ]
    renWin Render
}

proc SetToolOpening {State} {

    global cam
    global ActiveTool

    if {!$cam} {
	set tool [Scenario GetTool $ActiveTool]
	$tool SetOpening $State
    }
}

proc KeyPress {} {
    set iren [renWin GetInteractor]
    set tecla [$iren GetKeySym]
    global cam ActiveTool Opened0 Opened1

    set pi 3.1415926535897931

    switch $tecla {

	"c" {
	    #global cam
	    if {$cam} {
		set cam 0
	    } else {
		set cam 1
	    }
	    #puts "\tCamera Selected: $cam \n"
	}
	"i" {
	    #avanza herramienta de la derecha
	    set ActiveTool 0
	    #puts "\tLeft-hand Tool Selected\n"
	}
	"d" {
	    #moviento mouseLeft mouseRight de la herramienta de la izquierda
	    set ActiveTool 1
	    #puts "\tRight-hand Tool Selected\n"
	}
	"v" {
	    SetCameraAzimuth [ren1 GetActiveCamera] -5
	}
	"b" {
	    SetCameraAzimuth [ren1 GetActiveCamera] 5
	}
	"x" {
	    if {!$cam} {
		set tool [Scenario GetTool $ActiveTool]
		set angle -10
		$tool RotateZ $angle
		renWin Render
		#puts "\tTool Rotated Right\n"
	    }
	}
	"z" {
	    if {!$cam} {
		set tool [Scenario GetTool $ActiveTool]
		set angle 10
		$tool RotateZ $angle
		renWin Render
		#puts "\tTool Rotated Left\n"
	    }
	}
	"a" {
	    SetToolOpening 1
	    renWin Render
	    #puts "\tTool State: Open\n"
	}
	"s" {
	    SetToolOpening 0
	    renWin Render
	    #puts "\tTool State: Closed\n"
	}
	"u" {
	    wm deiconify .vtkInteract
	}

	"q" {::vtk::cb_exit
	    exit}
    }
}	
