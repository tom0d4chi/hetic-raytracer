#include "Plane.hpp"

#include "../raymath/Ray.hpp"
#include "../raymath/Vec3.hpp"

#include <cmath>
#include <utility>

Plane::Plane(std::array<Color, 2> colors_, float posY_, float tileSize_)
    : colors(std::move(colors_))
    , tileSize(tileSize_)
    , posY(posY_) {
}

void Plane::DrawPlane(Image& image,
                      const math::Vec3& camOrigin,
                      const math::Vec3& lowerLeft,
                      const math::Vec3& horizontal,
                      const math::Vec3& vertical,
                      int width,
                      int height) {
    if (width <= 0 || height <= 0) {
        return;
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const math::Real u = (math::Real(x) + math::Real(0.5)) / math::Real(width);
            const math::Real v = (math::Real(height - 1 - y) + math::Real(0.5)) / math::Real(height);

            const math::Vec3 direction = (lowerLeft + horizontal * u + vertical * v) - camOrigin;
            const math::Vec3 dirNorm = direction.normalized();
            const math::Ray ray(camOrigin, dirNorm);

            if (ray.direction().y >= 0) {
                continue;
            }

            const math::Real t = (posY - ray.origin().y) / ray.direction().y;
            if (t <= 0) {
                continue;
            }

            const math::Vec3 floorPoint = ray.at(t);
            const math::Real floorX = floorPoint.x;
            const math::Real floorZ = floorPoint.z;

            const int gridX = static_cast<int>(std::floor(floorX / tileSize));
            const int gridZ = static_cast<int>(std::floor(floorZ / tileSize));

            const bool isPrimary = (gridX + gridZ) % 2 == 0;
            image.SetPixel(static_cast<unsigned>(x),
                           static_cast<unsigned>(y),
                           isPrimary ? colors[0] : colors[1]);
        }
    }
}

void Plane::DrawPlane(Image& image,
                      const math::Vec3& camOrigin,
                      int width,
                      int height) {
    const math::Real aspect = math::Real(width) / math::Real(height);
    const math::Real viewportHeight = 2.0;
    const math::Real viewportWidth = aspect * viewportHeight;
    const math::Real focalLength = 1.0;

    const math::Vec3 horizontal(viewportWidth, 0.0, 0.0);
    const math::Vec3 vertical(0.0, viewportHeight, 0.0);
    const math::Vec3 lowerLeft = camOrigin - horizontal / 2.0 - vertical / 2.0 - math::Vec3(0.0, 0.0, focalLength);

    DrawPlane(image, camOrigin, lowerLeft, horizontal, vertical, width, height);
}
