#pragma once

#include <array>
#include "../raymath/Color.hpp"
#include "../raymath/Vec3.hpp"
#include "../rayimage/Image.hpp"

class Plane {
    private:
        std::array<Color, 2> colors;
        float tileSize = 1.0f;
        float posY = 0.0f;

    public:
        Plane(std::array<Color, 2> colors, float posY = 0.0f, float tileSize = 1.0f);

        void DrawPlane(Image& image,
                       const math::Vec3& camOrigin,
                       const math::Vec3& lowerLeft,
                       const math::Vec3& horizontal,
                       const math::Vec3& vertical,
                       int width,
                       int height);

        void DrawPlane(Image& image,
                       const math::Vec3& camOrigin,
                       int width,
                       int height);

        // TODO implement intersect()

        // TODO implement getColorAt()
};
