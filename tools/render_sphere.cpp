#include <optional>
#include <memory>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

#include "math/Vec3.hpp"
#include "math/Ray.hpp"
#include "math/Vec2.hpp"
#include "math/Intersection.hpp"
#include "render/Sphere.hpp"
#include "lodepng.h"

using math::Vec3;
using math::Ray;
using render::Sphere;
using math::Real;

int main() {
    const int W = 800;
    const int H = 600;
    const Real aspect = Real(W) / Real(H);

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

    std::vector<unsigned char> pixels;
    pixels.resize(static_cast<std::size_t>(W) * static_cast<std::size_t>(H) * 4);

    for (int j = H-1; j >= 0; --j) {
        for (int i = 0; i < W; ++i) {
            Real u = (Real(i) + 0.5) / Real(W);
            Real v = (Real(j) + 0.5) / Real(H);

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

            // clamp and convert to bytes
            auto toByte = [](Real x)->unsigned char {
                if (x < 0) x = 0;
                if (x > 1) x = 1;
                return static_cast<unsigned char>(std::floor(255.99 * x));
            };

            unsigned char r8 = toByte(color.x);
            unsigned char g8 = toByte(color.y);
            unsigned char b8 = toByte(color.z);

            const int row = (H - 1) - j;
            const std::size_t idx = (static_cast<std::size_t>(row) * static_cast<std::size_t>(W) + static_cast<std::size_t>(i)) * 4;
            pixels[idx + 0] = r8;
            pixels[idx + 1] = g8;
            pixels[idx + 2] = b8;
            pixels[idx + 3] = 255; // opaque alpha for PNG
        }
    }

    unsigned error = lodepng::encode("sphere.png", pixels, static_cast<unsigned>(W), static_cast<unsigned>(H));
    if (error) {
        std::cerr << "PNG encode error " << error << ": " << lodepng_error_text(error) << "\n";
        return 1;
    }

    std::cout << "Wrote sphere.png\n";
    return 0;
}
