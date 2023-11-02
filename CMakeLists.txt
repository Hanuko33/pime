cmake_minimum_required(VERSION 3.10)
project(game_new)

find_package(PkgConfig)
pkg_check_modules(SDL sdl2)
pkg_check_modules(SDL2_IMAGE SDL2_image>=2.0.0)
pkg_check_modules(SDL2_GFX SDL2_gfx)
pkg_check_modules(SDL2_TTF SDL2_ttf)

set(CMAKE_BUILD_TYPE Debug)

add_executable(game_new window.c text.c texture.c dungeon.c main.c menu.c)

target_link_libraries(game_new
    SDL2 SDL2_gfx SDL2_image SDL2_ttf
    ${SDL_LDFLAGS} 
    -lm 
)
