cmake_minimum_required(VERSION 3.10)
project(pime)

find_package(PkgConfig)
pkg_check_modules(SDL sdl2)
pkg_check_modules(SDL2_IMAGE SDL2_image)
pkg_check_modules(SDL2_TTF SDL2_ttf)
pkg_check_modules(SDL2_MIXER SDL2_mixer)

set(CMAKE_BUILD_TYPE Debug)

add_executable(pime 
	window.cpp music.cpp text.cpp  
	texture.cpp 
    # dungeon.cpp 
    # cave.cpp
    main.cpp menu.cpp world.cpp player.cpp 
	notifier.cpp craft.cpp 

    alchemist/elements.cpp
	alchemist/names.cpp
	alchemist/el_list.cpp
    alchemist/game_time.cpp

    alchemist/axe_blade.cpp
    alchemist/axe_handle.cpp
    alchemist/axe.cpp

    alchemist/knife_blade.cpp
    alchemist/knife_handle.cpp
    alchemist/knife.cpp

    OpenSimplex/OpenSimplex2F.c
	)

target_link_libraries(pime
    SDL2 SDL2_image SDL2_ttf SDL2_mixer
    ${SDL_LDFLAGS} ${SDL2_MIXER_LDFLAGS}
    -lm 
)
