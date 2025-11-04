#include "Light.hpp"
#include "../raymath/Vec3.hpp"

using namespace math;

Light::Light(Vec3 position) : position(position) {}

Light::~ Light() {}

Vec3 Light::getPosition() {
    return position;
}
