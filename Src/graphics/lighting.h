#ifndef __LIGHTING_H__
#define __LIGHTING_H__

#include <vector>
#include "../trixelmath.h"

enum class LightType {
    DIRECTIONAL,
    POINT,
    SPOT
};

struct Light {
    LightType type;
    Vector3 position;      // Для POINT и SPOT
    Vector3 direction;     // Для DIRECTIONAL и SPOT
    Vector3 color;
    float intensity;
    float range;          // Для POINT и SPOT
    float spotAngle;      // Только для SPOT
    bool castShadows;
};

class LightingSystem {
public:
    LightingSystem();
    ~LightingSystem();

    // Управление источниками света
    void AddLight(const Light& light);
    void RemoveLight(int index);
    void UpdateLight(int index, const Light& light);
    
    // Вычисление освещения
    Vector3 CalculateLighting(const Vector3& position, const Vector3& normal);
    
    // Настройки освещения
    void SetAmbientLight(const Vector3& color, float intensity);
    void EnableShadows(bool enable);

private:
    std::vector<Light> lights;
    Vector3 ambientColor;
    float ambientIntensity;
    bool shadowsEnabled;

    // Вспомогательные функции для расчета освещения
    Vector3 CalculateDirectionalLight(const Light& light, const Vector3& position, const Vector3& normal);
    Vector3 CalculatePointLight(const Light& light, const Vector3& position, const Vector3& normal);
    Vector3 CalculateSpotLight(const Light& light, const Vector3& position, const Vector3& normal);
};

#endif 