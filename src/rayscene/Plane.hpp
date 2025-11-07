#pragma once

#include <array>
#include "../raymath/Color.hpp"
#include "../raymath/Vec3.hpp"
#include "../raymath/Ray.hpp"
#include "../raymath/Intersection.hpp"
#include "../rayimage/Image.hpp"
#include "../rayscene/Sphere.hpp"
#include <optional>

using namespace math;
using namespace std;

class Plane {
    private:
        array<Color, 2> colors;
        float tileSize = 1.0f;
        float posY = 0.0f;

    public:
        Plane(array<Color, 2> colors, float posY = 0.0f, float tileSize = 1.0f);

        void DrawPlane(Image& image, const Vec3& camOrigin, int width, int height, const std::vector<rayscene::Sphere>& spheres, Light light, int echantillonsNumber = 1);

        optional<HitInfo> intersect(const Ray& ray) const noexcept;

        Vec3 getColorAt(const Vec3& point) const noexcept;
};
