#pragma once

#include "Constants.hpp"
#include <cassert>
#include <cmath>
#include <algorithm>  // pour std::min, std::max

namespace math {

struct alignas(16) Vec3 {
    Real x, y, z;

    Vec3() noexcept;
    Vec3(Real x_, Real y_, Real z_) noexcept;
    explicit Vec3(Real s) noexcept;

    Real operator[](int i) const noexcept;
    Real& operator[](int i) noexcept;

    Vec3 operator-() const noexcept;
    Vec3 operator+(const Vec3& v) const noexcept;
    Vec3 operator-(const Vec3& v) const noexcept;
    Vec3 operator*(Real s) const noexcept;
    Vec3 operator/(Real s) const noexcept;

    Vec3 cwiseProduct(const Vec3& v) const noexcept;

    Vec3& operator+=(const Vec3& v) noexcept;
    Vec3& operator-=(const Vec3& v) noexcept;
    Vec3& operator*=(Real s) noexcept;
    Vec3& operator/=(Real s) noexcept;

    Real dot(const Vec3& v) const noexcept;
    Vec3 cross(const Vec3& v) const noexcept;

    Real lengthSquared() const noexcept;
    Real length() const noexcept;

    Vec3& normalize() noexcept;
    [[nodiscard]] Vec3 normalized() const noexcept;

    [[nodiscard]] Vec3 reflect(const Vec3& n) const noexcept;

    bool isZero() const noexcept;
    bool hasNaN() const noexcept;

    static Vec3 Zero() noexcept;
    static Vec3 One() noexcept;
    static Vec3 UnitX() noexcept;
    static Vec3 UnitY() noexcept;
    static Vec3 UnitZ() noexcept;
};

Vec3 operator*(Real s, const Vec3& v) noexcept;
Vec3 min(const Vec3& a, const Vec3& b) noexcept;
Vec3 max(const Vec3& a, const Vec3& b) noexcept;
Vec3 lerp(const Vec3& a, const Vec3& b, Real t) noexcept;
bool approxEqual(const Vec3& a, const Vec3& b, Real eps = EPSILON) noexcept;

} // namespace math
