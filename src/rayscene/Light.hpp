#pragma once

#include "../raymath/Vec3.hpp"

using namespace math;

class Light {
    private:
        Vec3 position;

    public:
        Light(Vec3 position);
        ~ Light();

        Vec3 getPosition();
};
