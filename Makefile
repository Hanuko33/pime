all: game_gui alchemist

game_gui:
	scons debug_symbols=yes platform=linux -j `nproc`

alchemist:
	make -C src/alchemist-tui

clean:
	scons -c -j `nproc`
	make -C src/alchemist-tui clean

