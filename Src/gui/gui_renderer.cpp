#include "gui_renderer.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <cmath>

bool GUIRenderer::initialized = false;
static SDL_Renderer* renderer = nullptr;

void GUIRenderer::Initialize() {
    if (!initialized) {
        // Рендерер будет установлен извне
        initialized = true;
    }
}

void GUIRenderer::Shutdown() {
    if (initialized) {
        renderer = nullptr;
        initialized = false;
    }
}

void GUIRenderer::SetRenderer(SDL_Renderer* sdlRenderer) {
    renderer = sdlRenderer;
}

void GUIRenderer::DrawFilledRect(const Vector2& position, const Vector2& size, const Vector3& color) {
    if (!renderer) return;

    SDL_SetRenderDrawColor(renderer, 
        static_cast<Uint8>(color.x * 255),
        static_cast<Uint8>(color.y * 255),
        static_cast<Uint8>(color.z * 255),
        255
    );

    SDL_Rect rect = {
        static_cast<int>(position.x),
        static_cast<int>(position.y),
        static_cast<int>(size.x),
        static_cast<int>(size.y)
    };

    SDL_RenderFillRect(renderer, &rect);
}

void GUIRenderer::DrawRect(const Vector2& position, const Vector2& size, const Vector3& color) {
    if (!renderer) return;

    SDL_SetRenderDrawColor(renderer, 
        static_cast<Uint8>(color.x * 255),
        static_cast<Uint8>(color.y * 255),
        static_cast<Uint8>(color.z * 255),
        255
    );

    SDL_Rect rect = {
        static_cast<int>(position.x),
        static_cast<int>(position.y),
        static_cast<int>(size.x),
        static_cast<int>(size.y)
    };

    SDL_RenderDrawRect(renderer, &rect);
}

void GUIRenderer::DrawRoundedRect(const Vector2& position, const Vector2& size, float radius, const Vector3& color) {
    if (!renderer) return;

    // Рисуем основной прямоугольник
    DrawFilledRect(
        Vector2(position.x + radius, position.y),
        Vector2(size.x - 2 * radius, size.y),
        color
    );

    DrawFilledRect(
        Vector2(position.x, position.y + radius),
        Vector2(size.x, size.y - 2 * radius),
        color
    );

    // Рисуем закругленные углы
    const int segments = 16;
    for (int i = 0; i < segments; i++) {
        float angle1 = 2.0f * M_PI * i / segments;
        float angle2 = 2.0f * M_PI * (i + 1) / segments;

        // Верхний левый угол
        SDL_RenderDrawLine(renderer,
            static_cast<int>(position.x + radius + radius * cos(angle1)),
            static_cast<int>(position.y + radius + radius * sin(angle1)),
            static_cast<int>(position.x + radius + radius * cos(angle2)),
            static_cast<int>(position.y + radius + radius * sin(angle2))
        );

        // Верхний правый угол
        SDL_RenderDrawLine(renderer,
            static_cast<int>(position.x + size.x - radius + radius * cos(angle1)),
            static_cast<int>(position.y + radius + radius * sin(angle1)),
            static_cast<int>(position.x + size.x - radius + radius * cos(angle2)),
            static_cast<int>(position.y + radius + radius * sin(angle2))
        );

        // Нижний левый угол
        SDL_RenderDrawLine(renderer,
            static_cast<int>(position.x + radius + radius * cos(angle1)),
            static_cast<int>(position.y + size.y - radius + radius * sin(angle1)),
            static_cast<int>(position.x + radius + radius * cos(angle2)),
            static_cast<int>(position.y + size.y - radius + radius * sin(angle2))
        );

        // Нижний правый угол
        SDL_RenderDrawLine(renderer,
            static_cast<int>(position.x + size.x - radius + radius * cos(angle1)),
            static_cast<int>(position.y + size.y - radius + radius * sin(angle1)),
            static_cast<int>(position.x + size.x - radius + radius * cos(angle2)),
            static_cast<int>(position.y + size.y - radius + radius * sin(angle2))
        );
    }
}

void GUIRenderer::DrawLine(const Vector2& start, const Vector2& end, const Vector3& color) {
    if (!renderer) return;

    SDL_SetRenderDrawColor(renderer, 
        static_cast<Uint8>(color.x * 255),
        static_cast<Uint8>(color.y * 255),
        static_cast<Uint8>(color.z * 255),
        255
    );

    SDL_RenderDrawLine(renderer,
        static_cast<int>(start.x),
        static_cast<int>(start.y),
        static_cast<int>(end.x),
        static_cast<int>(end.y)
    );
}

void GUIRenderer::DrawText(const std::string& text, const Vector2& position, const Vector3& color) {
    // Временная реализация - рисуем текст как прямоугольники
    float x = position.x;
    for (char c : text) {
        DrawFilledRect(Vector2(x, position.y), Vector2(8, 16), color);
        x += 10;
    }
}

void GUIRenderer::DrawGradient(const Vector2& position, const Vector2& size, const Vector3& colorStart, const Vector3& colorEnd) {
    if (!renderer) return;

    for (int y = 0; y < static_cast<int>(size.y); y++) {
        float t = y / size.y;
        Vector3 color = Vector3(
            colorStart.x * (1.0f - t) + colorEnd.x * t,
            colorStart.y * (1.0f - t) + colorEnd.y * t,
            colorStart.z * (1.0f - t) + colorEnd.z * t
        );

        DrawLine(
            Vector2(position.x, position.y + y),
            Vector2(position.x + size.x, position.y + y),
            color
        );
    }
}

void GUIRenderer::DrawShadow(const Vector2& position, const Vector2& size, float blur) {
    Vector3 shadowColor(0.0f, 0.0f, 0.0f);
    for (float i = 0; i < blur; i += 1.0f) {
        float alpha = 1.0f - (i / blur);
        DrawRect(
            Vector2(position.x + i, position.y + i),
            Vector2(size.x, size.y),
            shadowColor * alpha
        );
    }
}

Vector2 GUIRenderer::GetTextSize(const std::string& text) {
    return Vector2(text.length() * 10, 16);
}

void GUIRenderer::SetClipRect(const Vector2& position, const Vector2& size) {
    if (!renderer) return;

    SDL_Rect rect = {
        static_cast<int>(position.x),
        static_cast<int>(position.y),
        static_cast<int>(size.x),
        static_cast<int>(size.y)
    };

    SDL_RenderSetClipRect(renderer, &rect);
}

void GUIRenderer::ClearClipRect() {
    if (!renderer) return;
    SDL_RenderSetClipRect(renderer, nullptr);
} 