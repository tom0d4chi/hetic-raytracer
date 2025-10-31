#pragma once

#include "Constants.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"
#include "Vec2.hpp"
#include <cmath>
#include <optional>

namespace math {

// Résultat d'une intersection avec une surface
struct HitInfo {
    Real t;                    // Distance le long du rayon
    Vec3 point;               // Point d'intersection
    Vec3 normal;              // Normale au point d'intersection
    Vec2 uv;                  // Coordonnées de texture
    bool frontFace{true};     // Si on frappe la face avant

    // Définit la normale en tenant compte de la face (avant/arrière)
    void setFaceNormal(const Ray& ray, const Vec3& outwardNormal) noexcept;
};

// Résultat de la résolution d'une équation quadratique
struct QuadraticRoots {
    bool hasRoots{false};     // A-t-on des racines réelles ?
    Real t0{INFINITY};        // Première racine (la plus proche si elle existe)
    Real t1{INFINITY};        // Deuxième racine (la plus éloignée si elle existe)
};

// Résout at² + bt + c = 0 de manière numériquement stable
QuadraticRoots solveQuadratic(Real a, Real b, Real c) noexcept;

// Trouve la première intersection valide (t > min_t)
std::optional<Real> firstValidHit(const QuadraticRoots& roots, Real min_t = RAY_MIN_T) noexcept;

} // namespace math
