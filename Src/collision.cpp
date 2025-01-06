#include "collision.h"
#include "world.h"

void handleCollision(camera& cam) {
    // Более строгие ограничения
    if(cam.pos.z < 5.0) cam.pos.z = 5.0;       // Минимальная высота
    if(cam.pos.z > 58.0) cam.pos.z = 58.0;     // Максимальная высота
    
    // Границы комнаты
    if(cam.pos.x < 5.0) cam.pos.x = 5.0;
    if(cam.pos.x > 58.0) cam.pos.x = 58.0;
    if(cam.pos.y < 5.0) cam.pos.y = 5.0;
    if(cam.pos.y > 58.0) cam.pos.y = 58.0;
} 