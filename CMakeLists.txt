cmake_minimum_required(VERSION 3.10)

# Укажите имя проекта
project(Trixel3DEngine)

# Укажите стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Найдите SDL2
find_package(SDL2 REQUIRED PATHS "C:/msys64/mingw64/include/SDL2")

# Укажите исходные файлы
set(SOURCES
    Src/init.cpp
    Src/input.cpp
    Src/screen.cpp
    Src/trixel.cpp
    Src/trixelmath.cpp
    Src/world.cpp
    Src/gui.cpp
    Src/collision.cpp
    Src/scene.cpp
)

# Создайте исполняемый файл
add_executable(trixel ${SOURCES})

# Свяжите библиотеку SDL2 с вашим проектом
target_include_directories(trixel PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(trixel ${SDL2_LIBRARIES})