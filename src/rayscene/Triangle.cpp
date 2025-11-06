#include "Triangle.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <utility>

namespace render {



Triangle::Triangle(const Vec3& a, const Vec3& b, const Vec3& c,
                   std::shared_ptr<Material> mat,
                   bool backfaceCull) noexcept
: m_v0(a), m_v1(b), m_v2(c),
  m_material(std::move(mat)),
  m_color(Vec3(1,1,1)),
  m_backfaceCull(backfaceCull)
{}

Triangle::Triangle(const Vec3& a, const Vec3& b, const Vec3& c,
                   std::shared_ptr<Material> mat, const Vec3& color,
                   bool backfaceCull) noexcept
: m_v0(a), m_v1(b), m_v2(c),
  m_material(std::move(mat)),
  m_color(color),
  m_backfaceCull(backfaceCull)
{}



std::optional<math::HitInfo> Triangle::intersect(const Ray& ray) const noexcept {
    using math::Real;

    const Vec3 edge1 = m_v1 - m_v0;
    const Vec3 edge2 = m_v2 - m_v0;

    const Vec3 pvec = ray.direction().cross(edge2);
    const Real det  = edge1.dot(pvec);

    if (m_backfaceCull) {
        if (det <= Real(0)) return std::nullopt;               // face dos caméra
    } else {
        if (std::abs(det) < Real(1e-8)) return std::nullopt;   // quasi parallèle
    }

    const Real invDet = Real(1) / det;

    const Vec3 tvec = ray.origin() - m_v0;
    const Real u    = tvec.dot(pvec) * invDet;
    if (u < Real(0) || u > Real(1)) return std::nullopt;

    const Vec3 qvec = tvec.cross(edge1);
    const Real v    = ray.direction().dot(qvec) * invDet;
    if (v < Real(0) || u + v > Real(1)) return std::nullopt;

    const Real t = edge2.dot(qvec) * invDet;
    if (t <= Real(0)) return std::nullopt; // derrière la caméra ou à 0

    math::HitInfo hit{};
    hit.t     = t;
    hit.point = ray.at(t);

    // normale géométrique, orientée face caméra
    Vec3 n = edge1.cross(edge2).normalized();
    if (!m_backfaceCull && n.dot(ray.direction()) > Real(0)) n = -n;
    hit.normal = n;

    
    return hit;
}

// --------------------- Rendu simple ---------------------

void Triangle::DrawTriangles(Image& image,
                             const math::Vec3& camOrigin,
                             int width,
                             int height,
                             const std::vector<Triangle>& triangles,
                             Light light,
                             int defaultSpecularPower)
{
    using math::Vec3; using math::Real; using math::Ray; using math::HitInfo;

    if (width <= 0 || height <= 0) return;

    const Real aspect = Real(width) / Real(height);
    const Real focal_length = Real(4.0);

    auto clamp01 = [](Real v) -> float {
        if (v < Real(0)) return 0.0f;
        if (v > Real(1)) return 1.0f;
        return static_cast<float>(v);
    };
    auto normalize = [](const Vec3& v){ return v.normalized(); };

    const Vec3 lightPos = light.getPosition();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            const Real screenX = ((Real(2.0) * x / width) - Real(1.0)) * aspect;
            const Real screenY = (Real(2.0) * y / height) - Real(1.0);

            Vec3 rayDir(screenX, screenY, focal_length); // caméra vers +Z
            rayDir = rayDir.normalized();
            Ray ray(camOrigin, rayDir);

            Real closestT = std::numeric_limits<Real>::infinity();
            const Triangle* hitTri = nullptr;
            std::optional<HitInfo> closestHit;

            for (const auto& tri : triangles) {
                auto hit = tri.intersect(ray);
                if (hit && hit->t < closestT) {
                    closestT   = hit->t;
                    closestHit = hit;
                    hitTri     = &tri;
                }
            }
            if (!hitTri || !closestHit) continue;

            Vec3 N = closestHit->normal;
            Vec3 L = normalize(lightPos - closestHit->point);
            Vec3 V = normalize(camOrigin - closestHit->point);
            Vec3 H = normalize(L + V);

            Real ndotl = std::max(Real(0), N.dot(L));
            Real ndoth = std::max(Real(0), N.dot(H));

            Vec3 base = hitTri->color();
            Vec3 ambient = base * Real(0.05);
            Vec3 diffuse = base * ndotl;
            Real spec = std::pow(ndoth, Real(defaultSpecularPower)) * Real(0.5);

            Vec3 rgb = ambient + diffuse + Vec3(spec, spec, spec);

            image.SetPixel(
                static_cast<unsigned>(x), static_cast<unsigned>(y),
                Color(clamp01(rgb.x), clamp01(rgb.y), clamp01(rgb.z))
            );
        }
    }
}

} // namespace render
