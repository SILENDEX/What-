cmake_minimum_required(VERSION 3.10)
project(WHAT)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2IMAGE REQUIRED SDL2_image)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
link_directories(${SDL2_LIBRARY_DIRS} ${SDL2IMAGE_LIBRARY_DIRS})

add_executable(what
    src/main.cpp
    src/Game.cpp
    src/Player.cpp
    src/Enemy.cpp
    src/Kunti.cpp
    src/Barrel.cpp
    src/Jumpscare.cpp
    src/Labirin.cpp
    src/utils.cpp
)

target_link_libraries(what ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
