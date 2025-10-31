#pragma once

#include "../../math/include/math/Vec3.hpp"
#include "../../math/include/math/Ray.hpp"
#include "../../math/include/math/Intersection.hpp"

#include <memory>

namespace render {

struct Material; // forward declare (project-specific)

// HitRecord-like wrapper for render module that reuses math::HitInfo
using math::HitInfo;
using math::Ray;
using math::Vec3;
using math::QuadraticRoots;

class Sphere {
public:
    Sphere(const Vec3& center, math::Real radius, std::shared_ptr<Material> mat = nullptr) noexcept
        : m_center(center)
        , m_radius(radius)
        , m_radius2(radius * radius)
        , m_material(std::move(mat))
    {}

    // Convenience constructor with a per-sphere color (albedo)
    Sphere(const Vec3& center, math::Real radius, std::shared_ptr<Material> mat, const Vec3& color) noexcept
        : m_center(center)
        , m_radius(radius)
        , m_radius2(radius * radius)
        , m_material(std::move(mat))
        , m_color(color)
    {}

    const Vec3& center() const noexcept { return m_center; }
    math::Real radius() const noexcept { return m_radius; }
    const Vec3& color() const noexcept { return m_color; }

    // Intersect the sphere with a ray. Returns an optional HitInfo when hit.
    inline std::optional<HitInfo> intersect(const Ray& ray) const noexcept {
        // Ray: P(t) = O + t D
        const Vec3 oc = ray.origin() - m_center;
        const math::Real a = ray.direction().dot(ray.direction());
        const math::Real b = 2 * oc.dot(ray.direction());
        const math::Real c = oc.dot(oc) - m_radius2;

        const auto roots = math::solveQuadratic(a, b, c);
        const auto tOpt = math::firstValidHit(roots, math::RAY_MIN_T);
        if (!tOpt) return std::nullopt;

        const math::Real t = *tOpt;
        HitInfo info;
        info.t = t;
        info.point = ray.at(t);
        const Vec3 outward = (info.point - m_center) / m_radius;
        info.setFaceNormal(ray, outward);

        // UV spherical coordinates
        const math::Real theta = std::acos(std::clamp(outward.y, math::Real(-1), math::Real(1)));
        const math::Real phi = std::atan2(outward.z, outward.x);
        info.uv.u = (phi + math::TWO_PI) / math::TWO_PI;
        info.uv.v = theta / math::PI;

        return info;
    }

private:
    Vec3 m_center;
    math::Real m_radius;
    math::Real m_radius2;
    std::shared_ptr<Material> m_material;
    Vec3 m_color{Vec3(0, 1, 0)}; // default green for backward compatibility
};

} // namespace render