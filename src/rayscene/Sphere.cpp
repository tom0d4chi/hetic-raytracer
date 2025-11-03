#include "Sphere.hpp"
#include "../rayimage/Image.hpp"
#include "../raymath/Color.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

namespace rayscene {

using math::HitInfo;
using math::Ray;
using math::Vec3;
using math::Real;
using ::Color;

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

void Sphere::DrawSphere(Image& image,
                        const Vec3& camOrigin,
                        int width,
                        int height,
                        const std::vector<Sphere>& spheres) {
    if (width <= 0 || height <= 0) {
        return;
    }

    const Real aspect = Real(width) / Real(height);
    const Real viewport_height = 2.0;
    const Real viewport_width = aspect * viewport_height;
    const Real focal_length = 3.0;

    const Vec3 horizontal(viewport_width, 0, 0);
    const Vec3 vertical(0, viewport_height, 0);
    const Vec3 lower_left = camOrigin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);

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

            const Vec3 dir = (lower_left + horizontal * u + vertical * v) - camOrigin;
            const Vec3 dirn = dir.normalized();
            const Ray ray(camOrigin, dirn);

            Vec3 color(0.0, 0.0, 0.0);
            Real closest_t = std::numeric_limits<Real>::infinity();
            for (const auto& sphere : spheres) {
                const auto hit = sphere.intersect(ray);
                if (hit && hit->t < closest_t) {
                    closest_t = hit->t;
                    color = sphere.color();
                }
            }

            const Color pixelColor(
                clampColor(color.x),
                clampColor(color.y),
                clampColor(color.z)
            );

            image.SetPixel(static_cast<unsigned>(x), static_cast<unsigned>(y), pixelColor);
        }
    }
}

} // namespace rayscene
