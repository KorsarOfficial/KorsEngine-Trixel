cmake_minimum_required(VERSION 3.10)

# Имя проекта
project(Trixel3DEngine)

# Устанавливаем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Находим SDL2
find_package(SDL2 REQUIRED)

# Исходные файлы движка
set(ENGINE_SOURCES
    Src/trixelmath.cpp
    Src/world.cpp
    Src/screen.cpp
    Src/input.cpp
    Src/collision.cpp
    Src/scene.cpp
    Src/init.cpp
    
    # Новые модули
    Src/gui/gui_system.cpp
    Src/gui/gui_renderer.cpp
    Src/physics/physics_system.cpp
    Src/effects/particle_system.cpp
    Src/graphics/lighting.cpp
)

# Создаем основную библиотеку движка
add_library(trixel_engine STATIC ${ENGINE_SOURCES})

# Добавляем включаемые директории
target_include_directories(trixel_engine PUBLIC
    ${SDL2_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/Src
)

# Линкуем SDL2
target_link_libraries(trixel_engine PUBLIC ${SDL2_LIBRARIES})

# Тестовое приложение GUI
add_executable(test_gui Src/test_gui.cpp)
target_link_libraries(test_gui PRIVATE trixel_engine)

# Основное приложение движка
add_executable(trixel_app Src/trixel.cpp)
target_link_libraries(trixel_app PRIVATE trixel_engine)