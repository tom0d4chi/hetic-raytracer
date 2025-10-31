#pragma once

#include "Vec3.hpp"
#include "Constants.hpp"

namespace math {

class Ray {
public:
    Ray() noexcept;
    Ray(const Vec3& origin, const Vec3& direction, Real time = 0) noexcept;

    const Vec3& origin() const noexcept;
    const Vec3& direction() const noexcept;
    Real time() const noexcept;

    Vec3 at(Real t) const noexcept;
    Vec3 closestPoint(const Vec3& point) const noexcept;
    Real distanceSquared(const Vec3& point) const noexcept;

private:
    Vec3 m_origin;
    Vec3 m_direction;
    Real m_time;
};

} // namespace math
