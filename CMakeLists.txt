cmake_minimum_required(VERSION 3.15)
project(TombOfTheMask)

set(CMAKE_CXX_STANDARD 17)


set(SFML_DIR "C:/SFML-2.6.1-windows-gcc-13.1.0-mingw-64-bit/SFML-2.6.1/lib/cmake/SFML")

find_package(SFML 2.6.1 COMPONENTS graphics window system REQUIRED)

#список файлов проекта 
add_executable(TombOfTheMask 
    main.cpp 
    Player.cpp 
    Player.h 
    Level.h
)

target_link_libraries(TombOfTheMask sfml-graphics sfml-window sfml-system)