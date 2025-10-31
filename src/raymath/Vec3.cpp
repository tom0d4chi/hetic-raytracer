#include "Vec3.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>

namespace math {

Vec3::Vec3() noexcept : x(0), y(0), z(0) {}

Vec3::Vec3(Real x_, Real y_, Real z_) noexcept : x(x_), y(y_), z(z_) {}

Vec3::Vec3(Real s) noexcept : x(s), y(s), z(s) {}

Real Vec3::operator[](int i) const noexcept {
#ifdef NDEBUG
    return (&x)[i];
#else
    return (i >= 0 && i < 3) ? (&x)[i] : (assert(false), x);
#endif
}

Real& Vec3::operator[](int i) noexcept {
#ifdef NDEBUG
    return (&x)[i];
#else
    return (i >= 0 && i < 3) ? (&x)[i] : (assert(false), x);
#endif
}

Vec3 Vec3::operator-() const noexcept {
    return Vec3(-x, -y, -z);
}

Vec3 Vec3::operator+(const Vec3& v) const noexcept {
    return Vec3(x + v.x, y + v.y, z + v.z);
}

Vec3 Vec3::operator-(const Vec3& v) const noexcept {
    return Vec3(x - v.x, y - v.y, z - v.z);
}

Vec3 Vec3::operator*(Real s) const noexcept {
    return Vec3(x * s, y * s, z * s);
}

Vec3 Vec3::operator/(Real s) const noexcept {
    Real inv_s = Real(1.0) / s;
    return Vec3(x * inv_s, y * inv_s, z * inv_s);
}

Vec3 Vec3::cwiseProduct(const Vec3& v) const noexcept {
    return Vec3(x * v.x, y * v.y, z * v.z);
}

Vec3& Vec3::operator+=(const Vec3& v) noexcept {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Vec3& Vec3::operator-=(const Vec3& v) noexcept {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

Vec3& Vec3::operator*=(Real s) noexcept {
    x *= s;
    y *= s;
    z *= s;
    return *this;
}

Vec3& Vec3::operator/=(Real s) noexcept {
    Real inv_s = Real(1.0) / s;
    x *= inv_s;
    y *= inv_s;
    z *= inv_s;
    return *this;
}

Real Vec3::dot(const Vec3& v) const noexcept {
    return x * v.x + y * v.y + z * v.z;
}

Vec3 Vec3::cross(const Vec3& v) const noexcept {
    return Vec3(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x
    );
}

Real Vec3::lengthSquared() const noexcept {
    return dot(*this);
}

Real Vec3::length() const noexcept {
    return std::sqrt(lengthSquared());
}

Vec3& Vec3::normalize() noexcept {
    Real l2 = lengthSquared();
    if (l2 > NORMAL_EPSILON * NORMAL_EPSILON) {
        Real inv_l = Real(1.0) / std::sqrt(l2);
        x *= inv_l;
        y *= inv_l;
        z *= inv_l;
    }
    return *this;
}

Vec3 Vec3::normalized() const noexcept {
    Vec3 v = *this;
    v.normalize();
    return v;
}

Vec3 Vec3::reflect(const Vec3& n) const noexcept {
    return *this - n * (2 * this->dot(n));
}

bool Vec3::isZero() const noexcept {
    return lengthSquared() < EPSILON * EPSILON;
}

bool Vec3::hasNaN() const noexcept {
    return std::isnan(x) || std::isnan(y) || std::isnan(z);
}

Vec3 Vec3::Zero() noexcept { return Vec3(0, 0, 0); }
Vec3 Vec3::One() noexcept { return Vec3(1, 1, 1); }
Vec3 Vec3::UnitX() noexcept { return Vec3(1, 0, 0); }
Vec3 Vec3::UnitY() noexcept { return Vec3(0, 1, 0); }
Vec3 Vec3::UnitZ() noexcept { return Vec3(0, 0, 1); }

Vec3 operator*(Real s, const Vec3& v) noexcept {
    return v * s;
}

Vec3 min(const Vec3& a, const Vec3& b) noexcept {
    return Vec3(
        (a.x < b.x) ? a.x : b.x,
        (a.y < b.y) ? a.y : b.y,
        (a.z < b.z) ? a.z : b.z
    );
}

Vec3 max(const Vec3& a, const Vec3& b) noexcept {
    return Vec3(
        (a.x > b.x) ? a.x : b.x,
        (a.y > b.y) ? a.y : b.y,
        (a.z > b.z) ? a.z : b.z
    );
}

Vec3 lerp(const Vec3& a, const Vec3& b, Real t) noexcept {
    return a * (Real(1) - t) + b * t;
}

bool approxEqual(const Vec3& a, const Vec3& b, Real eps) noexcept {
    Vec3 diff = a - b;
    return diff.lengthSquared() < eps * eps;
}

} // namespace math
