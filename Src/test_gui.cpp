#include <SDL2/SDL.h>
#include "gui/gui_system.h"
#include "gui/gui_renderer.h"

int main(int argc, char* argv[]) {
    // Инициализация SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL не смог инициализироваться! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Создание окна
    SDL_Window* window = SDL_CreateWindow(
        "Trixel Engine - GUI Test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1024,
        768,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        printf("Окно не может быть создано! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Создание рендерера
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Рендерер не может быть создан! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Инициализация GUI
    GUIRenderer::Initialize();
    GUIRenderer::SetRenderer(renderer);
    GUISystem gui;

    // Создание тестовых элементов интерфейса
    auto mainWindow = gui.CreateWindow("Тестовое окно");
    mainWindow->SetPosition(100, 100);
    mainWindow->SetSize(400, 300);

    auto button = gui.CreateButton("Тестовая кнопка");
    button->SetPosition(10, 40);
    button->SetOnClick([]() {
        printf("Кнопка нажата!\n");
    });
    mainWindow->AddChild(button);

    auto slider = gui.CreateSlider(0, 100);
    slider->SetPosition(10, 80);
    slider->SetOnValueChanged([](const std::string& value) {
        printf("Значение слайдера: %s\n", value.c_str());
    });
    mainWindow->AddChild(slider);

    auto textBox = gui.CreateTextBox();
    textBox->SetPosition(10, 120);
    textBox->SetSize(200, 30);
    textBox->SetText("Тестовый текст");
    mainWindow->AddChild(textBox);

    // Основной цикл
    bool running = true;
    SDL_Event event;

    while (running) {
        // Обработка событий
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    gui.HandleInput(event.button.x, event.button.y, true);
                    break;
                case SDL_MOUSEBUTTONUP:
                    gui.HandleInput(event.button.x, event.button.y, false);
                    break;
                case SDL_MOUSEMOTION:
                    gui.HandleInput(event.motion.x, event.motion.y, false);
                    break;
            }
        }

        // Очистка экрана
        SDL_SetRenderDrawColor(renderer, 45, 45, 48, 255);
        SDL_RenderClear(renderer);

        // Обновление и рендеринг GUI
        gui.Update(1.0f / 60.0f);
        gui.Render();

        // Отображение
        SDL_RenderPresent(renderer);

        // Задержка для ограничения FPS
        SDL_Delay(16); // примерно 60 FPS
    }

    // Очистка
    GUIRenderer::Shutdown();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
} 