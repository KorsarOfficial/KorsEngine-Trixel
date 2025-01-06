/*
Trixel 3D Engine

Copyright (C) 2002 Lode Vandevenne (lode@vandevenne.net)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc.,
59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

/*
This file contains all functions related to the screen.  These include
setting up the SDL surface, plotting a pixel and clearing the screen.
*/

#include "screen.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;
Uint32* pixels = NULL;
int pitch;

void setScreen(int width, int height, int colorDepth)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);

    window = SDL_CreateWindow("Trixel",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            width, height,
                            SDL_WINDOW_SHOWN);
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer,
                               SDL_PIXELFORMAT_ARGB8888,
                               SDL_TEXTUREACCESS_STREAMING,
                               width, height);
    
    pixels = new Uint32[width * height];
}

void drawPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
    if(x >= 0 && x < screenWidth && y >= 0 && y < screenHeight) {
        pixels[y * screenWidth + x] = (R << 16) | (G << 8) | B;
    }
}

void updateScreen(int u, int v, int width, int height)
{
    SDL_UpdateTexture(texture, NULL, pixels, screenWidth * sizeof(Uint32));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void clearScreen()
{
    for (int x=0;x<screenWidth;x++)
    {
     for (int y = 0; y<screenHeight;y++)
     {
            drawPixel(x,y,0,0,0);
     }
    }
}

void cleanupScreen() {
    if (pixels) {
        delete[] pixels;
        pixels = nullptr;
    }
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}
