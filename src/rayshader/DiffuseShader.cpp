#include "DiffuseShader.hpp"
#include "../raymath/Color.hpp"
#include "../rayscene/Light.hpp"
#include "../rayscene/Sphere.hpp"
#include "../raymath/Ray.hpp"
#include "../raymath/Intersection.hpp"
#include <vector>

using namespace math;

float DiffuseShader::Shade(math::HitInfo hitInfo, Light light, const std::vector<rayscene::Sphere>& spheres, Vec3 camera, int specularPower) {
    // TODO make ambientFactor a global variable
    float ambientFactor = 0.3f;

    Vec3 normal = hitInfo.normal.normalize();

    Vec3 lightPos = light.getPosition();
    Vec3 lightVector = Vec3(lightPos.x - hitInfo.point.x, lightPos.y - hitInfo.point.y, lightPos.z - hitInfo.point.z);
    Vec3 lightDir = lightVector.normalize();

    Ray shadowRay(hitInfo.point, lightDir);

    for (const auto& sphere : spheres) {
        const auto hit = sphere.intersect(shadowRay);
        if (hit) {
            return ambientFactor;
        }
    }

    float dotProduct = normal.x * lightDir.x + normal.y * lightDir.y + normal.z * lightDir.z;

    float diffuse = dotProduct > 0.0f ? dotProduct : 0.0f;

    // calcul de specular

    Vec3 cameraDir = Vec3(camera - hitInfo.point).normalize();

    Vec3 reflected = (2 * (lightDir.x * normal.x + lightDir.y * normal.y + lightDir.z * normal.z) * normal - lightDir);

    float dotReflectedCamera = reflected.x * cameraDir.x + reflected.y * cameraDir.y + reflected.z * cameraDir.z;

    float specular = pow(max(0.0f, (float)dotReflectedCamera), specularPower);

    return ambientFactor + diffuse + specular;
}
