#include "Sphere.hpp"

#include <algorithm>
#include <cmath>

namespace rayscene {

using math::HitInfo;
using math::Ray;
using math::Vec3;

Sphere::Sphere(const Vec3& center, math::Real radius, std::shared_ptr<Material> mat) noexcept
    : m_center(center)
    , m_radius(radius)
    , m_radius2(radius * radius)
    , m_material(std::move(mat))
    , m_color(Vec3(0, 1, 0)) // default green
{}

Sphere::Sphere(const Vec3& center, math::Real radius, std::shared_ptr<Material> mat, const Vec3& color) noexcept
    : m_center(center)
    , m_radius(radius)
    , m_radius2(radius * radius)
    , m_material(std::move(mat))
    , m_color(color)
{}

const Vec3& Sphere::center() const noexcept {
    return m_center;
}

math::Real Sphere::radius() const noexcept {
    return m_radius;
}

const Vec3& Sphere::color() const noexcept {
    return m_color;
}

std::optional<HitInfo> Sphere::intersect(const Ray& ray) const noexcept {
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

    const math::Real theta = std::acos(std::clamp(outward.y, math::Real(-1), math::Real(1)));
    const math::Real phi = std::atan2(outward.z, outward.x);
    info.uv.u = (phi + math::TWO_PI) / math::TWO_PI;
    info.uv.v = theta / math::PI;

    return info;
}

} // namespace rayscene
