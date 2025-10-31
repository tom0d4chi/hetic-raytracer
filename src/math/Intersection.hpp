#pragma once

#include "Constants.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"
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
    constexpr void setFaceNormal(const Ray& ray, const Vec3& outwardNormal) noexcept {
        frontFace = ray.direction().dot(outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

// Résultat de la résolution d'une équation quadratique
struct QuadraticRoots {
    bool hasRoots{false};     // A-t-on des racines réelles ?
    Real t0{INFINITY};        // Première racine (la plus proche si elle existe)
    Real t1{INFINITY};        // Deuxième racine (la plus éloignée si elle existe)
};

// Résout at² + bt + c = 0 de manière numériquement stable
constexpr QuadraticRoots solveQuadratic(Real a, Real b, Real c) noexcept {
    QuadraticRoots result;
    
    // Si a est presque zéro, l'équation est linéaire
    if (std::abs(a) < EPSILON) {
        if (std::abs(b) < EPSILON) {
            return result;  // Pas de solution
        }
        result.hasRoots = true;
        result.t0 = result.t1 = -c / b;
        return result;
    }

    const Real discriminant = b * b - 4 * a * c;
    
    if (discriminant < 0) {
        return result;  // Pas de solution réelle
    }

    result.hasRoots = true;

    // Méthode numériquement stable pour éviter la perte de précision
    // Utilise la formule q = -0.5 * (b + sign(b) * sqrt(discriminant))
    // pour éviter la soustraction de grands nombres similaires
    const Real q = (b >= 0) ?
        -0.5 * (b + std::sqrt(discriminant)) :
        -0.5 * (b - std::sqrt(discriminant));

    result.t0 = q / a;
    result.t1 = c / q;

    // Assure que t0 <= t1
    if (result.t0 > result.t1) {
        std::swap(result.t0, result.t1);
    }

    return result;
}

// Trouve la première intersection valide (t > min_t)
inline std::optional<Real> firstValidHit(const QuadraticRoots& roots, Real min_t = RAY_MIN_T) noexcept {
    if (!roots.hasRoots) return std::nullopt;
    
    if (roots.t0 > min_t) return roots.t0;
    if (roots.t1 > min_t) return roots.t1;
    
    return std::nullopt;
}

} // namespace math
