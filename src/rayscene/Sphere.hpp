#pragma once

#include "../raymath/Vec3.hpp"
#include "../raymath/Ray.hpp"
#include "../raymath/Intersection.hpp"
#include "Light.hpp"

#include <memory>
#include <optional>
#include <vector>

class Image;

namespace rayscene {

struct Material; // placeholder for future extensions

class Sphere {
public:
    Sphere(const math::Vec3& center, math::Real radius, std::shared_ptr<Material> mat = nullptr, const math::Real reflectFactor = 0.0, int specularPower = 0) noexcept;
    Sphere(const math::Vec3& center, math::Real radius, std::shared_ptr<Material> mat, const math::Vec3& color, const math::Real reflectFactor, int specularPower = 0) noexcept;

    const math::Vec3& center() const noexcept;
    math::Real radius() const noexcept;
    const math::Vec3& color() const noexcept;

    std::optional<math::HitInfo> intersect(const math::Ray& ray) const noexcept;
    static void DrawSphere(Image& image,
                           const math::Vec3& camOrigin,
                           int width,
                           int height,
                           const std::vector<Sphere>& spheres,
                           Light light);

    math::Real reflectFactor() const noexcept;

    int specularPower() const noexcept;

private:
    math::Vec3 m_center;
    math::Real m_radius;
    math::Real m_radius2;
    std::shared_ptr<Material> m_material;
    math::Vec3 m_color;
    math::Real m_reflectFactor;
    int m_specularPower;
};

} // namespace rayscene
