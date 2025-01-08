#ifndef __GUI_RENDERER_H__
#define __GUI_RENDERER_H__

#include "../trixelmath.h"
#include <SDL2/SDL.h>
#include <string>

class GUIRenderer {
public:
    static void Initialize();
    static void Shutdown();
    static void SetRenderer(SDL_Renderer* renderer);

    // Примитивы рендеринга
    static void DrawFilledRect(const Vector2& position, const Vector2& size, const Vector3& color);
    static void DrawRect(const Vector2& position, const Vector2& size, const Vector3& color);
    static void DrawRoundedRect(const Vector2& position, const Vector2& size, float radius, const Vector3& color);
    static void DrawLine(const Vector2& start, const Vector2& end, const Vector3& color);
    static void DrawText(const std::string& text, const Vector2& position, const Vector3& color);
    static void DrawGradient(const Vector2& position, const Vector2& size, const Vector3& colorStart, const Vector3& colorEnd);
    static void DrawShadow(const Vector2& position, const Vector2& size, float blur);

    // Вспомогательные функции
    static Vector2 GetTextSize(const std::string& text);
    static void SetClipRect(const Vector2& position, const Vector2& size);
    static void ClearClipRect();

private:
    static bool initialized;
};

#endif 