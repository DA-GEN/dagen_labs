# Чиста C++17 бібліотека ігрової логіки. Не залежить від Qt і не містить
# вихідних .cpp файлів - усі класи з цілком визначеними шаблонами/невеликими
# методами залишаються header-only (як у вихідному проєкті), тож описуємо
# її як INTERFACE-бібліотеку: вона лише постачає шлях до заголовків і
# стандарт мови всім, хто на неї лінкується (тести, Qt-застосунок).
add_library(dungeon_core INTERFACE)

target_include_directories(dungeon_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(dungeon_core INTERFACE cxx_std_17)
