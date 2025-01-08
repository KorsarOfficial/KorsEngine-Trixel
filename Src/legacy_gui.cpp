#include "gui.h"
#include "screen.h"
#include <cstring>

// Простой растровый шрифт 8x8 (только для цифр и букв)
const unsigned char fontData[8] = {
    0xFF, // █████████
    0x81, // █      █
    0x81, // █      █
    0x81, // █      █
    0x81, // █      █
    0x81, // █      █
    0x81, // █      █
    0xFF  // █████████
};

void GUI::init() {
    // Инициализация не требуется
}

void GUI::drawChar(char c, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    // Рисуем символ как простой квадрат 8x8
    for(int py = 0; py < 8; py++) {
        for(int px = 0; px < 8; px++) {
            if(fontData[py] & (1 << px)) {
                drawPixel(x + px, y + py, r, g, b);
            }
        }
    }
}

void GUI::renderText(const char* text, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    int currentX = x;
    while(*text) {
        drawChar(*text, currentX, y, r, g, b);
        currentX += 10; // Расстояние между символами
        text++;
    }
}

void GUI::update() {
    // ASCII text only
    renderText("TRIXEL ENGINE v1.0", 10, 10, 255, 255, 255);
    renderText("WASD - Move", 10, 30, 200, 200, 200);
    renderText("MOUSE - Look around", 10, 50, 200, 200, 200);
    renderText("Q/E - Roll camera", 10, 70, 200, 200, 200);
    renderText("R - Reset view", 10, 90, 200, 200, 200);
    renderText("ESC - Exit", 10, 110, 200, 200, 200);
    renderText("FPS: 160", screenWidth - 100, 10, 255, 255, 0);
} 