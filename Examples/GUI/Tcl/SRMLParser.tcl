#--------------------
#Program: ESQUI
#Info and Bugs: {marf,rsantana}@ctm.ulpgc.es 
#-------------------------------------------
#
#Copyright (c) 2006, Center for Technology in Medicine, University of Las Palmas
#de Gran Canaria (Canary Islands, Spain).
#All rights reserved.
#
#This program is free software; you can redistribute it and/or modify it under 
#the terms of the Creative Commons' "by-attribution" license 
#(http://creativecommons.org/licenses/by/2.5).
#
#Redistribution and use in source and binary forms, with or without
#modification, are permitted provided that the following conditions are met:
#
#1) Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
#2) Redistributions in binary form must reproduce the above copyright 
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
#
#
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
#AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
#ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
#DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
#SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
#OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#=============================================================================
proc SRMLReadVersion0.1 {etiqueta fileName {verbose 1}} {
   
	# Returns list of tags on success else 0

	# Read file
	if {[catch {set fid [open $fileName r]} errmsg] == 1} {
	    puts $errmsg
	    tk_messagebox -message $errmsg
	    return 0
	}
	set srml [read $fid]
	close $fid

	set srml [string map { "= '" "= \{" ' "\}" "= \"" "= \{" "\"" "\}"} $srml]
	# Check that it's the right file type and version
	if {[regexp {<!DOCTYPE SRML SYSTEM SRML1.dtd>} $srml match] == 0} {
		set errmsg "The file is NOT SRML version 1.0"
		tk_messageBox -message $errmsg
		return 0
	}

	# Strip off everything but the body
	if {[regexp {<SRML>(.*)</SRML>} $srml match srml] == 0} {
		# There's no content in the file
		return ""
	}
	# Strip leading white space
	regsub "^\[\n\t \]*" $srml "" srml
	set tags1 ""
#	lappend tags1 "SceneToolsAdded"
	while {$srml != ""} {

		# Find next tag
		if {[regexp {^<([^ >]*)([^>]*)>([^<]*)} $srml match tag attr stuffing] == 0} {
			set errmsg "Invalid SRML file. Can't parse tags:\n$srml"
			puts "$errmsg"
			tk_messageBox -message "$errmsg"
			return 0
		}
		# Strip off this tag, so we can continue.
		if {[lsearch "$etiqueta /$etiqueta" $tag] != -1} {
#			puts dentro
			set str "<$tag>"
		} else {
#			puts dentro2
#			puts $tag
			set str "</$tag>"
		}
		set i [string first $str $srml]
		set srml [string range $srml [expr $i + [string length $str]] end]
		# Give the EndTransform tag a name
		if {$tag == "/$etiqueta"} {
			set tag $etiqueta
		}

	
		# Append to List of tags1
#		puts SceneToolsAdded
#		puts $tags1
		if { $tag != $etiqueta } {
		
			if {[lsearch $tags1 "$etiqueta"] == -1} {
				if {$attr != ""} {
				lappend tags1 "$tag" "{$attr}"
				} 
				}
		}

		# Strip leading white space
		regsub "^\[\n\t \]*" $srml "" srml
	
	}

	# Parse the attribute list for each tag
	set tags2 ""
	foreach pair $tags1 {
		set tag [lindex $pair 0]
		set attr [lindex $pair 1]

		# Add the options (the "stuffing" from inside the start and end tags)
		set attrList ""
#		lappend attrList "options $stuffing"

		# Strip leading white space
		regsub "^\[\n\t \]*" "$attr $stuffing" "" attr
		regsub "^\[\n\t \]*" "$attr" "" attr
		
		while {$attr != ""} {
		
			# Find the next key=value pair (and also strip it off... all in one step!)
			if {[regexp "^(\[^=\]*)\[\n\t \]*=\[\n\t \]*\['\"\](\[^'\"\]*)\['\"\](.*)$" \
				$attr match key value attr] == 0} {
				set errmsg "Invalid SRML file. Can't parse attributes:\n$attr"
				puts "$errmsg"
				tk_messageBox -message "$errmsg"
				return 0
			}
			lappend attrList "$key $value"

			# Strip leading white space
			regsub "^\[\n\t \]*" $attr "" attr
		}

		# Add this tag
		lappend tags2 "$tag $attrList"
	}
	lappend tags2 "End$etiqueta"
	return $tags2
}


# Este fichero extrae los datos del fichero generado por el generador
# de escenas, asignando sus valores a las distintas variables del 
# simulador. Muchas de ellas son listas, por lo que habrá que utilizar 
# tanto lindex que selecciona los elementos como llength que cuenta el
# numero de elementos en una lista
proc STAReadVersion0.1 {fileName {verbose 1}} {
   
	# Returns list of tags on success else 0

	# Read file
	if {[catch {set fid [open $fileName r]} errmsg] == 1} {
	    puts $errmsg
	    tk_messagebox -message $errmsg
	    return 0
	}
	set sta [read $fid]
	close $fid

	# Check that it's the right file type and version
	if {[regexp {<!DOCTYPE SceneToolsAdded SYSTEM 'Tools.dtd'>} $sta match] == 0} {
		set errmsg "The file is NOT STA version 1.0"
		tk_messageBox -message $errmsg
		return 0
	}

	# Strip off everything but the body
	if {[regexp {<SceneToolsAdded>(.*)</SceneToolsAdded>} $sta match sta] == 0} {
		# There's no content in the file
		return ""
	#}
	# Strip leading white space
	regsub "^\[\n\t \]*" $sta "" sta
	set tags1 ""
#	lappend tags1 "SceneToolsAdded"
	while {$sta != ""} {

		# Find next tag
		if {[regexp {^<([^ >]*)([^>]*)>([^<]*)} $sta match tag attr stuffing] == 0} {
			set errmsg "Invalid STA file. Can't parse tags:\n$sta"
			puts "$errmsg"
			tk_messageBox -message "$errmsg"
			return 0
		}
		# Strip off this tag, so we can continue.
		if {[lsearch "SceneToolsAdded /SceneToolsAdded" $tag] != -1} {
#			puts dentro
			set str "<$tag>"
		} else {
#			puts dentro2
#			puts $tag
			set str "</$tag>"
		}
		set i [string first $str $sta]
		set sta [string range $sta [expr $i + [string length $str]] end]
		# Give the EndTransform tag a name
		if {$tag == "/SceneToolsAdded"} {
			set tag EndSceneToolsAdded
		}

	
		# Append to List of tags1
#		puts SceneToolsAdded
#		puts $tags1
		if { $tag != "SceneToolsAdded" } {
		
			if {[lsearch $tags1 "EndSceneToolsAdded"] == -1} {
				if {$attr != ""} {
				lappend tags1 "$tag" "{$attr}"
				} 
				}
		}

		# Strip leading white space
		regsub "^\[\n\t \]*" $sta "" sta
	
	}

	# Parse the attribute list for each tag
	set tags2 ""
	foreach pair $tags1 {
		set tag [lindex $pair 0]
		set attr [lindex $pair 1]

		# Add the options (the "stuffing" from inside the start and end tags)
		set attrList ""
#		lappend attrList "options $stuffing"

		# Strip leading white space
		regsub "^\[\n\t \]*" "$attr $stuffing" "" attr
		regsub "^\[\n\t \]*" "$attr" "" attr
		
		while {$attr != ""} {
		
			# Find the next key=value pair (and also strip it off... all in one step!)
			if {[regexp "^(\[^=\]*)\[\n\t \]*=\[\n\t \]*\['\"\](\[^'\"\]*)\['\"\](.*)$" \
				$attr match key value attr] == 0} {
				set errmsg "Invalid STA file. Can't parse attributes:\n$attr"
				puts "$errmsg"
				tk_messageBox -message "$errmsg"
				return 0
			}
			lappend attrList "$key $value"

			# Strip leading white space
			regsub "^\[\n\t \]*" $attr "" attr
		}

		# Add this tag
		lappend tags2 "$tag $attrList"
	}
	lappend tags2 "EndSceneToolsAdded"
	return $tags2
}