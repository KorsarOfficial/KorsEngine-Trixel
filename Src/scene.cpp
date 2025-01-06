#include "scene.h"
#include "trixelmath.h"
#include "extern.h"

void buildKorsarScene() {
    // Очистка мира
    for(int x = 0; x < 64; x++) {
        for(int y = 0; y < 64; y++) {
            for(int z = 0; z < 64; z++) {
                world[x][y][z].shape = 0;
                world[x][y][z].r = 128;
                world[x][y][z].g = 128;
                world[x][y][z].b = 128;
            }
        }
    }

    // Стены, пол и потолок
    for(int x = 0; x < 64; x++) {
        for(int y = 0; y < 64; y++) {
            // Пол
            world[x][y][1].shape = 1;
            world[x][y][1].r = 100;
            world[x][y][1].g = 100;
            world[x][y][1].b = 100;
            
            // Потолок
            world[x][y][20].shape = 1;
            world[x][y][20].r = 80;
            world[x][y][20].g = 80;
            world[x][y][20].b = 80;
            
            // Стены
            world[x][0][y].shape = 1;
            world[x][63][y].shape = 1;
            world[0][x][y].shape = 1;
            world[63][x][y].shape = 1;
        }
    }

    // Создаем надпись KORSAR
    int baseHeight = 10;
    int letterHeight = 5;
    int startX = 20;

    // K
    for(int z = baseHeight; z < baseHeight + letterHeight; z++) {
        world[startX][20][z].shape = 4;
        world[startX+1][21][z].shape = 4;
        world[startX+2][22][z].shape = 4;
        world[startX+1][19][z].shape = 4;
        world[startX+2][18][z].shape = 4;
    }

    // O
    startX += 7;
    for(int z = baseHeight; z < baseHeight + letterHeight; z++) {
        for(int i = 0; i < 4; i++) {
            world[startX][20+i][z].shape = 4;
            world[startX+3][20+i][z].shape = 4;
        }
        world[startX+1][19][z].shape = 4;
        world[startX+2][19][z].shape = 4;
        world[startX+1][24][z].shape = 4;
        world[startX+2][24][z].shape = 4;
    }

    // Аналогично для R, S, A, R...

    // Начальная позиция камеры
    cam.pos = quickVector(32, 32, 5);
    cam.dir = quickVector(1, 0, 0);
}

void handleCollision(camera& cam) {
    // Проверка коллизии с полом
    if(cam.pos.z < 2.0) cam.pos.z = 2.0;
    
    // Проверка коллизии с потолком
    if(cam.pos.z > 19.0) cam.pos.z = 19.0;
    
    // Проверка коллизии со стенами
    if(cam.pos.x < 1.0) cam.pos.x = 1.0;
    if(cam.pos.x > 62.0) cam.pos.x = 62.0;
    if(cam.pos.y < 1.0) cam.pos.y = 1.0;
    if(cam.pos.y > 62.0) cam.pos.y = 62.0;
} 