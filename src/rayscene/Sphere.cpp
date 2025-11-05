#include "Sphere.hpp"
#include "../rayimage/Image.hpp"
#include "../raymath/Color.hpp"
#include "Light.hpp"
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

Sphere::Sphere(const Vec3& center, math::Real radius, std::shared_ptr<Material> mat, const math::Real reflectFactor, int specularPower) noexcept
    : m_center(center)
    , m_radius(radius)
    , m_radius2(radius * radius)
    , m_material(std::move(mat))
    , m_color(Vec3(0, 1, 0)) // default green
    , m_reflectFactor(reflectFactor)
    , m_specularPower(specularPower)
{}

Sphere::Sphere(const Vec3& center, math::Real radius, std::shared_ptr<Material> mat, const Vec3& color, const math::Real reflectFactor, int specularPower) noexcept
    : m_center(center)
    , m_radius(radius)
    , m_radius2(radius * radius)
    , m_material(std::move(mat))
    , m_color(color)
    , m_reflectFactor(reflectFactor)
    , m_specularPower(specularPower)
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

math::Real Sphere::reflectFactor() const noexcept {
    return m_reflectFactor;
}

int Sphere::specularPower() const noexcept {
    return m_specularPower;
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

void Sphere::DrawSphere(Image& image,
                        const Vec3& camOrigin,
                        int width,
                        int height,
                        const std::vector<Sphere>& spheres,
                        Light light) {
    if (width <= 0 || height <= 0) {
        return;
    }

    const Real aspect = Real(width) / Real(height);
    const Real focal_length = 4.0;

    auto clampColor = [](Real value) -> float {
        if (value < 0) {
            return 0.0f;
        }
        if (value > 1) {
            return 1.0f;
        }
        return static_cast<float>(value);
    };

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Real u = (Real(x) + Real(0.5)) / Real(width);
            const Real v = (Real(height - 1 - y) + Real(0.5)) / Real(height);

            // Calcul de la direction du rayon avec focal_length
            const Real screenX = ((Real(2.0) * x / width) - Real(1.0)) * aspect;
            const Real screenY = (Real(2.0) * y / height) - Real(1.0);

            Vec3 rayDirection(screenX, screenY, focal_length);
            rayDirection = rayDirection.normalized();
            const Ray ray(camOrigin, rayDirection);

            Real closest_t = std::numeric_limits<Real>::infinity();
            Vec3 color(0.0, 0.0, 0.0);
            bool hitSphere = false;
            std::optional<HitInfo> closestHit;
            int specularPowerToUse = 0;

            for (const auto& sphere : spheres) {
                const auto hit = sphere.intersect(ray);
                if (hit && hit->t < closest_t) {
                    closest_t = hit->t;
                    color = sphere.color();
                    closestHit = hit;
                    hitSphere = true;
                    specularPowerToUse = sphere.specularPower();
                }
            }

            if (hitSphere && closestHit) {
                DiffuseShader shader;
                float intensity = shader.Shade(*closestHit, light, spheres, camOrigin, specularPowerToUse);
                Vec3 baseColor = color * intensity;

                Vec3 reflectDir = ray.direction().reflect(closestHit->normal);
                Ray reflectRay(closestHit->point, reflectDir);
                Real reflect_closest_t = std::numeric_limits<Real>::infinity();

                for (const auto& sphere : spheres) {
                    const auto hit = sphere.intersect(reflectRay);
                    if (hit && hit->t < reflect_closest_t) {
                        reflect_closest_t = hit->t;
                        baseColor = baseColor + (sphere.color() * sphere.reflectFactor() * intensity);
                    }
                }

                const Color pixelColor(
                    clampColor(baseColor.x),
                    clampColor(baseColor.y),
                    clampColor(baseColor.z)
                );

                image.SetPixel(static_cast<unsigned>(x), static_cast<unsigned>(y), pixelColor);
            }
        }
    }
}

} // namespace rayscene
