#include "Triangle.hpp"

#include <algorithm> // std::abs
#include <cmath>
#include <utility>   // std::move

namespace render {

Triangle::Triangle(const Vec3& a, const Vec3& b, const Vec3& c,
                   std::shared_ptr<Material> mat,
                   bool backfaceCull) noexcept
    : m_v0(a)
    , m_v1(b)
    , m_v2(c)
    , m_material(std::move(mat))
    , m_backfaceCull(backfaceCull)
{}

Triangle::Triangle(const Vec3& a, const Vec3& b, const Vec3& c,
                   std::shared_ptr<Material> mat, const Vec3& color,
                   bool backfaceCull) noexcept
    : m_v0(a)
    , m_v1(b)
    , m_v2(c)
    , m_material(std::move(mat))
    , m_color(color)
    , m_backfaceCull(backfaceCull)
{}

std::optional<HitInfo> Triangle::intersect(const Ray& ray) const noexcept {

    const Vec3 e1 = m_v1 - m_v0;
    const Vec3 e2 = m_v2 - m_v0;

    const Vec3 pvec = ray.direction().cross(e2);
    const math::Real det = e1.dot(pvec);

    constexpr math::Real EPS = math::Real(1e-8);

    if (m_backfaceCull) {
        if (det < EPS) return std::nullopt;
    } else {
        if (std::abs(det) < EPS) return std::nullopt;
    }

    const math::Real invDet = math::Real(1) / det;

    const Vec3 tvec = ray.origin() - m_v0;
    const math::Real u = tvec.dot(pvec) * invDet;
    if (u < math::Real(0) || u > math::Real(1)) return std::nullopt;

    const Vec3 qvec = tvec.cross(e1);
    const math::Real v = ray.direction().dot(qvec) * invDet;
    if (v < math::Real(0) || (u + v) > math::Real(1)) return std::nullopt;

    const math::Real t = e2.dot(qvec) * invDet;

 
    constexpr math::Real T_MIN = math::RAY_MIN_T;

    HitInfo info;
    info.t = t;
    info.point = ray.at(t);

    
    const Vec3 ng = e1.cross(e2).normalized();
    info.setFaceNormal(ray, ng);

   
    info.uv.u = u;
    info.uv.v = v;

    return info;
}

} // namespace render
