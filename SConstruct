#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/"])
#env.Append(CXXFLAGS = '-g')
#env.Append(CPPPATH=["game_gui/"])
sources = Glob("src/*.cpp") + ["game_gui/world.c", "game_gui/player.c", "game_gui/dungeon.c", "game_gui/cave.c"]

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "the_game/bin/libgdexample.{}.{}.framework/libgdexample.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "the_game/bin/libgdexample{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
