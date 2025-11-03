#include "Ray.hpp"

#include <cassert>
#include <cmath>

namespace math {

Ray::Ray() noexcept
    : m_origin(Vec3::Zero())
    , m_direction(Vec3::UnitZ()) {}

Ray::Ray(const Vec3& origin, const Vec3& direction) noexcept
    : m_origin(origin)
    , m_direction(direction) {
#ifndef NDEBUG
    assert(std::abs(direction.lengthSquared() - 1.0) < NORMAL_EPSILON);
#endif
}

const Vec3& Ray::origin() const noexcept {
    return m_origin;
}

const Vec3& Ray::direction() const noexcept {
    return m_direction;
}

Vec3 Ray::at(Real t) const noexcept {
    return m_origin + m_direction * t;
}

} // namespace math
