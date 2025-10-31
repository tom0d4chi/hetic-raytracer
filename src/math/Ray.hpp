#pragma once

#include "Vec3.hpp"
#include "Constants.hpp"
#include <cassert>

namespace math {

class Ray {
public:
    // Constructeurs
    constexpr Ray() noexcept = default;
    
    constexpr Ray(const Vec3& origin, const Vec3& direction, Real time = 0) noexcept
        : m_origin(origin)
        , m_direction(direction)
        , m_time(time) 
    {
        #ifndef NDEBUG
            // En debug, on vérifie que la direction est à peu près normalisée
            assert(std::abs(direction.lengthSquared() - 1.0) < NORMAL_EPSILON);
        #endif
    }

    // Accesseurs
    constexpr const Vec3& origin() const noexcept { return m_origin; }
    constexpr const Vec3& direction() const noexcept { return m_direction; }
    constexpr Real time() const noexcept { return m_time; }

    // Point le long du rayon à la distance t
    constexpr Vec3 at(Real t) const noexcept {
        return m_origin + m_direction * t;
    }

    // Point le plus proche sur le rayon d'un point donné
    constexpr Vec3 closestPoint(const Vec3& point) const noexcept {
        const Real t = (point - m_origin).dot(m_direction);
        return at(t);
    }

    // Distance au carré entre un point et le rayon
    constexpr Real distanceSquared(const Vec3& point) const noexcept {
        const Vec3 closest = closestPoint(point);
        return (point - closest).lengthSquared();
    }

private:
    Vec3 m_origin{Vec3::Zero()};       // Point de départ du rayon
    Vec3 m_direction{Vec3::UnitZ()};   // Direction normalisée
    Real m_time{0};                    // Temps pour motion blur
};

} // namespace math
