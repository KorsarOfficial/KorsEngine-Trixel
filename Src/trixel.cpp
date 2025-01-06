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
This file contains the gameloop and the raycasting function.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "init.h"
#include "world.h"
#include "trixel.h"
#include "input.h"
#include "extern.h"
#include "screen.h"
#include "gui.h"
#include "optimization/cpu_utils.h"
#include "optimization/gpu_utils.h"


SDL_Event event;


//Definition of some external variables:
double uDraw = 640;
double vDraw = 480;
camera cam = {2,14,14, 1,0,0, 0,0,0.75, 0,1,0};
camera defaultCam = {32,32,32, 1,0,0, 0,0,0.75, 0,1,0};
bool gameShouldEnd = false; //gebruikt voor muis terug te krijgen op einde


//The gameloop; this loop draws the scene, updates the screen and gets input commands from the user each frame.  The loop ends when the user ends the game.

void gameLoop()
{
    int done=0;
    while(done == 0)
    {
         while ( SDL_PollEvent(&event) )
         {
               if ( event.type == SDL_QUIT || gameShouldEnd ) done = 1;
         }
         cameraCorrection();
         rayCast();
         
         // Обновляем экран через существующую функцию
         updateScreen(static_cast<int>(screenWidth/2-uDraw/2),
                     static_cast<int>(screenHeight/2-vDraw/2),
                     static_cast<int>(uDraw),
                     static_cast<int>(vDraw));
         
         // Отрисовка GUI до SDL_RenderPresent
         GUI::update();
         
         // Финальное обновление экрана
         SDL_RenderPresent(renderer);
         
         userInput();
    }
}

void cameraCorrection()
{
       //There are some problems if the position of the camera is an exact integer position, so we move it a little bit if that's the case
       if (cam.pos.x - static_cast<int>(cam.pos.x) == 0) cam.pos.x += 0.01;
       if (cam.pos.y - static_cast<int>(cam.pos.y) == 0) cam.pos.y += 0.01;
       if (cam.pos.z - static_cast<int>(cam.pos.z) == 0) cam.pos.z += 0.01;
}

//Here follows the long rayCast function.  It's all in one function to increase performance.  The function is a big double for loop: for every pixel on screen the same calculations must be redone.
//The function consists out of 3 main parts inside the double for loop:
//1: Calculate the direction of the ray, based on the position of the pixel on screen and the shape of the camera plane
//2: Perform the actual raycasting, using a 3DDDA algorithm
//3: Calculate the color of the pixel based on what kind of cube was hit, and place the pixel on screen

void rayCast()
{
    for(int x = 0; x < screenWidth; x++) {
        for(int y = 0; y < screenHeight; y++) {
            // 1. Вычисляем направление луча
            double dirX = cam.dir.x + cam.v.x * (2.0 * x / screenWidth - 1.0);
            double dirY = cam.dir.y + cam.v.y * (2.0 * x / screenWidth - 1.0);
            double dirZ = cam.dir.z + cam.v.z * (2.0 * x / screenWidth - 1.0);
            
            // Нормализуем направление
            double length = sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);
            dirX /= length;
            dirY /= length;
            dirZ /= length;
            
            // 2. DDA алгоритм
            double posX = cam.pos.x;
            double posY = cam.pos.y;
            double posZ = cam.pos.z;
            
            int mapX = static_cast<int>(posX);
            int mapY = static_cast<int>(posY);
            int mapZ = static_cast<int>(posZ);
            
            // Вычисляем длину луча до следующей клетки по X, Y, Z
            double deltaDistX = sqrt(1 + (dirY * dirY + dirZ * dirZ) / (dirX * dirX));
            double deltaDistY = sqrt(1 + (dirX * dirX + dirZ * dirZ) / (dirY * dirY));
            double deltaDistZ = sqrt(1 + (dirX * dirX + dirY * dirY) / (dirZ * dirZ));
            
            // Определяем направление шага и начальное расстояние
            double sideDistX, sideDistY, sideDistZ;
            int stepX, stepY, stepZ;
            
            if(dirX < 0) {
                stepX = -1;
                sideDistX = (posX - mapX) * deltaDistX;
            } else {
                stepX = 1;
                sideDistX = (mapX + 1.0 - posX) * deltaDistX;
            }
            
            if(dirY < 0) {
                stepY = -1;
                sideDistY = (posY - mapY) * deltaDistY;
            } else {
                stepY = 1;
                sideDistY = (mapY + 1.0 - posY) * deltaDistY;
            }
            
            if(dirZ < 0) {
                stepZ = -1;
                sideDistZ = (posZ - mapZ) * deltaDistZ;
            } else {
                stepZ = 1;
                sideDistZ = (mapZ + 1.0 - posZ) * deltaDistZ;
            }
            
            // DDA цикл
            bool hit = false;
            int side = 0; // 0 = X, 1 = Y, 2 = Z
            
            while(!hit && mapX >= 0 && mapX < 64 && mapY >= 0 && mapY < 64 && mapZ >= 0 && mapZ < 64) {
                if(sideDistX < sideDistY && sideDistX < sideDistZ) {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                } else if(sideDistY < sideDistZ) {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                } else {
                    sideDistZ += deltaDistZ;
                    mapZ += stepZ;
                    side = 2;
                }
                
                if(world[mapX][mapY][mapZ].shape > 0) hit = true;
            }
            
            // 3. Отрисовка
            if(hit) {
                Uint8 r = world[mapX][mapY][mapZ].r;
                Uint8 g = world[mapX][mapY][mapZ].g;
                Uint8 b = world[mapX][mapY][mapZ].b;
                
                // Затемняем цвет в зависимости от стороны
                if(side == 0) { r = r * 0.9; g = g * 0.9; b = b * 0.9; }
                else if(side == 1) { r = r * 0.8; g = g * 0.8; b = b * 0.8; }
                else { r = r * 0.7; g = g * 0.7; b = b * 0.7; }
                
                drawPixel(x, y, r, g, b);
            } else {
                drawPixel(x, y, 64, 64, 64); // Фоновый цвет
            }
        }
    }
}







