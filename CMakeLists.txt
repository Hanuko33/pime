cmake_minimum_required(VERSION 3.10)
project(game_gui)

find_package(PkgConfig)
pkg_check_modules(SDL sdl2)
pkg_check_modules(SDL2_IMAGE SDL2_image)
pkg_check_modules(SDL2_TTF SDL2_ttf)
pkg_check_modules(SDL2_MIXER SDL2_mixer)

set(CMAKE_BUILD_TYPE Debug)

add_executable(game_gui 
	window.cpp music.cpp text.cpp time.cpp 
	texture.cpp dungeon.cpp cave.cpp main.cpp menu.cpp world.cpp player.cpp 
	notifier.cpp 
	alchemist/elements.cpp
	alchemist/names.cpp
	alchemist/el_list.cpp
	OpenSimplex/OpenSimplex2F.c
	)

target_link_libraries(game_gui
    SDL2 SDL2_image SDL2_ttf SDL2_mixer
    ${SDL_LDFLAGS} ${SDL2_MIXER_LDFLAGS}
    -lm 
)
