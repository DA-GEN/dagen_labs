cmake_minimum_required(VERSION 3.10)
project(DungeonCrawler VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(dungeon_game
    src/main.cpp
)

if(MSVC)
    target_compile_options(dungeon_game PRIVATE /W4)
else()
    target_compile_options(dungeon_game PRIVATE -Wall -Wextra -pedantic)
endif()

