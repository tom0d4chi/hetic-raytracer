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

void Plane::DrawPlane(Image& image, const Vec3& camOrigin, int width, int height, const std::vector<rayscene::Sphere>& spheres, Light light) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Convertir les coordonnées écran en coordonnées normalisées [-1, 1]
            float screenX = (2.0f * x / width) - 1.0f;
            float screenY = (2.0f * y / height) - 1.0f;

            Vec3 rayDirection(screenX, -screenY, 1.0f);
            rayDirection = rayDirection.normalized();
            Ray ray(camOrigin, rayDirection);

            if (ray.direction().y < 0) {
                // Calculer distance t jusqu'au plan
                float t = (posY - ray.origin().y) / ray.direction().y;

                Vec3 floorPoint = ray.at(t);

                HitInfo hit;
                hit.t = t;
                hit.point = floorPoint;

                DiffuseShader shader;
                float shadowFactor = shader.ShadowFactorPlane(hit, light, spheres);

                float floorX = floorPoint.x;
                float floorZ = floorPoint.z;

                int gridX = (int)floor(floorX / tileSize);
                int gridZ = (int)floor(floorZ / tileSize);

                bool isWhite = (gridX + gridZ) % 2 == 0;
                Color baseColor = isWhite ? colors[0] : colors[1];
                const Color shadedColor(
                    baseColor.R() * shadowFactor,
                    baseColor.G() * shadowFactor,
                    baseColor.B() * shadowFactor
                );

                image.SetPixel(x, y, shadedColor);
            }
        }
    }
}
