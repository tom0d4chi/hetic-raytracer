#include "Cube.hpp"
#include <algorithm>
#include <cmath>

namespace rayscene {
using math::HitInfo;
using math::Ray;
using math::Vec3;

Cube::Cube(const Vec3& min, const Vec3& max, std::shared_ptr<Material> mat, const Vec3& color) noexcept
    : m_min(min)
    , m_max(max)
    , m_material(std::move(mat))
    , m_color(color)
{}

const Vec3& Cube::min() const noexcept { return m_min; }
const Vec3& Cube::max() const noexcept { return m_max; }
const Vec3& Cube::color() const noexcept { return m_color; }

std::optional<HitInfo> Cube::intersect(const Ray& ray) const noexcept {
    // Slab method for axis-aligned box
    const Vec3 invD = Vec3(1.0 / ray.direction().x, 1.0 / ray.direction().y, 1.0 / ray.direction().z);
    const Vec3 t0s = (m_min - ray.origin()).cwiseProduct(invD);
    const Vec3 t1s = (m_max - ray.origin()).cwiseProduct(invD);
    Vec3 tmin = Vec3(std::min(t0s.x, t1s.x), std::min(t0s.y, t1s.y), std::min(t0s.z, t1s.z));
    Vec3 tmax = Vec3(std::max(t0s.x, t1s.x), std::max(t0s.y, t1s.y), std::max(t0s.z, t1s.z));
    double t_enter = std::max({tmin.x, tmin.y, tmin.z});
    double t_exit = std::min({tmax.x, tmax.y, tmax.z});
    if (t_enter > t_exit || t_exit < math::RAY_MIN_T) return std::nullopt;
    double t = t_enter >= math::RAY_MIN_T ? t_enter : t_exit;
    if (t < math::RAY_MIN_T) return std::nullopt;
    HitInfo info;
    info.t = t;
    info.point = ray.at(t);
    // Compute normal
    Vec3 p = info.point;
    Vec3 n(0,0,0);
    for (int i = 0; i < 3; ++i) {
        if (std::abs(p[i] - m_min[i]) < 1e-4) n[i] = -1;
        else if (std::abs(p[i] - m_max[i]) < 1e-4) n[i] = 1;
    }
    info.setFaceNormal(ray, n);
    // Simple UV mapping (project to largest axis)
    info.uv.u = (p.x - m_min.x) / (m_max.x - m_min.x);
    info.uv.v = (p.y - m_min.y) / (m_max.y - m_min.y);
    return info;
}

} // namespace rayscene
