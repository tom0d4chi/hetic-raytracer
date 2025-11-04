#pragma once

#include "../raymath/Color.hpp"
#include "../raymath/Vec3.hpp"

#include <optional>
#include <string>
#include <vector>

namespace rayscene {

struct CameraConfig {
    math::Vec3 origin;
    math::Vec3 lookAt;
    math::Vec3 up;
    math::Real verticalFov;
    math::Real focusDistance;
};

struct PlaneConfig {
    Color primaryColor;
    Color secondaryColor;
    float posY;
    float tileSize;
};

struct SphereConfig {
    math::Vec3 center;
    math::Real radius;
    math::Vec3 color;
};

struct SceneConfig {
    int width;
    int height;
    Color background;
    std::string outputPath;
    std::string timerLabel;
    CameraConfig camera;
    std::optional<PlaneConfig> plane;
    std::vector<SphereConfig> spheres;
};

SceneConfig LoadSceneFromJson(const std::string& filepath);

} // namespace rayscene
