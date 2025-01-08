#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>
#include <string>

class GUI {
public:
    static void init();
    static void renderText(const char* text, int x, int y, Uint8 r, Uint8 g, Uint8 b);
    static void update();
    
private:
    static const int CHAR_WIDTH = 8;
    static const int CHAR_HEIGHT = 12;
    static void drawChar(char c, int x, int y, Uint8 r, Uint8 g, Uint8 b);
};

#endif 