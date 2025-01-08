#include "gui_system.h"
#include "gui_renderer.h"
#include <algorithm>

// Базовый класс Widget
Widget::Widget(WidgetType type, const std::string& name) 
    : type(type), name(name), visible(true), enabled(true), 
      hovered(false), focused(false), parent(nullptr) {
    
    position = Vector2(0.0f, 0.0f);
    size = Vector2(100.0f, 30.0f);
}

void Widget::Update(float deltaTime) {
    if (!visible || !enabled) return;
    
    for (auto child : children) {
        child->Update(deltaTime);
    }
}

void Widget::Render() {
    if (!visible) return;
    
    // Отрисовка фона виджета
    GUIRenderer::DrawFilledRect(position, size, style.backgroundColor);
    
    // Отрисовка границы
    if(style.borderWidth > 0) {
        GUIRenderer::DrawRect(position, size, style.borderColor);
    }
    
    // Отрисовка дочерних элементов
    for (auto child : children) {
        child->Render();
    }
}

bool Widget::HandleInput(int x, int y, bool clicked) {
    if (!visible || !enabled) return false;

    bool handled = false;
    
    // Проверяем, находится ли курсор над виджетом
    bool isHovered = x >= position.x && x <= position.x + size.x &&
                    y >= position.y && y <= position.y + size.y;

    if (isHovered != hovered) {
        hovered = isHovered;
        if (hovered && onHover) {
            onHover();
        }
    }

    if (isHovered && clicked) {
        focused = true;
        if (onClick) {
            onClick();
        }
        handled = true;
    } else if (clicked) {
        focused = false;
    }

    // Обрабатываем ввод для дочерних элементов
    for (auto it = children.rbegin(); it != children.rend(); ++it) {
        if ((*it)->HandleInput(x, y, clicked)) {
            handled = true;
            break;
        }
    }

    return handled;
}

void Widget::SetPosition(float x, float y) {
    position = Vector2(x, y);
}

void Widget::SetSize(float width, float height) {
    size = Vector2(width, height);
}

void Widget::SetVisible(bool isVisible) {
    visible = isVisible;
}

void Widget::SetEnabled(bool isEnabled) {
    enabled = isEnabled;
}

void Widget::SetStyle(const UIStyle& newStyle) {
    style = newStyle;
}

void Widget::SetOnClick(std::function<void()> callback) {
    onClick = callback;
}

void Widget::SetOnHover(std::function<void()> callback) {
    onHover = callback;
}

void Widget::SetOnValueChanged(std::function<void(const std::string&)> callback) {
    onValueChanged = callback;
}

void Widget::AddChild(Widget* child) {
    if (child && child->parent != this) {
        if (child->parent) {
            child->parent->RemoveChild(child);
        }
        children.push_back(child);
        child->parent = this;
    }
}

void Widget::RemoveChild(Widget* child) {
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        (*it)->parent = nullptr;
        children.erase(it);
    }
}

// Реализация Button
Button::Button(const std::string& text) 
    : Widget(WidgetType::BUTTON, "Button"), text(text) {
}

void Button::SetText(const std::string& newText) {
    text = newText;
}

void Button::Render() {
    if (!visible) return;
    
    // Отрисовка фона кнопки
    Vector3 bgColor = hovered ? style.backgroundColor * 1.2f : style.backgroundColor;
    GUIRenderer::DrawRoundedRect(position, size, style.cornerRadius, bgColor);
    
    // Отрисовка границы
    if(style.borderWidth > 0) {
        GUIRenderer::DrawRect(position, size, style.borderColor);
    }
    
    // Отрисовка текста по центру
    Vector2 textSize = GUIRenderer::GetTextSize(text);
    Vector2 textPos(
        position.x + (size.x - textSize.x) / 2,
        position.y + (size.y - textSize.y) / 2
    );
    GUIRenderer::DrawText(text, textPos, style.foregroundColor);
}

// Реализация TextBox
TextBox::TextBox() 
    : Widget(WidgetType::TEXTBOX, "TextBox"), cursorPosition(0) {
}

