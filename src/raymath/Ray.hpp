#pragma once

#include "Vec3.hpp"
#include "Constants.hpp"

namespace math {

class Ray {
public:
    Ray() noexcept;
    Ray(const Vec3& origin, const Vec3& direction) noexcept;

    const Vec3& origin() const noexcept;
    const Vec3& direction() const noexcept;

    Vec3 at(Real t) const noexcept;

private:
    Vec3 m_origin;
    Vec3 m_direction;
};

} // namespace math
