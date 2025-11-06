#include "Plane.hpp"
#include "../raymath/Ray.hpp"
#include "../raymath/Vec3.hpp"
#include "../rayscene/Light.hpp"
#include "../rayscene/Sphere.hpp"
#include "../rayshader/DiffuseShader.hpp"
#include <cmath>

using namespace std;
using namespace math;
using namespace rayscene;

Plane::Plane(array<Color, 2> colors, float posY, float tileSize)
    : colors(colors), posY(posY), tileSize(tileSize) {
}

void Plane::DrawPlane(Image& image, const Vec3& camOrigin, int width, int height, const std::vector<Sphere>& spheres, const Light& light,  std::vector<Real>& zbuf) {
    if (width <= 0 || height <= 0) return;
    
    const Real aspect = Real(width) / Real(height);
    const Real focal_length = Real(4.0); 

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            
            // Conversion en coordonnées normalisées
            const Real screenX = ((Real(2.0) * x / width) - Real(1.0)) * aspect;
            const Real screenY = (Real(2.0) * y / height) - Real(1.0);

          
            Vec3 rayDirection(screenX, -screenY, focal_length);
            rayDirection = rayDirection.normalized();
            Ray ray(camOrigin, rayDirection);

            if (ray.direction().y < 0) {
                float t = (posY - ray.origin().y) / ray.direction().y;

                if (t > math::RAY_MIN_T) {
                    Vec3 floorPoint = ray.at(t);

                    const int idx = y * width + x; 

                    if (t < zbuf[idx]) { 
                        HitInfo hit;
                        hit.t = t;
                        hit.point = floorPoint;
                        hit.normal = Vec3(0, 1, 0); 
                        
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

                        image.SetPixel(static_cast<unsigned>(x), static_cast<unsigned>(y), shadedColor);
                        zbuf[idx] = t;
                    }
                }
            }
        }
    }
}