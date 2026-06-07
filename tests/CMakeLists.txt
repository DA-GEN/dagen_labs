# Якщо у системі вже є встановлений GoogleTest - використовуємо його,
# інакше тягнемо потрібну версію через FetchContent (як у попередній
# лабораторній роботі - зберігаємо узгоджений підхід у репозиторії).
find_package(GTest QUIET)

if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

add_executable(dungeon_core_tests
    dungeon_core_tests.cpp
)

target_link_libraries(dungeon_core_tests
    PRIVATE
        dungeon_core
        GTest::gtest_main
)

if(MSVC)
    target_compile_options(dungeon_core_tests PRIVATE /W4)
else()
    target_compile_options(dungeon_core_tests PRIVATE -Wall -Wextra -pedantic)
endif()

include(GoogleTest)
gtest_discover_tests(dungeon_core_tests)
