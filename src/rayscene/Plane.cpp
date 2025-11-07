#include "Plane.hpp"
#include "../raymath/Ray.hpp"
#include "../raymath/Vec3.hpp"
#include "../rayscene/Light.hpp"
#include "../rayscene/Sphere.hpp"
#include "../rayshader/DiffuseShader.hpp"
#include <cmath>

using namespace std;
using namespace math;

Plane::Plane(array<Color, 2> colors, float posY, float tileSize)
    : colors(colors), posY(posY), tileSize(tileSize) {
}

std::optional<HitInfo> Plane::intersect(const Ray& ray) const noexcept {
    Real t = (posY - ray.origin().y) / ray.direction().y;

    if (t < RAY_MIN_T) {
        return std::nullopt;
    }

    HitInfo info;
    info.t = t;
    info.point = ray.at(t);

    return info;
}

Vec3 Plane::getColorAt(const Vec3& point) const noexcept {
    int gridX = static_cast<int>(floor(point.x / tileSize));
    int gridZ = static_cast<int>(floor(point.z / tileSize));

    bool isWhite = (gridX + gridZ) % 2 == 0;

    Color baseColor = isWhite ? colors[0] : colors[1];
    return Vec3(baseColor.R(), baseColor.G(), baseColor.B());
}

void Plane::DrawPlane(Image& image, const Vec3& camOrigin, int width, int height, const std::vector<rayscene::Sphere>& spheres, Light light, int echantillonsNumber) {
    if (width <= 0 || height <= 0) {
        return;
    }

    const Real aspect = Real(width) / Real(height);
    const Real focal_length = 4.0;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Vec3 accumulatorColor(0, 0, 0);

            for (int echantillon = 0; echantillon < echantillonsNumber; ++echantillon) {
                Real sampleX = Real(x) + randomReal(0, 1);
                Real sampleY = Real(y) + randomReal(0, 1);

                const Real screenX = ((Real(2.0) * sampleX / width) - Real(1.0)) * aspect;
                const Real screenY = (Real(2.0) * sampleY / height) - Real(1.0);

                Vec3 rayDirection(screenX, -screenY, focal_length);
                rayDirection = rayDirection.normalized();
                const Ray ray(camOrigin, rayDirection);

                if (ray.direction().y < 0) {
                    // Calculer distance t jusqu'au plan
                    float t = (posY - ray.origin().y) / ray.direction().y;

                    Vec3 floorPoint = ray.at(t);

                    float floorX = floorPoint.x;
                    float floorZ = floorPoint.z;

                    int gridX = (int)floor(floorX / tileSize);
                    int gridZ = (int)floor(floorZ / tileSize);

                    HitInfo hit;
                    hit.t = t;
                    hit.point = floorPoint;

                    DiffuseShader shader;
                    float shadowFactor = shader.ShadowFactorPlane(hit, light, spheres);

                    bool isWhite = (gridX + gridZ) % 2 == 0;
                    Color baseColor = isWhite ? colors[0] : colors[1];

                    Vec3 shadedColor(
                        baseColor.R() * shadowFactor,
                        baseColor.G() * shadowFactor,
                        baseColor.B() * shadowFactor
                    );

                    Vec3 planeNormal(0, 1, 0);
                    Vec3 reflectDir = ray.direction().reflect(planeNormal);
                    Ray reflectRay(hit.point, reflectDir);
                    Real reflect_closest_t = numeric_limits<Real>::infinity();

                    for (const auto& sphere : spheres) {
                        const auto sphereHit = sphere.intersect(reflectRay);
                        if (sphereHit && sphereHit->t < reflect_closest_t) {
                            reflect_closest_t = sphereHit->t;
                            Vec3 sphereShadedColor = sphere.getShadedColor(*sphereHit, reflectRay, light, spheres, camOrigin, *this);
                            shadedColor = (shadedColor + (sphereShadedColor * sphere.reflectFactor())) * shadowFactor;
                        }
                    }

                    accumulatorColor = accumulatorColor + shadedColor;
                }
            }

            Vec3 finalColor(accumulatorColor.x / echantillonsNumber, accumulatorColor.y / echantillonsNumber, accumulatorColor.z / echantillonsNumber);

            image.SetPixel(x, y, Color(finalColor.x, finalColor.y, finalColor.z));
        }
    }
}