void TextBox::SetText(const std::string& newText) {
    text = newText;
    if (onValueChanged) {
        onValueChanged(text);
    }
}

std::string TextBox::GetText() const {
    return text;
}

bool TextBox::HandleInput(int x, int y, bool clicked) {
    if (!Widget::HandleInput(x, y, clicked)) return false;
    
    // TODO: Реализовать обработку ввода текста
    return true;
}

void TextBox::Render() {
    if (!visible) return;
    
    // Отрисовка фона
    GUIRenderer::DrawFilledRect(position, size, style.backgroundColor);
    
    // Отрисовка границы
    if(style.borderWidth > 0) {
        Vector3 borderColor = focused ? Vector3(0.4f, 0.6f, 1.0f) : style.borderColor;
        GUIRenderer::DrawRect(position, size, borderColor);
    }
    
    // Отрисовка текста
    Vector2 textPos(position.x + style.padding, position.y + (size.y - 12) / 2);
    GUIRenderer::DrawText(text, textPos, style.foregroundColor);
    
    // Отрисовка курсора если в фокусе
    if(focused) {
        float cursorX = textPos.x + GUIRenderer::GetTextSize(text.substr(0, cursorPosition)).x;
        GUIRenderer::DrawLine(
            Vector2(cursorX, position.y + 5),
            Vector2(cursorX, position.y + size.y - 5),
            style.foregroundColor
        );
    }
}

// Реализация Slider
Slider::Slider(float min, float max) 
    : Widget(WidgetType::SLIDER, "Slider"),
      minValue(min), maxValue(max), currentValue(min) {
}

void Slider::SetValue(float value) {
    currentValue = std::max(minValue, std::min(maxValue, value));
    if (onValueChanged) {
        onValueChanged(std::to_string(currentValue));
    }
}

float Slider::GetValue() const {
    return currentValue;
}

bool Slider::HandleInput(int x, int y, bool clicked) {
    if (!Widget::HandleInput(x, y, clicked)) return false;

    if (clicked && hovered) {
        float percentage = (x - position.x) / size.x;
        SetValue(minValue + (maxValue - minValue) * percentage);
        return true;
    }
    return false;
}

void Slider::Render() {
    if (!visible) return;
    
    // Отрисовка линии слайдера
    Vector2 lineStart(position.x + 5, position.y + size.y / 2);
    Vector2 lineEnd(position.x + size.x - 5, position.y + size.y / 2);
    GUIRenderer::DrawLine(lineStart, lineEnd, style.borderColor);
    
    // Отрисовка ползунка
    float percentage = (currentValue - minValue) / (maxValue - minValue);
    float knobX = position.x + 5 + percentage * (size.x - 10);
    Vector2 knobPos(knobX - 5, position.y + size.y / 2 - 5);
    Vector2 knobSize(10, 10);
    
    Vector3 knobColor = hovered ? style.backgroundColor * 1.2f : style.backgroundColor;
    GUIRenderer::DrawFilledRect(knobPos, knobSize, knobColor);
    GUIRenderer::DrawRect(knobPos, knobSize, style.borderColor);
    
    // Отрисовка значения
    std::string valueText = std::to_string(static_cast<int>(currentValue));
    Vector2 textSize = GUIRenderer::GetTextSize(valueText);
    Vector2 textPos(
        position.x + size.x + 5,
        position.y + (size.y - textSize.y) / 2
    );
    GUIRenderer::DrawText(valueText, textPos, style.foregroundColor);
}

// Реализация Window
Window::Window(const std::string& title) 
    : Widget(WidgetType::WINDOW, "Window"),
      title(title), draggable(true), resizable(true),
      dragging(false) {
    size = Vector2(400.0f, 300.0f);
}

void Window::SetTitle(const std::string& newTitle) {
    title = newTitle;
}

void Window::SetDraggable(bool isDraggable) {
    draggable = isDraggable;
}

void Window::SetResizable(bool isResizable) {
    resizable = isResizable;
}

