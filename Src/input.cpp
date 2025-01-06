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
This file contains all user input routines.  userInput() reads mouse and keyboard
input and performs the actions related to the input, such as moving the camera or
placing blocks.  See the comments in the function to see what all keys do.
*/

#include <SDL2/SDL.h>
#include "trixel.h"
#include "input.h"
#include "world.h"
#include "init.h"
#include "extern.h"
#include "screen.h"
#include "camera.h"
#include "collision.h"

bool mouseMayMove = false; //this way the mouse won't "jump" at startup if the mouse wasn't centered yet
double speed = 0.2;  //Your movement speed.  This value should never be greater than 1, or the collision detection won't work correctly (because it doesn't support "4-dimensional collision detection")
const double pi = 3.1415926535897932384626433832795;

void userInput()
{
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    int xrel, yrel;
    SDL_GetRelativeMouseState(&xrel, &yrel);

    // Обработка мыши
    double mouseX = xrel;
    double mouseY = yrel;
    
    if(mouseX != 0 || mouseY != 0)
    {
        if(mouseY != 0)
            cam.rotate(cam.side, -pi*mouseY/180);
        if(mouseX != 0)
            cam.rotate(quickVector(0,0,1), -pi*mouseX/180);
    }

    // WASD управление
    if (keystate[SDL_SCANCODE_W])
    {
        vector moveCam = cam.dir;
        moveCam.x *= speed;
        moveCam.y *= speed;
        moveCam.z *= speed;
        cam.move(moveCam);
    }
    
    if (keystate[SDL_SCANCODE_S])
    {
        vector moveCam = cam.dir;
        moveCam.x *= -speed;
        moveCam.y *= -speed;
        moveCam.z *= -speed;
        cam.move(moveCam);
    }
    
    if (keystate[SDL_SCANCODE_A])
    {
        vector moveCam = cam.side;
        moveCam.x *= speed;
        moveCam.y *= speed;
        moveCam.z *= speed;
        cam.move(moveCam);
    }
    
    if (keystate[SDL_SCANCODE_D])
    {
        vector moveCam = cam.side;
        moveCam.x *= -speed;
        moveCam.y *= -speed;
        moveCam.z *= -speed;
        cam.move(moveCam);
    }

    // Остальные клавиши управления
    if (keystate[SDL_SCANCODE_Q])
        cam.rotate(cam.dir, 5*pi/180);
    
    if (keystate[SDL_SCANCODE_E])
        cam.rotate(cam.dir, -5*pi/180);

    if (keystate[SDL_SCANCODE_R])
    {
        cam.u = defaultCam.u;
        cam.v = defaultCam.v;
    }

    // Добавляем проверку коллизий после всех перемещений
    handleCollision(cam);
}

void handleEvent(SDL_Event event)
{
    switch(event.type)
    {
        case SDL_QUIT:
            SDL_SetRelativeMouseMode(SDL_FALSE);
            exit(0);
            break;
    }
}