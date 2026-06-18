cmake_minimum_required(VERSION 3.15)
project(TombOfTheMask)

set(CMAKE_CXX_STANDARD 17)

# Твой личный путь до папки с конфигурацией SFML (исправь под себя!):
set(SFML_DIR "C:/SFML-2.6.1/lib/cmake/SFML")

find_package(SFML 2.6.1 COMPONENTS graphics window system REQUIRED)

# Когда создашь файлы Map.cpp и Map.h, просто допиши их сюда через пробел:
add_executable(TombOfTheMask main.cpp Player.cpp Player.h Map.cpp Map.h Level.h)
    
target_link_libraries(TombOfTheMask sfml-graphics sfml-window sfml-system)