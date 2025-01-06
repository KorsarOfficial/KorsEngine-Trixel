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
This file contains functions that generate worlds in the world array.
The world array is a 3-dimensional array of voxels, and voxel is a struct that
contains 4 chars:
r = red component
g = green component
b = blue component
shape = the "shape" of the voxel.

Currently, the following shapes are implemented.  The source of each shape is in the
last part of the rayCast() function in trixel.cpp

Shape 0 = empty cube, but an empty cube can never be hit according to the 3DDDA algorithm
Shape 1 = cube with a single color determinated by R, G and B
Shape 2 = cube with 3 different colors for X, Y and Z side. Colors are Cyan, Yellow and Magenta.
Shape 3 = cube with 3 different colors for X, Y and Z side. Colors are Red, Green and Blue.
Shape 4 = cube with 3 different colors for X, Y and Z side. Colors are 3 different grey values.
Shape 5 = Cube with fake EnvironmentMap effect: color depends on location of the pixel
Shape 6 = Cube with random colored noise
Shape 7 = Cube with random black and white noise
Shape 8 = Cube with random red noise and a fake blue EnvironmentMap
Shape 9 = Cube with a color that depends on the position of the camera

When building up a world, please note that all worlds MUST have 6 walls on the sides,
so you can never see a part that's outside of the world array.

Below are 6 examples of worlds, each function is called by pressing a function key (F1 to F6).
A good improvement would be to support loading worlds from a big file.
*/

#include "world.h"
#include "trixelmath.h"
#include "extern.h"

voxel world[64][64][64];
int textPos = 20;

