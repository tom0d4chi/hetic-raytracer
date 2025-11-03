#include <iostream>
#include <cmath>
#include "Color.hpp"
#include "Image.hpp"
#include "Timer.hpp"
#include "Ray.hpp"
#include "Plane.hpp"

using namespace std;
using namespace math;

int main()
{
    Color red(1, 0, 0);
    Color green(0, 1, 0);
    Color white(1, 1, 1);
    Color black;

    cout << "Red : " << red << std::endl;
    cout << "Green : " << green << std::endl;
    cout << "Black : " << black << std::endl;

    Color yellow = red + green;

    cout << "Yellow : " << yellow << std::endl;

    Timer liveTimer("Generation de l'image");

    int width = 1920;
    int height = 1080;
    Image image(width, height, black);

    // Paramètres de la caméra et du plan
    Vec3 cam_origin(0, 1.5f, 0);
    array<Color, 2> planeColors = {white, black};
    Plane plane(planeColors, 0.0f, 1.0f);

    plane.DrawPlane(image, cam_origin, width, height);

    image.WriteFile("test.png");

    liveTimer.stop();

    return 0;
}
