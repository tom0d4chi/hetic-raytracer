#pragma once

#include <array>
#include "../raymath/Color.hpp"
#include "../raymath/Vec3.hpp"
#include "../rayimage/Image.hpp"

using namespace math;
using namespace std;

class Plane {
    private:
        array<Color, 2> colors;
        float tileSize = 1.0f;
        float posY = 0.0f;

    public:
        Plane(array<Color, 2> colors, float posY = 0.0f, float tileSize = 1.0f);

        void DrawPlane(Image& image, const Vec3& camOrigin, int width, int height);

        // TODO implement intersect()

        // TODO implement getColorAt()
};
