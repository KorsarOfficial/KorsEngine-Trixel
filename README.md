# KorsEngine-Trixel
# Trixel 3D Engine

## Описание проекта

Trixel 3D Engine — это мощный и гибкий движок для создания 3D-игр, разработанный с использованием C++ и библиотеки SDL2. Данный проект предоставляет разработчикам инструменты для создания интерактивных 3D-мира с поддержкой различных графических и игровых функций. Trixel 3D Engine ориентирован на простоту использования и высокую производительность.

## Основные функции

### 1. Инициализация SDL
Функция `SDL_Init` отвечает за инициализацию библиотеки SDL, необходимой для работы с графикой и вводом. Она обеспечивает настройку видеоподсистемы и управление событиями, что позволяет разработчикам сосредоточиться на создании контента.

### 2. Настройка экрана
Функция `setScreen` позволяет установить размеры окна и глубину цвета, обеспечивая необходимую конфигурацию для отображения графики. Это позволяет легко адаптировать игру под различные разрешения и форматы экранов.

### 3. Построение мира
Функция `buildWorld` создает начальную структуру игрового мира, включая инициализацию всех объектов и их свойств. Это позволяет разработчикам легко настраивать и изменять мир игры, добавляя новые элементы и механики.

### 4. Цикл игры
Функция `gameLoop` реализует основной цикл игры, который обрабатывает события, обновляет состояние игры и отрисовывает графику. Она обеспечивает плавный игровой процесс и взаимодействие с пользователем, позволяя игрокам наслаждаться игрой без задержек.

### 5. Обработка ввода
Функция `userInput` отвечает за обработку пользовательского ввода, включая клавиатуру и мышь. Это позволяет игрокам взаимодействовать с игровым миром, управляя персонажем и выполняя действия.

### 6. Обновление экрана
Функция `updateScreen` отвечает за обновление содержимого экрана, обеспечивая отображение текущего состояния игры. Она оптимизирует производительность и качество графики, что позволяет создавать визуально привлекательные сцены.

### 7. Очистка ресурсов
Функция `cleanupScreen` освобождает ресурсы, используемые для отображения, и завершает работу SDL. Это гарантирует корректное завершение игры и предотвращает утечки памяти, что особенно важно для долгосрочных проектов.

## Установка

Для установки Trixel 3D Engine выполните следующие шаги:

1. **Клонируйте репозиторий**:
   ```bash
   git clone https://github.com/ваш_пользователь/Trixel3DEngine.git
   ```

2. **Перейдите в каталог проекта**:
   ```bash
   cd Trixel3DEngine
   ```

3. **Создайте каталог для сборки**:
   ```bash
   mkdir build
   cd build
   ```

4. **Запустите CMake**:
   ```bash
   cmake .. -G "MinGW Makefiles"
   ```

5. **Соберите проект**:
   ```bash
   cmake --build .
   ```

## Использование

После успешной сборки вы можете запустить проект, выполнив следующую команду в каталоге `build`:


# Trixel 3D Engine

## Project Description

Trixel 3D Engine is a powerful and flexible engine for creating 3D games, developed using C++ and the SDL2 library. This project provides developers with tools to create interactive 3D worlds with support for various graphical and gameplay features. Trixel 3D Engine is designed for ease of use and high performance, making it an ideal choice for game developers.

## Key Features

### 1. SDL Initialization
The `SDL_Init` function is responsible for initializing the SDL library, which is necessary for graphics and input handling. It sets up the video subsystem and event management, allowing developers to focus on content creation.

### 2. Screen Setup
The `setScreen` function allows you to set the window size and color depth, providing the necessary configuration for displaying graphics. This makes it easy to adapt the game to various resolutions and screen formats.

### 3. World Building
The `buildWorld` function creates the initial structure of the game world, including the initialization of all objects and their properties. This allows developers to easily configure and modify the game world by adding new elements and mechanics.

### 4. Game Loop
The `gameLoop` function implements the main game loop, which processes events, updates the game state, and renders graphics. It ensures a smooth gaming experience and user interaction, allowing players to enjoy the game without delays.

### 5. Input Handling
The `userInput` function is responsible for processing user input, including keyboard and mouse actions. This allows players to interact with the game world, controlling characters and performing actions.

### 6. Screen Update
The `updateScreen` function is responsible for updating the screen content, ensuring the current state of the game is displayed. It optimizes performance and graphic quality, allowing for visually appealing scenes.

### 7. Resource Cleanup
The `cleanupScreen` function frees resources used for rendering and terminates SDL. This ensures the game ends correctly and prevents memory leaks, which is especially important for long-term projects.

## Installation

To install Trixel 3D Engine, follow these steps:

1. **Clone the repository**:
   ```bash
   git clone https://github.com/your_username/Trixel3DEngine.git
   ```

2. **Navigate to the project directory**:
   ```bash
   cd Trixel3DEngine
   ```

3. **Create a build directory**:
   ```bash
   mkdir build
   cd build
   ```

4. **Run CMake**:
   ```bash
   cmake .. -G "MinGW Makefiles"
   ```

5. **Build the project**:
   ```bash
   cmake --build .
   ```

## Usage

After a successful build, you can run the project by executing the following command in the `build` directory:

```bash
./trixel
```
bash
./trixel


