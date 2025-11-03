#include "Plane.hpp"
#include "../raymath/Ray.hpp"
#include "../raymath/Vec3.hpp"
#include <cmath>

using namespace std;
using namespace math;

Plane::Plane(array<Color, 2> colors, float posY, float tileSize)
    : colors(colors), posY(posY), tileSize(tileSize) {
}

void Plane::DrawPlane(Image& image, const Vec3& camOrigin, int width, int height) {
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
                float floorX = floorPoint.x;
                float floorZ = floorPoint.z;

                int gridX = (int)floor(floorX / tileSize);
                int gridZ = (int)floor(floorZ / tileSize);

                bool isWhite = (gridX + gridZ) % 2 == 0;
                image.SetPixel(x, y, isWhite ? colors[0] : colors[1]);
            }
        }
    }
}
