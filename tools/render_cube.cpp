#include <optional>
#include <memory>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

#include "raymath/Vec3.hpp"
#include "raymath/Ray.hpp"
#include "raymath/Intersection.hpp"
#include "rayscene/Cube.hpp"
#include "rayimage/Image.hpp"
#include "raymath/Color.hpp"
#include "raytimer/Timer.hpp"

#include <array>

using math::Vec3;
using math::Ray;
using rayscene::Cube;
using math::Real;
using ::Image;
using ::Color;

int main() {
    const int W = 800;
    const int H = 600;
    const Real aspect = Real(W) / Real(H);

    Timer timer("Cube render");

    // Camera: simple pinhole at (0,0,0), image plane at z = -1
    Vec3 cam_origin(0, 0, 0);
    Real viewport_height = 2.0;
    Real viewport_width = aspect * viewport_height;
    Real focal_length = 1.0;

    Vec3 horizontal(viewport_width, 0, 0);
    Vec3 vertical(0, viewport_height, 0);
    Vec3 lower_left = cam_origin - horizontal/2 - vertical/2 - Vec3(0, 0, focal_length);

    // Create a cube and rotate it around Y axis
    Vec3 cube_min(-0.7, -0.7, -3.7);
    Vec3 cube_max(0.7, 0.7, -2.3);

    // Rotation matrix around Y axis
    auto rotateY = [](const Vec3& v, Real angle) -> Vec3 {
        Real c = std::cos(angle);
        Real s = std::sin(angle);
        return Vec3(
            c * v.x + s * v.z,
            v.y,
            -s * v.x + c * v.z
        );
    };

    Real rotation_angle = 0.7; // radians
    std::array<Vec3, 8> corners = {
        cube_min,
        Vec3(cube_max.x, cube_min.y, cube_min.z),
        Vec3(cube_min.x, cube_max.y, cube_min.z),
        Vec3(cube_max.x, cube_max.y, cube_min.z),
        Vec3(cube_min.x, cube_min.y, cube_max.z),
        Vec3(cube_max.x, cube_min.y, cube_max.z),
        Vec3(cube_min.x, cube_max.y, cube_max.z),
        cube_max
    };
    for (auto& v : corners) v = rotateY(v, rotation_angle);
    // Find new AABB after rotation
    Vec3 new_min = corners[0];
    Vec3 new_max = corners[0];
    for (const auto& v : corners) {
        new_min.x = std::min(new_min.x, v.x);
        new_min.y = std::min(new_min.y, v.y);
        new_min.z = std::min(new_min.z, v.z);
        new_max.x = std::max(new_max.x, v.x);
        new_max.y = std::max(new_max.y, v.y);
        new_max.z = std::max(new_max.z, v.z);
    }
    Cube cube(new_min, new_max, nullptr, Vec3(1.0, 0.5, 0.0)); // orange

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

            // Test intersection against the rotated cube
            Vec3 color(0.0, 0.0, 0.0);
            auto hit = cube.intersect(r);
            if (hit) {
                color = cube.color();
            }

            Color pixelColor(
                clampColor(color.x),
                clampColor(color.y),
                clampColor(color.z)
            );

            image.SetPixel(static_cast<unsigned>(x), static_cast<unsigned>(y), pixelColor);
        }
    }

    image.WriteFile("cube.png");

    timer.stop();

    std::cout << "Wrote cube.png\n";
    return 0;
}
