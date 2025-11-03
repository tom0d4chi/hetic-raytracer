#include <array>
#include <iostream>

#include "raymath/Vec3.hpp"
#include "raymath/Ray.hpp"
#include "raymath/Color.hpp"
#include "rayimage/Image.hpp"
#include "raytimer/Timer.hpp"
#include "rayscene/Plane.hpp"

using Color = ::Color;
using Image = ::Image;
using math::Vec3;
using ::Plane;

int main() {
    Timer timer("Plane render");

    const int width = 1920;
    const int height = 1080;
    Color black(0, 0, 0);
    Color white(1, 1, 1);

    Image image(width, height, black);

    Vec3 cam_origin(0, 1.5f, 0);
    std::array<Color, 2> planeColors = {white, black};
    Plane plane(planeColors, 0.0f, 1.0f);

    plane.DrawPlane(image, cam_origin, width, height);

    image.WriteFile("plane.png");

    timer.stop();
    std::cout << "Plane render written to plane.png\n";
    return 0;
}
