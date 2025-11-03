#include <iostream>
#include <vector>

#include "raymath/Vec3.hpp"
#include "rayscene/Sphere.hpp"
#include "rayimage/Image.hpp"
#include "raymath/Color.hpp"
#include "raytimer/Timer.hpp"

using math::Vec3;
using math::Real;
using rayscene::Sphere;
using ::Image;
using ::Color;

int main() {
    const int W = 800;
    const int H = 600;

    Timer timer("Sphere render");

    // Camera: simple pinhole located at (0, 0, 5) looking toward -Z
    Vec3 cam_origin(0, 0, 5);

    // Create three spheres with distinct colors and positions
    std::vector<Sphere> spheres;
    spheres.emplace_back(Vec3(0.0, 0.0, -3.0), Real(1.0), nullptr, Vec3(0.0, 1.0, 0.0)); // green (center)
    spheres.emplace_back(Vec3(-1.8, 0.0, -3.0), Real(1.0), nullptr, Vec3(0.0, 0.0, 1.0)); // blue (left)
    spheres.emplace_back(Vec3(1.8, 0.0, -3.0), Real(0.6), nullptr, Vec3(1.0, 0.0, 0.0));  // red (right, smaller)

    Image image(static_cast<unsigned>(W), static_cast<unsigned>(H), Color(0.0f, 0.0f, 0.0f));

    Sphere::DrawSphere(image, cam_origin, W, H, spheres);

    image.WriteFile("sphere.png");

    timer.stop();

    std::cout << "Wrote sphere.png\n";
    return 0;
}
