#include "Triangle.hpp"
#include "Sphere.hpp"
#include "Light.hpp"
#include "../rayimage/Image.hpp"
#include "../raymath/Color.hpp"
#include "../rayshader/DiffuseShader.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

namespace rayscene {

using math::HitInfo;
using math::Ray;
using math::Vec3;
using math::Real;
using ::Color;


Triangle::Triangle(const Vec3& a, const Vec3& b, const Vec3& c, std::shared_ptr<Material> mat, const Real reflectFactor, int specularPower) noexcept
    : m_a(a), m_b(b), m_c(c), m_material(std::move(mat)), m_color(Vec3(0, 1, 0)), m_reflectFactor(reflectFactor), m_specularPower(specularPower) {}

Triangle::Triangle(const Vec3& a, const Vec3& b, const Vec3& c, std::shared_ptr<Material> mat, const Vec3& color, const Real reflectFactor, int specularPower) noexcept
    : m_a(a), m_b(b), m_c(c), m_material(std::move(mat)), m_color(color), m_reflectFactor(reflectFactor), m_specularPower(specularPower) {}


std::optional<HitInfo> Triangle::intersect(const Ray& ray) const noexcept {
    const Vec3 e1 = m_b - m_a;
    const Vec3 e2 = m_c - m_a;
    const Vec3 pvec = ray.direction().cross(e2);
    const Real det  = e1.dot(pvec);

    constexpr Real EPS = Real(1e-8);
    if (det > -EPS && det < EPS) return std::nullopt; 

    const Real invDet = Real(1) / det;
    const Vec3 tvec = ray.origin() - m_a;
    const Real u = tvec.dot(pvec) * invDet;
    if (u < Real(0) || u > Real(1)) return std::nullopt;

    const Vec3 qvec = tvec.cross(e1);
    const Real v = ray.direction().dot(qvec) * invDet;
    if (v < Real(0) || (u + v) > Real(1)) return std::nullopt;

    const Real t = e2.dot(qvec) * invDet;
    if (t < math::RAY_MIN_T) return std::nullopt;

    HitInfo info;
    info.t = t;
    info.point = ray.at(t);
    const Vec3 outward = e1.cross(e2).normalized();
    info.setFaceNormal(ray, outward);
    info.uv.u = u;
    info.uv.v = v;

    return info;
}


void Triangle::DrawTriangles(Image& image,
                             const Vec3& camOrigin,
                             int width,
                             int height,
                             const std::vector<Triangle>& triangles,
                             const std::vector<Sphere>& spheres,
                             const Light& light , 
                             std::vector<Real>& zbuf)
{
    if (width <= 0 || height <= 0) return;

    const Real aspect = Real(width) / Real(height);
    const Real focal_length = Real(4.0);

    auto clampColor = [](Real value) -> float {
        if (value < 0) return 0.0f;
        if (value > 1) return 1.0f;
        return static_cast<float>(value);
    };

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            
            const Real screenX = ((Real(2.0) * x / width) - Real(1.0)) * aspect;
            const Real screenY = (Real(2.0) * y / height) - Real(1.0);

           
            Vec3 rayDirection(screenX, -screenY, focal_length); 
            rayDirection = rayDirection.normalized();
            const Ray ray(camOrigin, rayDirection);

            Real closest_t = std::numeric_limits<Real>::infinity();
            Vec3 baseColor(0.0, 0.0, 0.0);
            bool hitTri = false;
            std::optional<HitInfo> closestHit;
            int specularPowerToUse = 0;
            Real reflectFactorToUse = Real(0);

         
            for (const auto& tri : triangles) {
                const auto hit = tri.intersect(ray);
                if (hit && hit->t < closest_t) {
                    closest_t = hit->t;
                    baseColor = tri.color();
                    closestHit = hit;
                    hitTri = true;
                    specularPowerToUse = tri.specularPower();
                    reflectFactorToUse = tri.reflectFactor();
                }
            }

            const int idx = y * width + x; 

            if (hitTri && closestHit && closest_t < zbuf[idx]) {
                
                DiffuseShader shader;
                float intensity = shader.Shade(*closestHit, light, spheres, camOrigin, specularPowerToUse);
                Vec3 shaded = baseColor * intensity;

                // Réflexion
                Vec3 reflectDir = ray.direction().reflect(closestHit->normal);
                Ray reflectRay(closestHit->point, reflectDir);
                Real reflect_closest_t = std::numeric_limits<Real>::infinity();
                Vec3 reflectColor(0,0,0);
                Real reflectFactor = reflectFactorToUse;

              
                for (const auto& tri : triangles) {
                    const auto hit = tri.intersect(reflectRay);
                    if (hit && hit->t < reflect_closest_t) {
                        reflect_closest_t = hit->t;
                        reflectColor = tri.color();
                        reflectFactor = std::max(reflectFactor, tri.reflectFactor());
                    }
                }
              
                for (const auto& sph : spheres) {
                    const auto hit = sph.intersect(reflectRay);
                    if (hit && hit->t < reflect_closest_t) {
                        reflect_closest_t = hit->t;
                        reflectColor = sph.color();
                        reflectFactor = std::max(reflectFactor, sph.reflectFactor());
                    }
                }

                if (reflect_closest_t < std::numeric_limits<Real>::infinity()) {
                    shaded = shaded + (reflectColor * reflectFactor * intensity);
                }

                const Color pixelColor(
                    clampColor(shaded.x),
                    clampColor(shaded.y),
                    clampColor(shaded.z)
                );
                
                image.SetPixel(static_cast<unsigned>(x), static_cast<unsigned>(y), pixelColor);
                zbuf[idx] = closest_t;
            }
        }
    }
}

} // namespace rayscene