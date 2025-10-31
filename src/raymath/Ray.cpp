#include "Ray.hpp"

#include <cassert>
#include <cmath>

namespace math {

Ray::Ray() noexcept
    : m_origin(Vec3::Zero())
    , m_direction(Vec3::UnitZ())
    , m_time(0) {}

Ray::Ray(const Vec3& origin, const Vec3& direction, Real time) noexcept
    : m_origin(origin)
    , m_direction(direction)
    , m_time(time) {
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

Real Ray::time() const noexcept {
    return m_time;
}

Vec3 Ray::at(Real t) const noexcept {
    return m_origin + m_direction * t;
}

Vec3 Ray::closestPoint(const Vec3& point) const noexcept {
    const Real t = (point - m_origin).dot(m_direction);
    return at(t);
}

Real Ray::distanceSquared(const Vec3& point) const noexcept {
    const Vec3 closest = closestPoint(point);
    return (point - closest).lengthSquared();
}

} // namespace math
