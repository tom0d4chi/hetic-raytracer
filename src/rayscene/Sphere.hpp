#pragma once

#include "../raymath/Vec3.hpp"
#include "../raymath/Ray.hpp"
#include "../raymath/Intersection.hpp"

#include <memory>
#include <optional>

namespace rayscene {

struct Material; // placeholder for future extensions

class Sphere {
public:
    Sphere(const math::Vec3& center, math::Real radius, std::shared_ptr<Material> mat = nullptr) noexcept;
    Sphere(const math::Vec3& center, math::Real radius, std::shared_ptr<Material> mat, const math::Vec3& color) noexcept;

    const math::Vec3& center() const noexcept;
    math::Real radius() const noexcept;
    const math::Vec3& color() const noexcept;

    std::optional<math::HitInfo> intersect(const math::Ray& ray) const noexcept;

private:
    math::Vec3 m_center;
    math::Real m_radius;
    math::Real m_radius2;
    std::shared_ptr<Material> m_material;
    math::Vec3 m_color;
};

} // namespace rayscene
