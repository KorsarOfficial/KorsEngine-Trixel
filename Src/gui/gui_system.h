#ifndef __GUI_SYSTEM_H__
#define __GUI_SYSTEM_H__

#include <vector>
#include <string>
#include <functional>
#include "../trixelmath.h"

enum class WidgetType {
    PANEL,
    BUTTON,
    LABEL,
    TEXTBOX,
    CHECKBOX,
    SLIDER,
    DROPDOWN,
    WINDOW
};

struct UIStyle {
    Vector3 backgroundColor;
    Vector3 foregroundColor;
    Vector3 borderColor;
    float borderWidth;
    float cornerRadius;
    float padding;
    float margin;
};

class Widget {
public:
    Widget(WidgetType type, const std::string& name);
    virtual ~Widget() = default;

    virtual void Update(float deltaTime);
    virtual void Render();
    virtual bool HandleInput(int x, int y, bool clicked);

    // Позиционирование и размеры
    void SetPosition(float x, float y);
    void SetSize(float width, float height);
    void SetVisible(bool visible);
    void SetEnabled(bool enabled);

    // Стилизация
    void SetStyle(const UIStyle& style);
    
    // Обработчики событий
    void SetOnClick(std::function<void()> callback);
    void SetOnHover(std::function<void()> callback);
    void SetOnValueChanged(std::function<void(const std::string&)> callback);

    // Иерархия виджетов
    void AddChild(Widget* child);
    void RemoveChild(Widget* child);

protected:
    WidgetType type;
    std::string name;
    Vector2 position;
    Vector2 size;
    bool visible;
    bool enabled;
    bool hovered;
    bool focused;
    
    UIStyle style;
    std::vector<Widget*> children;
    Widget* parent;

    std::function<void()> onClick;
    std::function<void()> onHover;
    std::function<void(const std::string&)> onValueChanged;
};

// Специализированные виджеты
class Button : public Widget {
public:
    Button(const std::string& text);
    void SetText(const std::string& text);
    virtual void Render() override;

private:
    std::string text;
};

class TextBox : public Widget {
public:
    TextBox();
    void SetText(const std::string& text);
    std::string GetText() const;
    virtual bool HandleInput(int x, int y, bool clicked) override;
    virtual void Render() override;

private:
    std::string text;
    int cursorPosition;
};

class Slider : public Widget {
public:
    Slider(float min, float max);
    void SetValue(float value);
    float GetValue() const;
    virtual bool HandleInput(int x, int y, bool clicked) override;
    virtual void Render() override;

private:
    float minValue;
    float maxValue;
    float currentValue;
};

class Window : public Widget {
public:
    Window(const std::string& title);
    void SetTitle(const std::string& title);
    void SetDraggable(bool draggable);
    void SetResizable(bool resizable);
    virtual bool HandleInput(int x, int y, bool clicked) override;
    virtual void Render() override;

private:
    std::string title;
    bool draggable;
    bool resizable;
    bool dragging;
    Vector2 dragOffset;
};

class GUISystem {
public:
    GUISystem();
    ~GUISystem();

    void Update(float deltaTime);
    void Render();
    bool HandleInput(int x, int y, bool clicked);

    // Создание виджетов
    Button* CreateButton(const std::string& text);
    TextBox* CreateTextBox();
    Slider* CreateSlider(float min, float max);
    Window* CreateWindow(const std::string& title);

    // Управление темой
    void SetDefaultStyle(const UIStyle& style);
    void LoadTheme(const std::string& themeName);

private:
    std::vector<Widget*> widgets;
    UIStyle defaultStyle;
    Widget* focusedWidget;
    Widget* hoveredWidget;
};

#endif 