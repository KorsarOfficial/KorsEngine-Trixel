#include "physics_system.h"
#include "../collision.h"

PhysicsSystem::PhysicsSystem() {
    gravity = Vector3(0.0f, -9.81f, 0.0f);
}

PhysicsSystem::~PhysicsSystem() {
    for(auto body : bodies) {
        delete body;
    }
    bodies.clear();
}

void PhysicsSystem::Update(float deltaTime) {
    // Применяем силы и обновляем позиции
    for(auto body : bodies) {
        if(!body->isStatic) {
            // Применяем гравитацию
            ApplyForce(body, gravity * body->mass);
            IntegrateForces(body, deltaTime);
        }
    }

    // Разрешаем коллизии
    ResolveCollisions();

    // Сбрасываем накопленные силы
    for(auto body : bodies) {
        body->forces = Vector3(0.0f, 0.0f, 0.0f);
    }
}

void PhysicsSystem::AddRigidBody(RigidBody* body) {
    bodies.push_back(body);
}

void PhysicsSystem::RemoveRigidBody(RigidBody* body) {
    auto it = std::find(bodies.begin(), bodies.end(), body);
    if(it != bodies.end()) {
        bodies.erase(it);
    }
}

void PhysicsSystem::ApplyForce(RigidBody* body, const Vector3& force) {
    if(!body->isStatic) {
        body->forces = body->forces + force;
    }
}

void PhysicsSystem::SetGravity(const Vector3& newGravity) {
    gravity = newGravity;
}

void PhysicsSystem::IntegrateForces(RigidBody* body, float dt) {
    if(body->mass <= 0.0f) return;

    // Обновляем ускорение
    body->acceleration = body->forces * (1.0f / body->mass);

    // Обновляем скорость
    body->velocity = body->velocity + body->acceleration * dt;

    // Обновляем позицию
    body->position = body->position + body->velocity * dt;
}

void PhysicsSystem::ResolveCollisions() {
    // Простая реализация разрешения коллизий
    for(size_t i = 0; i < bodies.size(); i++) {
        for(size_t j = i + 1; j < bodies.size(); j++) {
            RigidBody* bodyA = bodies[i];
            RigidBody* bodyB = bodies[j];

            // TODO: Добавить более сложную проверку коллизий
            // Сейчас используется простая проверка расстояния между объектами
            Vector3 diff = bodyA->position - bodyB->position;
            float dist = sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
            
            if(dist < 1.0f) { // Условная дистанция коллизии
                // Вычисляем нормаль столкновения
                Vector3 normal = diff * (1.0f / dist);

                // Применяем импульс для разрешения коллизии
                if(!bodyA->isStatic && !bodyB->isStatic) {
                    float restitution = std::min(bodyA->restitution, bodyB->restitution);
                    Vector3 relativeVel = bodyA->velocity - bodyB->velocity;
                    float velAlongNormal = relativeVel.x * normal.x + 
                                         relativeVel.y * normal.y + 
                                         relativeVel.z * normal.z;

                    if(velAlongNormal > 0) continue;

                    float j = -(1.0f + restitution) * velAlongNormal;
                    j /= 1.0f / bodyA->mass + 1.0f / bodyB->mass;

                    Vector3 impulse = normal * j;
                    bodyA->velocity = bodyA->velocity + impulse * (1.0f / bodyA->mass);
                    bodyB->velocity = bodyB->velocity - impulse * (1.0f / bodyB->mass);
                }
            }
        }
    }
} 