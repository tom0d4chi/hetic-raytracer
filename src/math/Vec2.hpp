#pragma once

#include "Constants.hpp"
#include <cassert>
#include <cmath>

namespace math {

struct alignas(8) Vec2 {
    Real u, v;

    // Constructeurs
    constexpr Vec2() noexcept : u(0), v(0) {}
    constexpr Vec2(Real u_, Real v_) noexcept : u(u_), v(v_) {}
    explicit constexpr Vec2(Real s) noexcept : u(s), v(s) {}

    // Opérateurs arithmétiques
    constexpr Vec2 operator+(const Vec2& other) const noexcept {
        return Vec2(u + other.u, v + other.v);
    }
    
    constexpr Vec2 operator-(const Vec2& other) const noexcept {
        return Vec2(u - other.u, v - other.v);
    }
    
    constexpr Vec2 operator*(Real s) const noexcept {
        return Vec2(u * s, v * s);
    }

    // Opérateurs composés
    constexpr Vec2& operator+=(const Vec2& other) noexcept {
        u += other.u; v += other.v;
        return *this;
    }
    
    constexpr Vec2& operator-=(const Vec2& other) noexcept {
        u -= other.u; v -= other.v;
        return *this;
    }

    // Produit scalaire
    constexpr Real dot(const Vec2& other) const noexcept {
        return u * other.u + v * other.v;
    }

    // Longueur
    constexpr Real lengthSquared() const noexcept {
        return dot(*this);
    }
    
    Real length() const noexcept {
        return std::sqrt(lengthSquared());
    }

    // Normalisation
    Vec2& normalize() noexcept {
        Real l2 = lengthSquared();
        if (l2 > NORMAL_EPSILON * NORMAL_EPSILON) {
            Real inv_l = Real(1.0) / std::sqrt(l2);
            u *= inv_l; v *= inv_l;
        }
        return *this;
    }
};

} // namespace math