bool Window::HandleInput(int x, int y, bool clicked) {
    if (!Widget::HandleInput(x, y, clicked)) return false;

    if (draggable) {
        bool inTitleBar = y >= position.y && y <= position.y + 30.0f;
        
        if (clicked && inTitleBar) {
            dragging = true;
            dragOffset = Vector2(x - position.x, y - position.y);
            return true;
        } else if (!clicked) {
            dragging = false;
        }

        if (dragging) {
            position = Vector2(x - dragOffset.x, y - dragOffset.y);
            return true;
        }
    }

    return false;
}

void Window::Render() {
    if (!visible) return;
    
    // Отрисовка тени
    GUIRenderer::DrawShadow(position, size, 5.0f);
    
    // Отрисовка фона окна
    GUIRenderer::DrawFilledRect(position, size, style.backgroundColor);
    
    // Отрисовка заголовка
    Vector2 titleBarSize(size.x, 30);
    GUIRenderer::DrawGradient(
        position,
        titleBarSize,
        style.backgroundColor * 1.1f,
        style.backgroundColor
    );
    
    // Отрисовка текста заголовка
    Vector2 titlePos(position.x + 10, position.y + 5);
    GUIRenderer::DrawText(title, titlePos, style.foregroundColor);
    
    // Отрисовка границы
    GUIRenderer::DrawRect(position, size, style.borderColor);
    GUIRenderer::DrawLine(
        Vector2(position.x, position.y + 30),
        Vector2(position.x + size.x, position.y + 30),
        style.borderColor
    );
    
    // Установка области отсечения для содержимого окна
    GUIRenderer::SetClipRect(
        Vector2(position.x, position.y + 30),
        Vector2(size.x, size.y - 30)
    );
    
    // Отрисовка дочерних элементов
    Widget::Render();
    
    // Сброс области отсечения
    GUIRenderer::ClearClipRect();
}

// Реализация GUISystem
GUISystem::GUISystem() : focusedWidget(nullptr), hoveredWidget(nullptr) {
    // Установка стиля по умолчанию
    defaultStyle.backgroundColor = Vector3(0.2f, 0.2f, 0.2f);
    defaultStyle.foregroundColor = Vector3(0.9f, 0.9f, 0.9f);
    defaultStyle.borderColor = Vector3(0.3f, 0.3f, 0.3f);
    defaultStyle.borderWidth = 1.0f;
    defaultStyle.cornerRadius = 3.0f;
    defaultStyle.padding = 5.0f;
    defaultStyle.margin = 3.0f;
}

GUISystem::~GUISystem() {
    for (auto widget : widgets) {
        delete widget;
    }
    widgets.clear();
}

void GUISystem::Update(float deltaTime) {
    for (auto widget : widgets) {
        widget->Update(deltaTime);
    }
}

void GUISystem::Render() {
    for (auto widget : widgets) {
        widget->Render();
    }
}

bool GUISystem::HandleInput(int x, int y, bool clicked) {
    bool handled = false;
    
    // Обрабатываем ввод в обратном порядке (сверху вниз)
    for (auto it = widgets.rbegin(); it != widgets.rend(); ++it) {
        if ((*it)->HandleInput(x, y, clicked)) {
            handled = true;
            break;
        }
    }
    
    return handled;
}

Button* GUISystem::CreateButton(const std::string& text) {
    auto button = new Button(text);
    button->SetStyle(defaultStyle);
    widgets.push_back(button);
    return button;
}

TextBox* GUISystem::CreateTextBox() {
    auto textBox = new TextBox();
    textBox->SetStyle(defaultStyle);
    widgets.push_back(textBox);
    return textBox;
}

Slider* GUISystem::CreateSlider(float min, float max) {
    auto slider = new Slider(min, max);
    slider->SetStyle(defaultStyle);
    widgets.push_back(slider);
    return slider;
}

Window* GUISystem::CreateWindow(const std::string& title) {
    auto window = new Window(title);
    window->SetStyle(defaultStyle);
    widgets.push_back(window);
    return window;
}

void GUISystem::SetDefaultStyle(const UIStyle& style) {
    defaultStyle = style;
}

void GUISystem::LoadTheme(const std::string& themeName) {
    // TODO: Реализовать загрузку тем из файла
} 