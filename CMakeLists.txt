cmake_minimum_required(VERSION 3.10)
project(game_new)

find_package(PkgConfig)
pkg_check_modules(SDL sdl2)
pkg_check_modules(SDL2_IMAGE SDL2_image)
pkg_check_modules(SDL2_GFX SDL2_gfx)

set(CMAKE_BUILD_TYPE Debug)

add_executable(game_new window.cpp main.cpp)

target_link_libraries(game_new
     SDL2 SDL2_gfx SDL2_image
    ${SDL_LDFLAGS} 
    -lm -lpthread
)
