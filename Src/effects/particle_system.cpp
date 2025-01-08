#include "particle_system.h"
#include <cstdlib>
#include <cmath>

ParticleEmitter::ParticleEmitter(int maxParticles) : maxParticles(maxParticles) {
    particles.resize(maxParticles);
    for(auto& particle : particles) {
        particle.active = false;
    }

    // Значения по умолчанию
    minLife = 1.0f;
    maxLife = 3.0f;
    minSpeed = 1.0f;
    maxSpeed = 5.0f;
    minSize = 0.1f;
    maxSize = 0.5f;
    gravityEffect = 0.5f;
}

ParticleEmitter::~ParticleEmitter() {
    particles.clear();
}

void ParticleEmitter::Update(float deltaTime) {
    for(auto& particle : particles) {
        if(particle.active) {
            UpdateParticle(particle, deltaTime);
        }
    }
}

void ParticleEmitter::Emit(const Vector3& position, const Vector3& direction, 
                          const Vector3& color, float spread) {
    Particle* particle = GetInactiveParticle();
    if(!particle) return;

    particle->active = true;
    particle->position = position;
    particle->color = color;
    particle->life = minLife + static_cast<float>(rand()) / RAND_MAX * (maxLife - minLife);
    particle->size = minSize + static_cast<float>(rand()) / RAND_MAX * (maxSize - minSize);

    // Вычисляем случайное направление в пределах конуса spread
    float speed = minSpeed + static_cast<float>(rand()) / RAND_MAX * (maxSpeed - minSpeed);
    float angle = static_cast<float>(rand()) / RAND_MAX * spread - spread / 2.0f;
    float angle2 = static_cast<float>(rand()) / RAND_MAX * 360.0f;

    // Преобразуем углы в вектор направления
    float rad = angle * 3.14159f / 180.0f;
    float rad2 = angle2 * 3.14159f / 180.0f;
    
    Vector3 randomDir;
    randomDir.x = direction.x + cos(rad) * cos(rad2);
    randomDir.y = direction.y + sin(rad);
    randomDir.z = direction.z + cos(rad) * sin(rad2);
    
    // Нормализуем и умножаем на скорость
    float length = sqrt(randomDir.x * randomDir.x + randomDir.y * randomDir.y + randomDir.z * randomDir.z);
    particle->velocity = Vector3(
        randomDir.x / length * speed,
        randomDir.y / length * speed,
        randomDir.z / length * speed
    );
}

void ParticleEmitter::UpdateParticle(Particle& particle, float deltaTime) {
    particle.life -= deltaTime;
    if(particle.life <= 0.0f) {
        particle.active = false;
        return;
    }

    // Применяем гравитацию
    particle.velocity.y -= gravityEffect * deltaTime;

    // Обновляем позицию
    particle.position = particle.position + particle.velocity * deltaTime;
}

Particle* ParticleEmitter::GetInactiveParticle() {
    for(auto& particle : particles) {
        if(!particle.active) {
            return &particle;
        }
    }
    return nullptr;
}

void ParticleEmitter::SetParticleLife(float min, float max) {
    minLife = min;
    maxLife = max;
}

void ParticleEmitter::SetParticleSpeed(float min, float max) {
    minSpeed = min;
    maxSpeed = max;
}

void ParticleEmitter::SetParticleSize(float min, float max) {
    minSize = min;
    maxSize = max;
}

void ParticleEmitter::SetGravityEffect(float gravity) {
    gravityEffect = gravity;
}

void ParticleEmitter::Render() {
    // TODO: Реализовать рендеринг частиц
    // Здесь нужно интегрироваться с существующей системой рендеринга
    for(const auto& particle : particles) {
        if(particle.active) {
            // Рендерим каждую активную частицу
            // Используем particle.position, particle.color и particle.size
        }
    }
} 