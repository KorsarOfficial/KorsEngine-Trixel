#include "lighting.h"
#include <cmath>

LightingSystem::LightingSystem() {
    ambientColor = Vector3(1.0f, 1.0f, 1.0f);
    ambientIntensity = 0.1f;
    shadowsEnabled = false;
}

LightingSystem::~LightingSystem() {
    lights.clear();
}

void LightingSystem::AddLight(const Light& light) {
    lights.push_back(light);
}

void LightingSystem::RemoveLight(int index) {
    if(index >= 0 && index < lights.size()) {
        lights.erase(lights.begin() + index);
    }
}

void LightingSystem::UpdateLight(int index, const Light& light) {
    if(index >= 0 && index < lights.size()) {
        lights[index] = light;
    }
}

Vector3 LightingSystem::CalculateLighting(const Vector3& position, const Vector3& normal) {
    Vector3 totalLight = ambientColor * ambientIntensity;

    for(const auto& light : lights) {
        Vector3 lightContribution;
        
        switch(light.type) {
            case LightType::DIRECTIONAL:
                lightContribution = CalculateDirectionalLight(light, position, normal);
                break;
            case LightType::POINT:
                lightContribution = CalculatePointLight(light, position, normal);
                break;
            case LightType::SPOT:
                lightContribution = CalculateSpotLight(light, position, normal);
                break;
        }

        totalLight = totalLight + lightContribution;
    }

    // Ограничиваем значения от 0 до 1
    totalLight.x = fmin(fmax(totalLight.x, 0.0f), 1.0f);
    totalLight.y = fmin(fmax(totalLight.y, 0.0f), 1.0f);
    totalLight.z = fmin(fmax(totalLight.z, 0.0f), 1.0f);

    return totalLight;
}

Vector3 LightingSystem::CalculateDirectionalLight(const Light& light, const Vector3& position, const Vector3& normal) {
    float diffuse = fmax(0.0f, -(
        light.direction.x * normal.x +
        light.direction.y * normal.y +
        light.direction.z * normal.z
    ));

    return Vector3(
        light.color.x * light.intensity * diffuse,
        light.color.y * light.intensity * diffuse,
        light.color.z * light.intensity * diffuse
    );
}

Vector3 LightingSystem::CalculatePointLight(const Light& light, const Vector3& position, const Vector3& normal) {
    Vector3 lightDir = Vector3(
        light.position.x - position.x,
        light.position.y - position.y,
        light.position.z - position.z
    );

    float distance = sqrt(
        lightDir.x * lightDir.x +
        lightDir.y * lightDir.y +
        lightDir.z * lightDir.z
    );

    if(distance > light.range) return Vector3(0.0f, 0.0f, 0.0f);

    // Нормализуем направление света
    lightDir = Vector3(
        lightDir.x / distance,
        lightDir.y / distance,
        lightDir.z / distance
    );

    float diffuse = fmax(0.0f, 
        lightDir.x * normal.x +
        lightDir.y * normal.y +
        lightDir.z * normal.z
    );

    // Затухание света с расстоянием
    float attenuation = 1.0f - fmin(distance / light.range, 1.0f);
    attenuation = attenuation * attenuation; // Квадратичное затухание

    return Vector3(
        light.color.x * light.intensity * diffuse * attenuation,
        light.color.y * light.intensity * diffuse * attenuation,
        light.color.z * light.intensity * diffuse * attenuation
    );
}

Vector3 LightingSystem::CalculateSpotLight(const Light& light, const Vector3& position, const Vector3& normal) {
    Vector3 lightDir = Vector3(
        light.position.x - position.x,
        light.position.y - position.y,
        light.position.z - position.z
    );

    float distance = sqrt(
        lightDir.x * lightDir.x +
        lightDir.y * lightDir.y +
        lightDir.z * lightDir.z
    );

    if(distance > light.range) return Vector3(0.0f, 0.0f, 0.0f);

    // Нормализуем направление света
    lightDir = Vector3(
        lightDir.x / distance,
        lightDir.y / distance,
        lightDir.z / distance
    );

    // Проверяем, находится ли точка в конусе прожектора
    float spotEffect = -(
        lightDir.x * light.direction.x +
        lightDir.y * light.direction.y +
        lightDir.z * light.direction.z
    );

    if(spotEffect < cos(light.spotAngle * 3.14159f / 180.0f)) {
        return Vector3(0.0f, 0.0f, 0.0f);
    }

    float diffuse = fmax(0.0f, 
        lightDir.x * normal.x +
        lightDir.y * normal.y +
        lightDir.z * normal.z
    );

    // Затухание света с расстоянием
    float attenuation = 1.0f - fmin(distance / light.range, 1.0f);
    attenuation = attenuation * attenuation; // Квадратичное затухание

    // Усиление эффекта прожектора
    float spotIntensity = pow(spotEffect, 8.0f);

    return Vector3(
        light.color.x * light.intensity * diffuse * attenuation * spotIntensity,
        light.color.y * light.intensity * diffuse * attenuation * spotIntensity,
        light.color.z * light.intensity * diffuse * attenuation * spotIntensity
    );
}

void LightingSystem::SetAmbientLight(const Vector3& color, float intensity) {
    ambientColor = color;
    ambientIntensity = intensity;
}

void LightingSystem::EnableShadows(bool enable) {
    shadowsEnabled = enable;
} 