#include <iostream>
#include <cmath>
#include <vector>
#include <limits>
#include "Color.hpp"
#include "Image.hpp"
#include "Timer.hpp"
#include "Ray.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "Light.hpp"

using namespace std;
using namespace math;
using namespace rayscene;

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

    Light light(Vec3(-5.0, 1.5, 5.0));

    Vec3 lightPos = light.getPosition();

    // Paramètres de la caméra et du plan
    Vec3 cam_origin(0, 1.5f, -8);
    array<Color, 2> planeColors = {white, black};
    Plane plane(planeColors, 0.0f, 1.0f);

    plane.DrawPlane(image, cam_origin, width, height);

    // Création de plusieurs sphères avec couleurs, positions et tailles variées
    vector<Sphere> spheres;

    // Zone très avant-plan (z: 1.0 à 3.0)
    spheres.emplace_back(Vec3(-1.0, 0.6, 1.0), Real(0.4), nullptr, Vec3(1.0, 0.3, 0.3)); // rouge
    spheres.emplace_back(Vec3(-3.5, 0.8, 1.7), Real(0.5), nullptr, Vec3(0.7, 1.0, 0.3)); // vert-jaune
    spheres.emplace_back(Vec3(1.8, 0.7, 2.4), Real(0.6), nullptr, Vec3(0.2, 0.6, 1.0));  // bleu moyen
    spheres.emplace_back(Vec3(-2.0, 0.9, 3.0), Real(0.6), nullptr, Vec3(1.0, 1.0, 0.2)); // jaune

    // Zone avant-plan proche (z: 3.7 à 6.0)
    spheres.emplace_back(Vec3(2.5, 1.0, 3.7), Real(0.7), nullptr, Vec3(0.5, 1.0, 1.0));  // cyan clair
    spheres.emplace_back(Vec3(0.0, 1.3, 4.5), Real(0.8), nullptr, Vec3(1.0, 0.2, 0.5));  // rose
    spheres.emplace_back(Vec3(-1.5, 1.4, 5.2), Real(0.9), nullptr, Vec3(0.3, 0.5, 1.0)); // bleu
    spheres.emplace_back(Vec3(-5.5, 1.2, 6.0), Real(0.7), nullptr, Vec3(0.0, 1.0, 1.0)); // cyan vif

    // Zone milieu (z: 6.7 à 9.5)
    spheres.emplace_back(Vec3(3.0, 1.2, 6.7), Real(0.7), nullptr, Vec3(0.2, 0.8, 0.5)); // vert émeraude
    spheres.emplace_back(Vec3(5.0, 1.5, 7.5), Real(0.8), nullptr, Vec3(1.0, 1.0, 0.0)); // jaune vif
    spheres.emplace_back(Vec3(0.5, 1.8, 8.3), Real(1.0), nullptr, Vec3(0.1, 1.0, 0.8)); // cyan
    spheres.emplace_back(Vec3(6.0, 1.8, 9.1), Real(0.9), nullptr, Vec3(1.0, 0.4, 0.4)); // rouge clair

    // Zone milieu-arrière (z: 9.8 à 12.5)
    spheres.emplace_back(Vec3(-3.5, 2.0, 9.8), Real(1.3), nullptr, Vec3(0.2, 1.0, 0.2));  // vert vif
    spheres.emplace_back(Vec3(-5.0, 1.6, 10.5), Real(1.0), nullptr, Vec3(1.0, 0.8, 0.3)); // orange/jaune
    spheres.emplace_back(Vec3(4.0, 2.0, 11.2), Real(1.0), nullptr, Vec3(0.6, 0.3, 0.8));  // violet
    spheres.emplace_back(Vec3(-2.0, 1.5, 11.8), Real(0.8), nullptr, Vec3(0.2, 1.0, 0.3)); // vert clair

    // Zone arrière (z: 13.2 à 15.5)
    spheres.emplace_back(Vec3(1.5, 2.2, 13.2), Real(1.5), nullptr, Vec3(1.0, 0.3, 0.8));  // rose/magenta
    spheres.emplace_back(Vec3(-6.0, 1.8, 13.8), Real(1.1), nullptr, Vec3(1.0, 0.5, 0.0)); // orange
    spheres.emplace_back(Vec3(3.5, 2.5, 14.4), Real(1.3), nullptr, Vec3(0.3, 0.9, 0.6));  // vert menthe
    spheres.emplace_back(Vec3(-1.0, 2.5, 15.0), Real(1.2), nullptr, Vec3(0.4, 0.7, 1.0)); // bleu clair
    spheres.emplace_back(Vec3(0.8, 3.0, 15.5), Real(1.2), nullptr, Vec3(0.9, 0.5, 0.2));  // orange foncé

    // Zone arrière lointaine (z: 16.2 à 18.0)
    spheres.emplace_back(Vec3(-6.5, 2.8, 16.2), Real(1.4), nullptr, Vec3(1.0, 0.7, 0.8)); // rose pâle
    spheres.emplace_back(Vec3(-4.5, 2.8, 16.8), Real(1.5), nullptr, Vec3(1.0, 0.0, 0.0)); // violet
    spheres.emplace_back(Vec3(-0.5, 3.5, 17.4), Real(1.0), nullptr, Vec3(0.5, 0.8, 1.0)); // bleu ciel
    spheres.emplace_back(Vec3(2.5, 3.2, 18.0), Real(1.5), nullptr, Vec3(0.4, 0.4, 1.0));  // bleu profond

    // Zone très lointaine (z: 18.6 à 20.0)
    spheres.emplace_back(Vec3(-3.0, 2.5, 18.6), Real(1.3), nullptr, Vec3(0.8, 1.0, 0.8)); // vert pâle
    spheres.emplace_back(Vec3(0.0, 2.0, 19.3), Real(1.6), nullptr, Vec3(1.0, 0.6, 0.0));  // or
    spheres.emplace_back(Vec3(-7.0, 2.3, 20.0), Real(0.9), nullptr, Vec3(0.8, 0.2, 0.9)); // violet foncé

    Sphere::DrawSphere(image, cam_origin, width, height, spheres, light);

    image.WriteFile("test.png");

    liveTimer.stop();

    return 0;
}
