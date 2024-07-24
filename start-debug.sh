#!/bin/bash

#./godot/bin/godot.linuxbsd.editor.x86_64 --verbose --rendering-driver opengl3 the_game/project.godot
./godot/bin/godot.linuxbsd.editor.x86_64 --verbose  the_game/project.godot &
pid=`pidof godot.linuxbsd.editor.x86_64`
cgdb -p $pid




