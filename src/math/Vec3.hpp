#pragma once

#include "Constants.hpp"
#include <cassert>
#include <cmath>
#include <algorithm>  // pour std::min, std::max

namespace math {

// Vec3: Vecteur 3D optimisé pour le raytracing
struct alignas(16) Vec3 {
    Real x, y, z;

    // Constructeurs
    constexpr Vec3() noexcept : x(0), y(0), z(0) {}
    constexpr Vec3(Real x_, Real y_, Real z_) noexcept : x(x_), y(y_), z(z_) {}
    explicit constexpr Vec3(Real s) noexcept : x(s), y(s), z(s) {}

    // Accès indexé
    constexpr Real operator[](int i) const noexcept {
        #ifdef NDEBUG
            return (&x)[i];
        #else
            return (i >= 0 && i < 3) ? (&x)[i] : (assert(false), x);
        #endif
    }
    constexpr Real& operator[](int i) noexcept {
        #ifdef NDEBUG
            return (&x)[i];
        #else
            return (i >= 0 && i < 3) ? (&x)[i] : (assert(false), x);
        #endif
    }

    // Opérateurs arithmétiques (const)
    constexpr Vec3 operator-() const noexcept { 
        return Vec3(-x, -y, -z); 
    }
    
    constexpr Vec3 operator+(const Vec3& v) const noexcept { 
        return Vec3(x + v.x, y + v.y, z + v.z); 
    }
    
    constexpr Vec3 operator-(const Vec3& v) const noexcept { 
        return Vec3(x - v.x, y - v.y, z - v.z); 
    }
    
    constexpr Vec3 operator*(Real s) const noexcept { 
        return Vec3(x * s, y * s, z * s); 
    }
    
    constexpr Vec3 operator/(Real s) const noexcept {
        Real inv_s = Real(1.0) / s;
        return Vec3(x * inv_s, y * inv_s, z * inv_s);
    }

    // Multiplication composante par composante (Hadamard)
    constexpr Vec3 cwiseProduct(const Vec3& v) const noexcept {
        return Vec3(x * v.x, y * v.y, z * v.z);
    }

    // Opérateurs composés (in-place)
    constexpr Vec3& operator+=(const Vec3& v) noexcept {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }
    
    constexpr Vec3& operator-=(const Vec3& v) noexcept {
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }
    
    constexpr Vec3& operator*=(Real s) noexcept {
        x *= s; y *= s; z *= s;
        return *this;
    }
    
    constexpr Vec3& operator/=(Real s) noexcept {
        Real inv_s = Real(1.0) / s;
        x *= inv_s; y *= inv_s; z *= inv_s;
        return *this;
    }

    // Produit scalaire et vectoriel
    constexpr Real dot(const Vec3& v) const noexcept {
        return x * v.x + y * v.y + z * v.z;
    }
    
    constexpr Vec3 cross(const Vec3& v) const noexcept {
        return Vec3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }

    // Opérations de longueur et normalisation
    constexpr Real lengthSquared() const noexcept {
        return dot(*this);
    }
    
    Real length() const noexcept {
        return ::sqrt(lengthSquared());
    }

    // Normalisation avec gestion des cas limites
    Vec3& normalize() noexcept {
        Real l2 = lengthSquared();
        if (l2 > NORMAL_EPSILON * NORMAL_EPSILON) {
            Real inv_l = Real(1.0) / ::sqrt(l2);
            x *= inv_l; y *= inv_l; z *= inv_l;
        }
        return *this;
    }
    
    [[nodiscard]] Vec3 normalized() const noexcept {
        Vec3 v = *this;
        v.normalize();
        return v;
    }

    // Réflexion et réfraction
    [[nodiscard]] Vec3 reflect(const Vec3& n) const noexcept {
        return *this - n * (2 * this->dot(n));
    }

    // Utilitaires
    constexpr bool isZero() const noexcept {
        return lengthSquared() < EPSILON * EPSILON;
    }
    
    constexpr bool hasNaN() const noexcept {
        return ::isnan(x) || ::isnan(y) || ::isnan(z);
    }

    // Helpers statiques pour vecteurs communs
    static constexpr Vec3 Zero() noexcept { return Vec3(0, 0, 0); }
    static constexpr Vec3 One() noexcept { return Vec3(1, 1, 1); }
    static constexpr Vec3 UnitX() noexcept { return Vec3(1, 0, 0); }
    static constexpr Vec3 UnitY() noexcept { return Vec3(0, 1, 0); }
    static constexpr Vec3 UnitZ() noexcept { return Vec3(0, 0, 1); }
};

// Opérateurs externes
inline constexpr Vec3 operator*(Real s, const Vec3& v) noexcept {
    return v * s;
}

// Min/Max composante par composante
inline constexpr Vec3 min(const Vec3& a, const Vec3& b) noexcept {
    return Vec3(
        (a.x < b.x) ? a.x : b.x,
        (a.y < b.y) ? a.y : b.y,
        (a.z < b.z) ? a.z : b.z
    );
}

inline constexpr Vec3 max(const Vec3& a, const Vec3& b) noexcept {
    return Vec3(
        (a.x > b.x) ? a.x : b.x,
        (a.y > b.y) ? a.y : b.y,
        (a.z > b.z) ? a.z : b.z
    );
}

// Interpolation linéaire
inline constexpr Vec3 lerp(const Vec3& a, const Vec3& b, Real t) noexcept {
    return a * (Real(1) - t) + b * t;
}

// Test d'égalité approximative
inline constexpr bool approxEqual(const Vec3& a, const Vec3& b, Real eps = EPSILON) noexcept {
    Vec3 diff = a - b;
    return diff.lengthSquared() < eps * eps;
}

} // namespace math
