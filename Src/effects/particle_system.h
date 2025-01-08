#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__

#include <vector>
#include "../trixelmath.h"

struct Particle {
    Vector3 position;
    Vector3 velocity;
    Vector3 color;
    float life;
    float size;
    bool active;
};

class ParticleEmitter {
public:
    ParticleEmitter(int maxParticles = 1000);
    ~ParticleEmitter();

    void Update(float deltaTime);
    void Emit(const Vector3& position, const Vector3& direction, 
              const Vector3& color, float spread = 30.0f);
    void Render();

    // Настройки эмиттера
    void SetParticleLife(float min, float max);
    void SetParticleSpeed(float min, float max);
    void SetParticleSize(float min, float max);
    void SetGravityEffect(float gravity);

private:
    std::vector<Particle> particles;
    int maxParticles;
    
    // Параметры частиц
    float minLife, maxLife;
    float minSpeed, maxSpeed;
    float minSize, maxSize;
    float gravityEffect;

    void UpdateParticle(Particle& particle, float deltaTime);
    Particle* GetInactiveParticle();
};

#endif 