#pragma once

#include <array>
#include "../raymath/Color.hpp"
#include "../raymath/Vec3.hpp"
#include "../rayimage/Image.hpp"
#include "../rayscene/Sphere.hpp"
#include "../rayscene/Light.hpp" 

using namespace math;
using namespace std;
using namespace rayscene;

class Plane {
    private:
        array<Color, 2> colors;
        float tileSize = 1.0f;
        float posY = 0.0f;

    public:
        Plane(array<Color, 2> colors, float posY = 0.0f, float tileSize = 1.0f);

       
        void DrawPlane(Image& image, 
                       const Vec3& camOrigin, 
                       int width, 
                       int height, 
                       const std::vector<Sphere>& spheres, 
                       const Light& light, 
                       std::vector<math::Real>& zbuf);
};