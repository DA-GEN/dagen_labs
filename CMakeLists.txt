cmake_minimum_required(VERSION 3.14)
project(DungeonGame VERSION 2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Бібліотека ігрової логіки (без Qt) - "To-Be" архітектурний поділ.
# Цей CMake-проєкт навмисно НЕ збирає Qt-застосунок (для цього є
# dungeonqt.pro/qmake) - його єдина мета: зібрати незалежну від UI
# бібліотеку та її юніт/інтеграційні тести.
add_subdirectory(core)

option(DUNGEON_BUILD_TESTS "Build unit tests for dungeon_core" ON)

if(DUNGEON_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
