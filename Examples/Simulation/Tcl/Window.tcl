
proc InitWindow {} {
	
	puts "Setting up the splash window ...\n"
	
	# Setup splash window
	wm withdraw .
	toplevel .x -bd 3 -relief raised
	image create photo logovtkesqui -file [file join splash.gif]
	wm overrideredirect .x 1
	label .x.l -image logovtkesqui -bd 1 -relief sunken
	pack .x.l -side top -expand 1 -fill both
	set sw [winfo screenwidth .]
	set sh [winfo screenheight .]
	set ww [winfo reqwidth .x]
	set wh [winfo reqheight .x]
	set x [expr ($sw - $ww)/4 +$ww]
	set y [expr ($sh - $wh)/4]
	wm geometry .x +$x+$y
	after 4500 {destroy .x}
	tkwait window .x
	
}

proc LoadWindow {} {
	
	puts "\t Loading main window ...\n"
	
	### Setup main window appearance and functionality ###
	wm withdraw .
	toplevel .reader 
	wm protocol .reader WM_DELETE_WINDOW ::vtk::cb_exit
	
	# Set window title
	wm title .reader "vtkESQui" 
	frame .reader.mbar -relief raised -bd 2
	tk_setPalette LightGray
	
	# Set menu items
	menubutton .reader.mbar.file -text "File" -underline 0 -menu .reader.mbar.file.menu 
	menubutton .reader.mbar.srmltools -text "SRML Tools" -underline 0 -menu .reader.mbar.srmltools.menu 
	menubutton .reader.mbar.help -text Help -underline 0  -menu .reader.mbar.help.menu
	menu .reader.mbar.help.menu -activebackground black -activeforeground white -tearoff FALSE
	menu .reader.mbar.file.menu -activebackground black -activeforeground white -tearoff FALSE
	menu .reader.mbar.srmltools.menu -activebackground black -activeforeground white -tearoff FALSE
	pack .reader.mbar -side top -fill x
	pack .reader.mbar.help -side right -in .reader.mbar
	pack .reader.mbar.file .reader.mbar.srmltools -side left -in .reader.mbar
	frame .reader.window -height 600 -width 900
	pack .reader.window -side top
	set inicio [image create photo]
	canvas .reader.window.c -height 600 -width 900
	pack .reader.window.c
	
	#Set background image
	$inicio read Portada.gif
	.reader.window.c create image 450 300 -image $inicio
	
	## Add functionality to the menu items ##
	
	# Menu item -> Open
	.reader.mbar.file.menu add command -label Open -command {
	
		set types1 {
    	{{Scenarios}       {.srml}       }
			{{Scenarios}       {.uni}        }
			{{All Files}        *             }
		}
	 
		set name [tk_getOpenFile -initialdir Scenarios/ -filetypes $types1 ]
		set ext [file extension $name]
		set path [file dirname $name]
		if {[file exist $name]} {	
	
			if {$ext != ".srml"} { 
				set name [BuildSRML $name]
			}
    	ImportSimulation $path $name
		}
	}
	
	# Menu item -> Close
	.reader.mbar.file.menu add command -label Close -command {::vtk::cb_exit}

	# Menu item -> Make SRML
	.reader.mbar.srmltools.menu add command -label "Make SRML" -command {
		set types2 {
    	{{Scenarios}       {.uni}        }
    	{{All Files}        *             }
		}
		set filename [BuildSRML [tk_getOpenFile -initialdir Scenarios/ -filetypes $types2 ]]
	
		if {[catch {set fid [open $filename r]} errmsg] == 1} {
			puts $errmsg
	    tk_messagebox -message $errmsg
	    return 0
		}
		set srml [read $fid]
		close $fid
	
		toplevel .dialogtext
		wm title .dialogtext "filename SRML generado"
		frame .dialogtext.buttons
		pack .dialogtext.buttons -side bottom -fill x -pady 2m
		button .dialogtext.buttons.okeys -text OK -command {destroy .dialogtext}
		button .dialogtext.buttons.print -text Imprimir -command { exec lp $filename }
		pack .dialogtext.buttons.okeys -side bottom
		pack .dialogtext.buttons.print -side bottom
		text .dialogtext.text -relief sunken -bd 2 -yscrollcommand ".dialogtext.scroll set" -setgrid 1 \
		-height 30 -undo 1 -autosep 1
		scrollbar .dialogtext.scroll -command ".dialogtext.text yview"
		pack .dialogtext.scroll -side right -fill y
		pack .dialogtext.text -expand yes -fill both
		.dialogtext.text insert 0.0 $srml
	}

	#Menu item ModifySRML
	.reader.mbar.srmltools.menu add command -label "Modify SRML" -command {
	
		set types2 {
   		{{Scenarios}       {.srml}        }
		}
		set filename [tk_getOpenFile -initialdir Scenarios/ -filetypes $types2 ]
	
		if {[catch {set fid [open $filename r]} errmsg] == 1} {
	  	puts $errmsg
	    tk_messagebox -message $errmsg
	    return 0
		}
		set srml [read $fid]
		close $fid
	
		toplevel .dialogtext
		wm title .dialogtext "SRML File generated"
		frame .dialogtext.buttons
		pack .dialogtext.buttons -side bottom -fill x -pady 2m
		
		button .dialogtext.buttons.okeys -text Finalizar -command {
			set srml [.dialogtext.text get 0.0 end]
			set filename [tk_getSaveFile -initialdir Scenarios/ -filetypes $types2]
			set fich [open $filename w]
			puts $fich $srml
			close $fich
			destroy .dialogtext}
		
		button .dialogtext.buttons.print -text Print -command { exec lp $filename }
		pack .dialogtext.buttons.okeys -side bottom
		pack .dialogtext.buttons.print -side bottom
		text .dialogtext.text -relief sunken -bd 2 -yscrollcommand ".dialogtext.scroll set" -setgrid 1 \
		-height 30 -undo 1 -autosep 1
		scrollbar .dialogtext.scroll -command ".dialogtext.text yview"
		pack .dialogtext.scroll -side right -fill y
		pack .dialogtext.text -expand yes -fill both
		.dialogtext.text insert 0.0 $srml
	}
	puts "\t Main window loaded.\n"
	
}
