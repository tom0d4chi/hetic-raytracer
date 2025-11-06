#pragma once

#include <vector>
#include "../raymath/Color.hpp"
#include "../rayscene/Light.hpp"
#include "../rayscene/Sphere.hpp"
#include "../raymath/Intersection.hpp"

using namespace std;

class DiffuseShader {
    private:

    public:
        float Shade(math::HitInfo hitInfo, Light light, const std::vector<rayscene::Sphere>& spheres, Vec3 camera, int specularPower);

        float ShadowFactorPlane(math::HitInfo hitInfo, Light light, const std::vector<rayscene::Sphere>& spheres);
};
