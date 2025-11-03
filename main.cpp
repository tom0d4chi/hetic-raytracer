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

    // Paramètres de la caméra et du plan
    Vec3 cam_origin(0, 1.5f, 0);
    array<Color, 2> planeColors = {white, black};
    Plane plane(planeColors, 0.0f, 1.0f);

    plane.DrawPlane(image, cam_origin, width, height);

    // Rendu des sphères
    Real aspect = Real(width) / Real(height);
    Real viewport_height = 2.0;
    Real viewport_width = aspect * viewport_height;
    Real focal_length = 1.0;

    Vec3 horizontal(viewport_width, 0, 0);
    Vec3 vertical(0, viewport_height, 0);
    Vec3 lower_left = cam_origin - horizontal/2 - vertical/2 - Vec3(0, 0, focal_length);

    // Création de plusieurs sphères avec couleurs, positions et tailles variées
    vector<Sphere> spheres;

    // Rangée arrière (plus éloignée)
    spheres.emplace_back(Vec3(-4.0, 2.5, -8.0), Real(1.2), nullptr, Vec3(0.4, 0.7, 1.0)); // bleu clair (gauche arrière)
    spheres.emplace_back(Vec3(-1.5, 2.2, -7.0), Real(1.5), nullptr, Vec3(1.0, 0.3, 0.8)); // rose/magenta (arrière)
    spheres.emplace_back(Vec3(2.0, 1.5, -6.5), Real(0.8), nullptr, Vec3(0.2, 1.0, 0.3)); // vert clair (arrière droite)
    spheres.emplace_back(Vec3(4.5, 2.8, -9.0), Real(1.5), nullptr, Vec3(0.7, 0.5, 1.0)); // violet (droite arrière)

    // Rangée milieu
    spheres.emplace_back(Vec3(-3.0, 1.2, -4.5), Real(0.7), nullptr, Vec3(0.2, 0.8, 0.5)); // vert émeraude
    spheres.emplace_back(Vec3(-0.5, 1.8, -5.0), Real(1.0), nullptr, Vec3(0.1, 1.0, 0.8)); // cyan
    spheres.emplace_back(Vec3(1.5, 1.4, -4.0), Real(0.9), nullptr, Vec3(0.3, 0.5, 1.0)); // bleu
    spheres.emplace_back(Vec3(3.5, 2.0, -5.5), Real(1.3), nullptr, Vec3(0.2, 1.0, 0.2)); // vert vif
    spheres.emplace_back(Vec3(5.0, 1.6, -5.0), Real(1.0), nullptr, Vec3(1.0, 0.8, 0.3)); // orange/jaune

    // Rangée avant
    spheres.emplace_back(Vec3(-2.5, 1.0, -2.5), Real(0.7), nullptr, Vec3(0.5, 1.0, 1.0)); // cyan clair
    spheres.emplace_back(Vec3(0.0, 1.3, -3.0), Real(0.8), nullptr, Vec3(1.0, 0.2, 0.5)); // rose
    spheres.emplace_back(Vec3(2.0, 0.9, -2.0), Real(0.6), nullptr, Vec3(1.0, 1.0, 0.2)); // jaune

    auto clampColor = [](Real value) -> float {
        if (value < 0) {
            return 0.0f;
        }
        if (value > 1) {
            return 1.0f;
        }
        return static_cast<float>(value);
    };

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Real u = (Real(x) + 0.5) / Real(width);
            Real v = (Real(height - 1 - y) + 0.5) / Real(height);

            Vec3 dir = (lower_left + horizontal * u + vertical * v) - cam_origin;
            Vec3 dirn = dir.normalized();
            Ray r(cam_origin, dirn);

            // Test d'intersection avec toutes les sphères et garde la plus proche
            Vec3 sphere_color(0.0, 0.0, 0.0);
            Real closest_t = numeric_limits<Real>::infinity();
            for (const auto& s : spheres) {
                auto hit = s.intersect(r);
                if (hit && hit->t < closest_t) {
                    closest_t = hit->t;
                    sphere_color = s.color();
                }
            }

            // Si on a touché une sphère, on utilise sa couleur
            if (closest_t < numeric_limits<Real>::infinity()) {
                Color pixelColor(
                    clampColor(sphere_color.x),
                    clampColor(sphere_color.y),
                    clampColor(sphere_color.z)
                );
                image.SetPixel(static_cast<unsigned>(x), static_cast<unsigned>(y), pixelColor);
            }
        }
    }

    image.WriteFile("test.png");

    liveTimer.stop();

    return 0;
}