void buildWorld()
{
    buildIntroWorld();
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


void buildIntroWorld()
{
    // Очистка мира
    for(int x = 0; x < 64; x++)
    {
        for(int y = 0; y < 64; y++)
        {
            for(int z = 0; z < 64; z++)
            {
                world[x][y][z].shape = 0;
                world[x][y][z].r = 255;
                world[x][y][z].g = 255;
                world[x][y][z].b = 255;
            }
        }
    }

    //build the protection (walls, floor, ceiling)
    for(int x=0;x<64;x++)
    {
        for(int y=0;y<64;y++)
        {
            world[x][y][0].shape = 1;
            world[x][0][y].shape = 1;
            world[0][x][y].shape = 1;
            world[x][y][63].shape = 1;
            world[x][63][y].shape = 1;
            world[63][x][y].shape = 1;

            //floor
            world[x][y][0].r=0;
            world[x][y][0].g=0;
            world[x][y][0].b=0;

            //ceiling
            world[x][y][63].r=64;
            world[x][y][63].g=0;
            world[x][y][63].b=0;

            //W
            world[x][0][y].r=128;
            world[x][0][y].g=128;
            world[x][0][y].b=128;

            //S
            world[0][x][y].r=220;
            world[0][x][y].g=220;
            world[0][x][y].b=220;

            //E
            world[x][63][y].r=128;
            world[x][63][y].g=128;
            world[x][63][y].b=128;

            //N
            world[63][x][y].r=220;
            world[63][x][y].g=220;
            world[63][x][y].b=220;



        }
    }

    //build the floor
   for(int x=0;x<64;x+=2)
   {
        for(int y=0;y<64;y+=2)
        {
                for(int u=0;u<1;u++)
                {
                       for(int v=0;v<1;v++)
                       {
                            world[x+u][y+v][0].r=255;
                            world[x+u][y+v][0].g=255;
                            world[x+u][y+v][0].b=255;
                       }

                }
         }
    }
   for(int x=1;x<64;x+=2)
   {
        for(int y=1;y<64;y+=2)
        {
                for(int u=0;u<1;u++)
                {
                       for(int v=0;v<1;v++)
                       {
                            world[x+u][y+v][0].r=255;
                            world[x+u][y+v][0].g=255;
                            world[x+u][y+v][0].b=255;
                       }

                }
         }
    }





   //build the walls
  for(int x=0;x<64;x+=1)
    {
        for(int y=0;y<64;y+=1)
        {
            //for (int u=0; u<3; u+=2)
            //{
             //W
            world[x][0][y].r=x*4;
            world[x][0][y].g=y*4;
            world[x][0][y].b=y*4;

            //S
            world[0][x][y].r=y*4;
            world[0][x][y].g=x*4;
            world[0][x][y].b=y*4;

            //E
            world[x][63][y].r=x*4;
            world[x][63][y].g=x*4;
            world[x][63][y].b=y*4;

            //N
            world[63][x][y].r=x*4;
            world[63][x][y].g=y*4;
            world[63][x][y].b=x*4;
            //}
        }
    }



    //build small room

    for(int x=1;x<28;x++)
   {
        for(int y=0;y<28;y++)
        {
            world[x][y][28].shape=1;
            world[x][y][28].r=128;
            world[x][y][28].g=64;
            world[x][y][28].b=64;


            world[x][28][y].shape=1;
            world[x][28][y].r=128;
            world[x][28][y].g=90;
            world[x][28][y].b=90;

            world[28][y][x].shape=1;
            world[28][y][x].r=128;
            world[28][y][x].g=100;
            world[28][y][x].b=100;

            world[x][0][y].shape=1;
            world[x][0][y].r=128;
            world[x][0][y].g=90;
            world[x][0][y].b=90;

            world[0][y][x].shape=1;
            world[0][y][x].r=128;
            world[0][y][x].g=100;
            world[0][y][x].b=100;

            //small hole in wall
            world[14][28][14].shape = 0;
            world[15][28][14].shape = 2;
            world[14][28][15].shape = 2;
            world[13][28][14].shape = 2;
            world[14][28][13].shape = 2;

        }
   }


   //add the text shape
   int s = 7; //how much the "3D" text comes to you
   //T
   world[textPos][2][22].shape = 1;
   world[textPos][3][22].shape = 1;
   world[textPos][4][22].shape = 1;
   world[textPos][5][22].shape = 1;
   world[textPos][6][22].shape = 1;
   world[textPos][4][21].shape = 1;
   world[textPos][4][20].shape = 1;
   world[textPos][4][19].shape = 1;
   world[textPos][4][18].shape = 1;

   //R
   world[textPos][8][22].shape = 1;
   world[textPos][9][22].shape = 1;
   world[textPos][10][22].shape = 1;
   world[textPos][11][21].shape = 1;
   world[textPos][8][21].shape = 1;
   world[textPos][8][20].shape = 1;
   world[textPos][8][19].shape = 1;
   world[textPos][8][18].shape = 1;
   world[textPos][9][20].shape = 1;
   world[textPos][10][20].shape = 1;
   world[textPos][10][19].shape = 1;
   world[textPos][11][18].shape = 1;

   //I
   world[textPos][13][22].shape = 1;
   world[textPos][13][21].shape = 1;
   world[textPos][13][20].shape = 1;
   world[textPos][13][19].shape = 1;
   world[textPos][13][18].shape = 1;

   //X
   world[textPos][15][22].shape = 1;
   world[textPos][16][21].shape = 1;
   world[textPos][17][20].shape = 1;
   world[textPos][18][19].shape = 1;
   world[textPos][19][18].shape = 1;
   world[textPos][19][22].shape = 1;
   world[textPos][18][21].shape = 1;
   world[textPos][16][19].shape = 1;
   world[textPos][15][18].shape = 1;

   //E
   world[textPos][21][22].shape = 1;
   world[textPos][21][21].shape = 1;
   world[textPos][21][20].shape = 1;
   world[textPos][21][19].shape = 1;
   world[textPos][21][18].shape = 1;
   world[textPos][22][22].shape = 1;
   world[textPos][23][22].shape = 1;
   world[textPos][22][20].shape = 1;
   world[textPos][22][18].shape = 1;
   world[textPos][23][18].shape = 1;

   //L
   world[textPos][25][22].shape = 1;
   world[textPos][25][21].shape = 1;
   world[textPos][25][20].shape = 1;
   world[textPos][25][19].shape = 1;
   world[textPos][25][18].shape = 1;
   world[textPos][26][18].shape = 1;
   world[textPos][27][18].shape = 1;

   //3
   world[textPos-s][11][15].shape = 3;
   world[textPos-s][12][15].shape = 3;
   world[textPos-s][13][14].shape = 3;
   world[textPos-s][13][13].shape = 3;
   world[textPos-s][13][12].shape = 3;
   world[textPos-s][12][13].shape = 3;
   world[textPos-s][11][11].shape = 3;
   world[textPos-s][12][11].shape = 3;

   //D
   world[textPos-s][15][15].shape = 3;
   world[textPos-s][15][14].shape = 3;
   world[textPos-s][15][13].shape = 3;
   world[textPos-s][15][12].shape = 3;
   world[textPos-s][15][11].shape = 3;
   world[textPos-s][16][15].shape = 3;
   world[textPos-s][16][11].shape = 3;
   world[textPos-s][17][14].shape = 3;
   world[textPos-s][17][13].shape = 3;
   world[textPos-s][17][12].shape = 3;

   //B
   world[textPos][1][9].shape = 1;
   world[textPos][1][8].shape = 1;
   world[textPos][1][7].shape = 1;
   world[textPos][1][6].shape = 1;
   world[textPos][1][5].shape = 1;
   world[textPos][2][9].shape = 1;
   world[textPos][2][7].shape = 1;
   world[textPos][2][5].shape = 1;
   world[textPos][3][8].shape = 1;
   world[textPos][3][6].shape = 1;

   //Y
   world[textPos][5][9].shape = 1;
   world[textPos][5][8].shape = 1;
   world[textPos][6][7].shape = 1;
   world[textPos][7][9].shape = 1;
   world[textPos][7][8].shape = 1;

   //K
   world[textPos][10][9].shape = 1;
   world[textPos][10][8].shape = 1;
   world[textPos][10][7].shape = 1;
   world[textPos][10][6].shape = 1;
   world[textPos][10][5].shape = 1;
   world[textPos][11][7].shape = 1;
   world[textPos][12][9].shape = 1;
   world[textPos][12][8].shape = 1;
   world[textPos][12][6].shape = 1;
   world[textPos][12][5].shape = 1;

   //O
   world[textPos][14][9].shape = 1;
   world[textPos][14][8].shape = 1;
   world[textPos][14][7].shape = 1;
   world[textPos][14][6].shape = 1;
   world[textPos][14][5].shape = 1;
   world[textPos][15][9].shape = 1;
   world[textPos][15][5].shape = 1;
   world[textPos][16][9].shape = 1;
   world[textPos][16][8].shape = 1;
   world[textPos][16][7].shape = 1;
   world[textPos][16][6].shape = 1;
   world[textPos][16][5].shape = 1;

   //R
   world[textPos][18][9].shape = 1;
   world[textPos][18][8].shape = 1;
   world[textPos][18][7].shape = 1;
   world[textPos][18][6].shape = 1;
   world[textPos][18][5].shape = 1;
   world[textPos][19][9].shape = 1;
   world[textPos][19][7].shape = 1;
   world[textPos][20][8].shape = 1;
   world[textPos][20][6].shape = 1;
   world[textPos][20][5].shape = 1;

   //S
   world[textPos][22][9].shape = 1;
   world[textPos][22][7].shape = 1;
   world[textPos][22][5].shape = 1;
   world[textPos][23][9].shape = 1;
   world[textPos][23][7].shape = 1;
   world[textPos][23][5].shape = 1;
   world[textPos][24][8].shape = 1;
   world[textPos][24][6].shape = 1;

   //A
   world[textPos][26][9].shape = 1;
   world[textPos][26][8].shape = 1;
   world[textPos][26][7].shape = 1;
   world[textPos][26][6].shape = 1;
   world[textPos][26][5].shape = 1;
   world[textPos][27][7].shape = 1;
   world[textPos][28][9].shape = 1;
   world[textPos][28][8].shape = 1;
   world[textPos][28][7].shape = 1;
   world[textPos][28][6].shape = 1;
   world[textPos][28][5].shape = 1;

   //R
   world[textPos][30][9].shape = 1;
   world[textPos][30][8].shape = 1;
   world[textPos][30][7].shape = 1;
   world[textPos][30][6].shape = 1;
   world[textPos][30][5].shape = 1;
   world[textPos][31][9].shape = 1;
   world[textPos][31][7].shape = 1;
   world[textPos][32][8].shape = 1;
   world[textPos][32][6].shape = 1;
   world[textPos][32][5].shape = 1;

   //add the hit F2 text;

   for (int x = 0; x<10; x++)
   {
        world[40+x][63][40].shape = 8;
        world[22+x][63][49].shape = 8;
        world[17+x][63][49].shape = 8;
        world[40][63][40+x].shape = 8;
        world[40][63][40-x].shape = 8;
        world[49][63][40+x].shape = 8;
        world[49][63][40-x].shape = 8;
        world[35][63][40+x].shape = 8;
        world[35][63][40-x].shape = 8;
        world[25][63][40+x].shape = 8;
        world[25][63][40-x].shape = 8;

        world[60][63][15+x].shape = 8;
        world[60][63][15-x].shape = 8;
        world[60-x][63][25].shape = 8;
        world[60-x][63][15].shape = 8;

        world[38][63][15+x].shape = 8;
        world[48][63][15-x].shape = 8;
        world[48-x][63][25].shape = 8;
        world[48-x][63][15].shape = 8;
        world[48-x][63][6].shape = 8;

        world[25][63][15+x].shape = 8;
        world[25][63][15-x].shape = 8;
        world[25-x][63][25].shape = 8;
        world[25-x][63][15].shape = 8;

        world[3][63][15-x].shape = 8;
        world[13][63][15+x].shape = 8;
        world[13][63][15-x].shape = 8;
        world[13-x][63][25].shape = 8;
        world[13-x][63][15].shape = 8;
        world[13-x][63][6].shape = 8;

   }

   world[33][63][15].shape = 6;
   world[32][63][15].shape = 6;
   world[31][63][15].shape = 6;
   world[30][63][15].shape = 6;


 //add the color to "by lode"

 for(int x=1;x<28;x++)
   {
        for(int y=1;y<28;y++)
        {
              for(int z=1;z<12;z++)
              {
                   if (world[x][y][z].shape==1) world[x][y][z].shape=5;
              }
         }
    }

    // Добавляем текст "BY KORSAR"
    // B
    world[textPos][1][7].shape = 1;
    world[textPos][1][6].shape = 1;
    world[textPos][1][5].shape = 1;
    world[textPos][2][7].shape = 1;
    world[textPos][2][6].shape = 1;
    world[textPos][2][5].shape = 1;
    
    // Y
    world[textPos][4][7].shape = 1;
    world[textPos][5][6].shape = 1;
    world[textPos][6][7].shape = 1;
    
    // K
    world[textPos][8][7].shape = 1;
    world[textPos][8][6].shape = 1;
    world[textPos][8][5].shape = 1;
    world[textPos][9][6].shape = 1;
    world[textPos][10][7].shape = 1;
    world[textPos][10][5].shape = 1;
    
    // O
    world[textPos][12][7].shape = 1;
    world[textPos][12][6].shape = 1;
    world[textPos][12][5].shape = 1;
    world[textPos][13][7].shape = 1;
    world[textPos][13][5].shape = 1;
    world[textPos][14][7].shape = 1;
    world[textPos][14][6].shape = 1;
    world[textPos][14][5].shape = 1;
    
    // R
    world[textPos][16][7].shape = 1;
    world[textPos][16][6].shape = 1;
    world[textPos][16][5].shape = 1;
    world[textPos][17][7].shape = 1;
    world[textPos][17][6].shape = 1;
    world[textPos][18][5].shape = 1;
    
    // S
    world[textPos][20][7].shape = 1;
    world[textPos][20][6].shape = 1;
    world[textPos][21][6].shape = 1;
    world[textPos][22][6].shape = 1;
    world[textPos][22][5].shape = 1;
    
    // A
    world[textPos][24][7].shape = 1;
    world[textPos][24][6].shape = 1;
    world[textPos][24][5].shape = 1;
    world[textPos][25][6].shape = 1;
    world[textPos][26][7].shape = 1;
    world[textPos][26][6].shape = 1;
    world[textPos][26][5].shape = 1;
    
    // R
    world[textPos][28][7].shape = 1;
    world[textPos][28][6].shape = 1;
    world[textPos][28][5].shape = 1;
    world[textPos][29][7].shape = 1;
    world[textPos][29][6].shape = 1;
    world[textPos][30][5].shape = 1;

    // Устанавливаем безопасную начальную позицию камеры
    cam.pos = quickVector(32, 32, 10);  // Подняли камеру выше
    cam.dir = quickVector(1, 0, 0);     // Смотрим вдоль оси X
    cam.u = quickVector(0, 0, 1);       // Вверх по оси Z
    cam.v = quickVector(0, 1, 0);       // Вправо по оси Y
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


void buildStarFieldWorld()
{





   for(int x=0;x<64;x++)
   {
        for(int y=0;y<64;y++)
        {
                for(int z=0;z<64;z++)
                {
                       world[x][y][z].shape=false;
                       world[x][y][z].r=1;
                       world[x][y][z].g=0;
                       world[x][y][z].b=0;

                }
         }
    }

   for(int x=0;x<64;x++)
    {
        for(int y=0;y<64;y++)
        {
            world[x][y][0].shape=true;
            world[x][0][y].shape=true;
            world[0][x][y].shape=true;
            world[x][y][63].shape=true;
            world[x][63][y].shape=true;
            world[63][x][y].shape=true;
            world[x][y][1].shape=true;
            world[x][1][y].shape=true;
            world[1][x][y].shape=true;
            world[x][y][62].shape=true;
            world[x][62][y].shape=true;
            world[62][x][y].shape=true;


        }
    }

   for(int x=0;x<64;x++)
   {
        for(int y=0;y<64;y++)
        {
                for(int z=0;z<64;z++)
                {
                       if (rand1() > 0.995)
                       {
                            world[x][y][z].shape=true;
                            world[x][y][z].r=230;
                            world[x][y][z].g=230;
                            world[x][y][z].b=255;
                       }



                       if (rand1() > 0.999)
                       {
                            world[x][y][z].shape=true;
                            world[x][y][z].r=x*4;
                            world[x][y][z].g=y*4;
                            world[x][y][z].b=z*4;
                       }
                 }
         }
    }
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


void buildWorldWithSPikes()
{


   for(int x=0;x<64;x++)
   {
        for(int y=0;y<64;y++)
        {
                for(int z=0;z<64;z++)
                {
                       world[x][y][z].shape=false;
                       world[x][y][z].r=x*4;
                       world[x][y][z].g=y*4;
                       world[x][y][z].b=z*4;
                }
         }
    }


      

   for(int x=0;x<64;x++)
    {
        for(int y=0;y<64;y++)
        {
            world[x][y][0].shape=true;
            world[x][0][y].shape=true;
            world[0][x][y].shape=true;
            world[x][y][63].shape=true;
            world[x][63][y].shape=true;
            world[63][x][y].shape=true;
            world[x][y][1].shape=true;
            world[x][1][y].shape=true;
            world[1][x][y].shape=true;
            world[x][y][62].shape=true;
            world[x][62][y].shape=true;
            world[62][x][y].shape=true;

            world[x][y][0].r=x*4;
            world[x][0][y].r=y*4;
            world[0][x][y].r=255;
            world[x][y][63].r=x*4;
            world[x][63][y].r=y*4;
            world[63][x][y].r=0;
            world[x][y][1].r=x*4;
            world[x][1][y].r=y*4;
            world[1][x][y].r=255;
            world[x][y][62].r=x*4;
            world[x][62][y].r=y*4;
            world[62][x][y].r=0;

            world[x][y][0].g=x*4;
            world[x][0][y].g=y*4;
            world[0][x][y].g=128;
            world[x][y][63].g=x*4;
            world[x][63][y].g=y*4;
            world[63][x][y].g=128;
            world[x][y][1].g=x*4;
            world[x][1][y].g=y*4;
            world[1][x][y].g=128;
            world[x][y][62].g=x*4;
            world[x][62][y].g=y*4;
            world[62][x][y].g=128;

            world[x][y][0].b=0;
            world[x][0][y].b=255;
            world[0][x][y].b=0;
            world[x][y][63].b=0;
            world[x][63][y].b=255;
            world[63][x][y].b=0;
            world[x][y][1].b=0;
            world[x][1][y].b=255;
            world[1][x][y].b=0;
            world[x][y][62].b=0;
            world[x][62][y].b=255;
            world[62][x][y].b=0;

        }
    }


    for(int x=0;x<64;x+=3)
    {
        for(int y=0;y<64;y+=3)
        {
            world[x][y][2].r=255;
            world[x][2][y].r=255;
            world[2][x][y].g=255;
            world[x][y][61].r=255;
            world[x][61][y].r=255;
            world[61][x][y].g=255;

            world[x][y][2].shape=1;
            world[x][2][y].shape=1;
            world[2][x][y].shape=1;
            world[x][y][61].shape=1;
            world[x][61][y].shape=1;
            world[61][x][y].shape=1;


            world[x][y][3].r=255;
            world[x][3][y].r=255;
            world[3][x][y].g=255;
            world[x][y][60].r=255;
            world[x][60][y].r=255;
            world[60][x][y].g=255;

            world[x][y][3].shape=1;
            world[x][3][y].shape=1;
            world[3][x][y].shape=1;
            world[x][y][60].shape=1;
            world[x][60][y].shape=1;
            world[60][x][y].shape=1;

        }
    }



    //build randomnoise blocks

   for(int x=40;x<45;x++)
   {
        for(int y=45;y<50;y++)
        {
                for(int z=40;z<45;z++)
                {
                       world[x][y][z].shape=6;
                }
         }
    }

   for(int x=30;x<35;x++)
   {
        for(int y=45;y<50;y++)
        {
                for(int z=40;z<45;z++)
                {
                       world[x][y][z].shape=7;
                }
         }
    }

    for(int x=20;x<25;x++)
   {
        for(int y=45;y<50;y++)
        {
                for(int z=40;z<45;z++)
                {
                       world[x][y][z].shape=8;
                }
         }
    }

   for(int x=10;x<15;x++)
   {
        for(int y=45;y<50;y++)
        {
                for(int z=40;z<45;z++)
                {
                       world[x][y][z].shape=9;
                       world[x][y][z].r=255;//(x-10)*51;
                       world[x][y][z].g=0;//(y-45)*51;
                       world[x][y][z].b=0;//(x-40)*51;
                }
         }
    }

}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



void buildRasterWorld()
{
    // Очистка мира
    for(int x=0; x<64; x++) {
        for(int y=0; y<64; y++) {
            for(int z=0; z<64; z++) {
                world[x][y][z].shape = 0;
            }
        }
    }

    // Стены мира (оставляем как есть)
    for(int x=0; x<64; x++) {
        for(int y=0; y<64; y++) {
            world[x][y][0].shape = 1;
            world[x][0][y].shape = 1;
            world[0][x][y].shape = 1;
            world[x][y][63].shape = 1;
            world[x][63][y].shape = 1;
            world[63][x][y].shape = 1;
        }
    }

    // KORSAR - делаем буквы в безопасных пределах массива
    int letterHeight = 20;  // Уменьшаем высоту
    int baseZ = 15;
    int thickness = 2;     // Уменьшаем толщину

    // K
    for(int z = baseZ; z < baseZ + letterHeight && z < 64; z++) {
        for(int t = 0; t < thickness && t < 64; t++) {
            if(10+t < 64 && 20+t < 64) {
                world[10][20+t][z].shape = 4;
                world[10+t][20+t][z].shape = 4;
                world[10+t][30-t][z].shape = 4;
            }
        }
    }

    // O
    for(int z = baseZ; z < baseZ + letterHeight && z < 64; z++) {
        for(int t = 0; t < thickness && t < 64; t++) {
            if(20+t < 64 && 20+t < 64) {
                world[20][20+t][z].shape = 4;
                world[20][30+t][z].shape = 4;
            }
        }
    }

    // R, S, A, R - аналогично для остальных букв...

    // Устанавливаем начальную позицию камеры
    cam.pos = quickVector(32, 5, 25);
    cam.dir = quickVector(0, 1, 0);
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



void buildCubeMaze()
{
  int roomWidth;

   for(int x=0;x<64;x++)
   {
        for(int y=0;y<64;y++)
        {
                for(int z=0;z<64;z++)
                {
                       world[x][y][z].shape=4;
                       world[x][y][z].r=160;
                       world[x][y][z].g=192;
                       world[x][y][z].b=220;
                }
         }
    }

    roomWidth = 5;


    for(int x=1;x<60;x+=roomWidth+1)
    {
        for(int y=1;y<60;y+=roomWidth+1)
        {
            for(int z=1;z<60;z+=roomWidth+1)
            {
                for(int u=0;u<roomWidth;u++)
                {
                    for(int v=0;v<roomWidth;v++)
                    {
                        for(int w=0;w<roomWidth;w++)
                        {
                            world[x+u][y+v][z+w].shape=0;
                        }

                    }
                }

               if (rand1() > 0.5) world[x+roomWidth][y+roomWidth/2][z+roomWidth/2].shape=0; else world[x+roomWidth][y+roomWidth/2][z+roomWidth/2].shape=5;
               if (rand1() > 0.5) world[x+roomWidth/2][y+roomWidth][z+roomWidth/2].shape=0; else world[x+roomWidth/2][y+roomWidth][z+roomWidth/2].shape=5;
               if (rand1() > 0.5) world[x+roomWidth/2][y+roomWidth/2][z+roomWidth].shape=0; else world[x+roomWidth/2][y+roomWidth/2][z+roomWidth].shape=5;
            }

        }
    }


   world[33][33][33].shape=6;
   world[3][3][3].shape=7;
   world[57][57][57].shape=3;
   world[57][3][3].shape=7;
   world[3][57][3].shape=3;
   world[60][3][57].shape=7;
   world[3][57][57].shape=3;
   world[57][57][3].shape=7;
   world[57][3][3].shape=3;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


void buildSphereWorld()
{

   for(int x=0;x<64;x++)
   {
        for(int y=0;y<64;y++)
        {
                for(int z=0;z<64;z++)
                {
                       world[x][y][z].shape=false;
                       world[x][y][z].r=255;
                       world[x][y][z].g=255;
                       world[x][y][z].b=255;
                }
         }
    }
   //build the protection (walls, floor, ceiling)
    for(int x=0;x<64;x++)
    {
        for(int y=0;y<64;y++)
        {
            world[x][y][0].shape=true;
            world[x][0][y].shape=true;
            world[0][x][y].shape=true;
            world[x][y][63].shape=true;
            world[x][63][y].shape=true;
            world[63][x][y].shape=true;

        }
    }


    for(int x=1;x<64;x++)
    {
        for(int y=1;y<64;y++)
        {
            for(int z=1;z<64;z++)
            {
                if (sqrt((x-32)*(x-32)+(y-32)*(y-32)+(z-32)*(z-32)) < 15)
                {
                    world[x][y][z].shape=3;
                }
            }

        }
    }

}
