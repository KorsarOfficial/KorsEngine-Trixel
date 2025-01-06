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
This file contains the camera struct and it's functions.  At the bottom is an
explanation of how the camera looks and about the coordinate system.
The camera supports:C:\msys64\mingw64\bin
6 degrees of freedom for translation and rotation
resising of camera plane in U and V direction to change the FOV
skewing of camera plane
direction vector doesn't have to be perpendicular on the plane
*/





#ifndef CAMERA_H
#define CAMERA_H

#include "trixelmath.h"

struct camera
{
    vector pos;   // position
    vector dir;   // direction
    vector up;    // up vector
    vector side;  // side vector
    vector u;     // u vector for rotation
    vector v;     // v vector for rotation

    void rotate(vector axis, double angle) {
        dir = rotateAroundArbitrary(dir, axis, angle);
        up = rotateAroundArbitrary(up, axis, angle);
        side = rotateAroundArbitrary(side, axis, angle);
    }

    void move(vector direction) {
        pos.x += direction.x;
        pos.y += direction.y;
        pos.z += direction.z;
    }
};

#endif

/*
Camera Ascii Art Representation

Camera Front View:

    _____________________
   |         ^           |  Camera Plane
   |         | V         |
   |         |           |
   |         |        U  |
   |         x---------> |
   |        Dir          |
   |                     |
   |                     |
   |_____________________|


Camera Side View:

   ^ V
   |
   |
   |
   |            Dir
 U o-------------->
   |
   |
   |
   |Camera Plane


Glossary:

Dir = Direction camera is looking at, normally this vector is supposed to
    be perpendicular on the camera plane but other situations work just as good.
V = the Vertical vector of the camera plane
U = the Horizontal vector of the camera plane
Camera Plane = the world is projected on this in typical OpenGL or Direct3D
       applications, however in this raycaster the rays start from the eyepoint
       and go through the corners of the camera plane and all places between the
       corners.  The Camera Plane is determinated by the U and V vector, if the
       size of the Camera Plane is larger in U or V direction, this will result
       in a larger fovx or fovy.  If the U and V vector aren't perpendicular,
       the Camera Plane will be skewed and the world will appear skewed on the
       screen.


Camera Facing Directions:

camera dir (1,0,0) = North
camera dir (0,1,0) = East
camera dir (-1,0,0) = South
camera dir (0,-1,0) = West
camera dir (0,0,1) = Up
camera dir (0,0,-1) = Down


          N
          ^ +1
          | X
          |
 -1       |       Y +1
W<--------|--------->E
          |
          |
          |
          v  -1
          S
*/
