#include "SceneLoader.hpp"

#include "../nlohmann/json.hpp"

#include <fstream>
#include <stdexcept>
#include <string>

namespace rayscene {

namespace {

math::Vec3 readVec3(const nlohmann::json& node, const std::string& context) {
    if (!node.is_array() || node.size() != 3) {
        throw std::runtime_error("Expected array of 3 numbers for " + context);
    }
    return math::Vec3(
        static_cast<math::Real>(node[0].get<double>()),
        static_cast<math::Real>(node[1].get<double>()),
        static_cast<math::Real>(node[2].get<double>())
    );
}

Color readColor(const nlohmann::json& node, const std::string& context) {
    const math::Vec3 vec = readVec3(node, context);
    return Color(
        static_cast<float>(vec.x),
        static_cast<float>(vec.y),
        static_cast<float>(vec.z)
    );
}

} // namespace

SceneConfig LoadSceneFromJson(const std::string& filepath) {
    std::ifstream input(filepath);
    if (!input) {
        throw std::runtime_error("Unable to open scene file: " + filepath);
    }

    nlohmann::json root = nlohmann::json::parse(input);

    SceneConfig config{};

    const auto& image = root.at("image");
    config.width = image.at("width").get<int>();
    config.height = image.at("height").get<int>();
    if (image.contains("background")) {
        config.background = readColor(image.at("background"), "image.background");
    } else {
        config.background = Color(0.0f, 0.0f, 0.0f);
    }

    config.outputPath = root.value("output", std::string("scene.png"));
    config.timerLabel = root.value("timer_label", std::string("Scene render"));
    config.echantillonsNumber = root.value("echantillonsNumber", 1);

    const auto& camera = root.at("camera");
    config.camera.origin = readVec3(camera.at("origin"), "camera.origin");
    config.camera.lookAt = camera.contains("look_at")
        ? readVec3(camera.at("look_at"), "camera.look_at")
        : config.camera.origin + math::Vec3(0.0, 0.0, -1.0);
    config.camera.up = camera.contains("up")
        ? readVec3(camera.at("up"), "camera.up")
        : math::Vec3(0.0, 1.0, 0.0);
    config.camera.verticalFov = camera.value("vertical_fov", math::Real(60.0));
    config.camera.focusDistance = camera.contains("focus_distance")
        ? static_cast<math::Real>(camera.at("focus_distance").get<double>())
        : (config.camera.origin - config.camera.lookAt).length();
    if (config.camera.focusDistance <= math::EPSILON) {
        config.camera.focusDistance = 1.0;
    }

    if (root.contains("plane")) {
        const auto& plane = root.at("plane");
        const auto& colors = plane.at("colors");
        if (!colors.is_array() || colors.size() != 2) {
            throw std::runtime_error("plane.colors must contain exactly two entries");
        }

        PlaneConfig planeConfig{};
        planeConfig.primaryColor = readColor(colors[0], "plane.colors[0]");
        planeConfig.secondaryColor = readColor(colors[1], "plane.colors[1]");
        planeConfig.posY = plane.value("posY", 0.0f);
        planeConfig.tileSize = plane.value("tileSize", 1.0f);
        config.plane = planeConfig;
    } else {
        config.plane.reset();
    }

    if (root.contains("light")) {
        const auto& light = root.at("light");
        LightConfig lightConfig{};
        lightConfig.position = readVec3(light.at("position"), "light.position");
        config.light = lightConfig;
    } else {
        config.light.reset();
    }

    config.spheres.clear();
    if (root.contains("spheres")) {
        const auto& spheres = root.at("spheres");
        if (!spheres.is_array()) {
            throw std::runtime_error("spheres must be an array");
        }
        config.spheres.reserve(spheres.size());
        for (const auto& sphereJson : spheres) {
            SphereConfig sphere{};
            sphere.center = readVec3(sphereJson.at("center"), "sphere.center");
            sphere.radius = static_cast<math::Real>(sphereJson.at("radius").get<double>());
            sphere.color = readVec3(sphereJson.at("color"), "sphere.color");
            sphere.reflectFactor = sphereJson.value("reflectFactor", math::Real(0.5));
            sphere.specularPower = sphereJson.value("specularPower", 200);
            if (sphere.radius <= 0) {
                throw std::runtime_error("sphere.radius must be positive");
            }
            config.spheres.push_back(sphere);
        }
    }

    return config;
}

} // namespace rayscene
