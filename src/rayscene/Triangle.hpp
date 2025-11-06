#pragma once

#include "../raymath/Vec3.hpp"
#include "../raymath/Ray.hpp"
#include "../raymath/Intersection.hpp"
#include "../rayimage/Image.hpp"
#include "../raymath/Color.hpp"
#include "Light.hpp"

#include <optional>
#include <memory>
#include <vector>

namespace render {

struct Material;

using math::HitInfo;
using math::Ray;
using math::Vec3;

class Triangle {
public:
    Triangle(const Vec3& a, const Vec3& b, const Vec3& c,
             std::shared_ptr<Material> mat = nullptr,
             bool backfaceCull = false) noexcept;

    Triangle(const Vec3& a, const Vec3& b, const Vec3& c,
             std::shared_ptr<Material> mat, const Vec3& color,
             bool backfaceCull = false) noexcept;

    const Vec3& v0() const noexcept { return m_v0; }
    const Vec3& v1() const noexcept { return m_v1; }
    const Vec3& v2() const noexcept { return m_v2; }
    const Vec3& color() const noexcept { return m_color; }

    std::optional<HitInfo> intersect(const Ray& ray) const noexcept;

    static void DrawTriangles(Image& image,
                              const math::Vec3& camOrigin,
                              int width,
                              int height,
                              const std::vector<Triangle>& triangles,
                              Light light,
                              int defaultSpecularPower = 64);

private:
    Vec3 m_v0, m_v1, m_v2;
    std::shared_ptr<Material> m_material;
    Vec3 m_color{Vec3(1, 1, 1)};
    bool m_backfaceCull{false};
};

} // namespace render
