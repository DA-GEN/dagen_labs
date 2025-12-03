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

# Google Test configuration
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(dungeon_tests
    tests/dungeon_tests.cpp
)

target_link_libraries(dungeon_tests
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(dungeon_tests)

