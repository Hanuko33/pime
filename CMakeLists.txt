cmake_minimum_required(VERSION 3.10)
project(game_gui)

find_package(PkgConfig)
pkg_check_modules(SDL sdl2)
pkg_check_modules(SDL2_IMAGE SDL2_image)
pkg_check_modules(SDL2_TTF SDL2_ttf)
pkg_check_modules(SDL2_MIXER SDL2_mixer)

set(CMAKE_BUILD_TYPE Debug)

add_executable(game_gui window.c music.c text.c time.c texture.c dungeon.c cave.c main.c menu.c world.c player.c)

target_link_libraries(game_gui
    SDL2 SDL2_image SDL2_ttf SDL2_mixer
    ${SDL_LDFLAGS} 
    -lm 
)
