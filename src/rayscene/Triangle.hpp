#pragma once
#include "../raymath/Vec3.hpp"
#include "../raymath/Ray.hpp"
#include "../raymath/Intersection.hpp"
#include "../rayimage/Image.hpp"
#include "Light.hpp"

#include <memory>
#include <optional>
#include <vector>

namespace rayscene {

class Material;
class Sphere;

class Triangle {
public:
    Triangle(const math::Vec3& a,
             const math::Vec3& b,
             const math::Vec3& c,
             std::shared_ptr<Material> mat,
             const math::Real reflectFactor = math::Real(0),
             int specularPower = 0) noexcept;

    Triangle(const math::Vec3& a,
             const math::Vec3& b,
             const math::Vec3& c,
             std::shared_ptr<Material> mat,
             const math::Vec3& color,
             const math::Real reflectFactor = math::Real(0),
             int specularPower = 0) noexcept;

    std::optional<math::HitInfo> intersect(const math::Ray& ray) const noexcept;

    const math::Vec3& color() const noexcept { return m_color; }
    math::Real        reflectFactor() const noexcept { return m_reflectFactor; }
    int               specularPower() const noexcept { return m_specularPower; }

    
    static void DrawTriangles(Image& image,
                              const math::Vec3& camOrigin,
                              int width,
                              int height,
                              const std::vector<Triangle>& triangles,
                              const std::vector<Sphere>& spheres,
                              const Light& light ,
                            std::vector<math::Real>& zbuf);

private:
    math::Vec3 m_a, m_b, m_c;
    std::shared_ptr<Material> m_material;
    math::Vec3 m_color{0,1,0};
    math::Real m_reflectFactor{0};
    int        m_specularPower{0};
};

} // namespace rayscene