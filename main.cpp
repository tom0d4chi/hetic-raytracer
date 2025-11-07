#include <iostream>
#include <cmath>
#include <vector>
#include <limits>
#include <array>
#include <string>
#include "Color.hpp"
#include "Image.hpp"
#include "Timer.hpp"
#include "Ray.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "Light.hpp"
#include "SceneLoader.hpp"

using namespace std;
using namespace math;
using namespace rayscene;

int main(int argc, char* argv[])
{
    srand (static_cast <unsigned> (time(0)));

    const std::string sceneFile = (argc > 1) ? argv[1] : "../../../scene.json";

    SceneConfig sceneConfig = LoadSceneFromJson(sceneFile);

    std::cout << "Loaded scene: " << sceneFile << " (" << sceneConfig.width << "x" << sceneConfig.height << ")" << endl;

    Timer liveTimer(sceneConfig.timerLabel);

    Image image(sceneConfig.width, sceneConfig.height, sceneConfig.background);

    Light light = sceneConfig.light ? Light(sceneConfig.light->position) : Light(Vec3(-5.0, 1.5, 5.0));

    Vec3 cam_origin = sceneConfig.camera.origin;

    Plane plane = sceneConfig.plane
        ? Plane({sceneConfig.plane->primaryColor, sceneConfig.plane->secondaryColor},
                sceneConfig.plane->posY,
                sceneConfig.plane->tileSize)
        : Plane({Color(1, 1, 1), Color(0, 0, 0)}, 0.0f, 1.0f);

    vector<Sphere> spheres;
    spheres.reserve(sceneConfig.spheres.size());
    for (const auto& sphereCfg : sceneConfig.spheres) {
        spheres.emplace_back(sphereCfg.center,
                             sphereCfg.radius,
                             nullptr,
                             sphereCfg.color,
                             sphereCfg.reflectFactor,
                             sphereCfg.specularPower);
    }

    plane.DrawPlane(image, cam_origin, sceneConfig.width, sceneConfig.height, spheres, light, sceneConfig.echantillonsNumber);

    Sphere::DrawSphere(image, cam_origin, sceneConfig.width, sceneConfig.height, spheres, light, plane, sceneConfig.echantillonsNumber);

    image.WriteFile(sceneConfig.outputPath.c_str());

    liveTimer.stop();

    return 0;
}
