#pragma once

#include "../raymath/Vec3.hpp"
#include "../raymath/Ray.hpp"
#include "../raymath/Intersection.hpp"
#include <memory>
#include <optional>

namespace rayscene {

struct Material; // placeholder for future extensions

class Cube {
public:
    Cube(const math::Vec3& min, const math::Vec3& max, std::shared_ptr<Material> mat = nullptr, const math::Vec3& color = math::Vec3(1, 1, 1)) noexcept;

    const math::Vec3& min() const noexcept;
    const math::Vec3& max() const noexcept;
    const math::Vec3& color() const noexcept;

    std::optional<math::HitInfo> intersect(const math::Ray& ray) const noexcept;

private:
    math::Vec3 m_min;
    math::Vec3 m_max;
    std::shared_ptr<Material> m_material;
    math::Vec3 m_color;
};

} // namespace rayscene
