#include "Intersection.hpp"

#include <algorithm>
#include <cmath>

namespace math {

void HitInfo::setFaceNormal(const Ray& ray, const Vec3& outwardNormal) noexcept {
    frontFace = ray.direction().dot(outwardNormal) < 0;
    normal = frontFace ? outwardNormal : -outwardNormal;
}

QuadraticRoots solveQuadratic(Real a, Real b, Real c) noexcept {
    QuadraticRoots result;

    if (std::abs(a) < EPSILON) {
        if (std::abs(b) < EPSILON) {
            return result;
        }
        result.hasRoots = true;
        result.t0 = result.t1 = -c / b;
        return result;
    }

    const Real discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return result;
    }

    result.hasRoots = true;

    const Real q = (b >= 0)
        ? -0.5 * (b + std::sqrt(discriminant))
        : -0.5 * (b - std::sqrt(discriminant));

    result.t0 = q / a;
    result.t1 = c / q;

    if (result.t0 > result.t1) {
        std::swap(result.t0, result.t1);
    }

    return result;
}

std::optional<Real> firstValidHit(const QuadraticRoots& roots, Real min_t) noexcept {
    if (!roots.hasRoots) return std::nullopt;

    if (roots.t0 > min_t) return roots.t0;
    if (roots.t1 > min_t) return roots.t1;

    return std::nullopt;
}

} // namespace math
