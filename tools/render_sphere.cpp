#include <optional>
#include <memory>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

#include "raymath/Vec3.hpp"
#include "raymath/Ray.hpp"
#include "raymath/Intersection.hpp"
#include "rayscene/Sphere.hpp"
#include "rayimage/Image.hpp"
#include "raymath/Color.hpp"
#include "raytimer/Timer.hpp"


using math::Vec3;
using math::Ray;
using rayscene::Sphere;
using math::Real;
using ::Image;
using ::Color;

int main() {
    const int W = 800;
    const int H = 600;
    const Real aspect = Real(W) / Real(H);

    Timer timer("Sphere render");


    // Camera: simple pinhole at (0,0,0), image plane at z = -1
    Vec3 cam_origin(0, 0, 0);
    Real viewport_height = 2.0;
    Real viewport_width = aspect * viewport_height;
    Real focal_length = 1.0;

    Vec3 horizontal(viewport_width, 0, 0);
    Vec3 vertical(0, viewport_height, 0);
    Vec3 lower_left = cam_origin - horizontal/2 - vertical/2 - Vec3(0, 0, focal_length);

    // Create three spheres with distinct colors and positions
    std::vector<Sphere> spheres;
    spheres.emplace_back(Vec3(0.0, 0.0, -3.0), Real(1.0), nullptr, Vec3(0.0, 1.0, 0.0)); // green (center)
    spheres.emplace_back(Vec3(-1.8, 0.0, -3.0), Real(1.0), nullptr, Vec3(0.0, 0.0, 1.0)); // blue (left)
    spheres.emplace_back(Vec3(1.8, 0.0, -3.0), Real(0.6), nullptr, Vec3(1.0, 0.0, 0.0)); // red (right, smaller)

    Image image(static_cast<unsigned>(W), static_cast<unsigned>(H), Color(0.0f, 0.0f, 0.0f));

    auto clampColor = [](Real value) -> float {
        if (value < 0) {
            return 0.0f;
        }
        if (value > 1) {
            return 1.0f;
        }
        return static_cast<float>(value);
    };

    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            Real u = (Real(x) + 0.5) / Real(W);
            Real v = (Real(H - 1 - y) + 0.5) / Real(H);

            Vec3 dir = (lower_left + horizontal * u + vertical * v) - cam_origin;
            Vec3 dirn = dir.normalized();
            Ray r(cam_origin, dirn);

            // Test intersection against all spheres and keep the closest hit
            Vec3 color(0.0, 0.0, 0.0);
            Real closest_t = std::numeric_limits<Real>::infinity();
            for (const auto& s : spheres) {
                auto hit = s.intersect(r);
                if (hit && hit->t < closest_t) {
                    closest_t = hit->t;
                    color = s.color();
                }
            }

            Color pixelColor(
                clampColor(color.x),
                clampColor(color.y),
                clampColor(color.z)
            );

            image.SetPixel(static_cast<unsigned>(x), static_cast<unsigned>(y), pixelColor);
        }
    }

    image.WriteFile("sphere.png");

    timer.stop();

    std::cout << "Wrote sphere.png\n";
    return 0;
}
