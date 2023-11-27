cmake_minimum_required(VERSION 3.10)
project(game_gui)

find_package(PkgConfig)
pkg_check_modules(SDL sdl2)
pkg_check_modules(SDL2_IMAGE SDL2_image>=2.0.0)
pkg_check_modules(SDL2_GFX SDL2_gfx)
pkg_check_modules(SDL2_TTF SDL2_ttf)

set(CMAKE_BUILD_TYPE Debug)

add_executable(game_gui window.c text.c time.c texture.c dungeon.c main.c menu.c)

target_link_libraries(game_gui
    SDL2 SDL2_gfx SDL2_image SDL2_ttf
    ${SDL_LDFLAGS} 
    -lm 
)
