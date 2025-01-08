#ifndef __PHYSICS_SYSTEM_H__
#define __PHYSICS_SYSTEM_H__

#include <vector>
#include "../trixelmath.h"

struct RigidBody {
    float mass;
    float restitution; // коэффициент упругости
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    Vector3 forces;
    bool isStatic;
};

class PhysicsSystem {
public:
    PhysicsSystem();
    ~PhysicsSystem();

    void Update(float deltaTime);
    void AddRigidBody(RigidBody* body);
    void RemoveRigidBody(RigidBody* body);
    void ApplyForce(RigidBody* body, const Vector3& force);
    void SetGravity(const Vector3& gravity);

private:
    std::vector<RigidBody*> bodies;
    Vector3 gravity;
    void IntegrateForces(RigidBody* body, float dt);
    void ResolveCollisions();
};

#endif 