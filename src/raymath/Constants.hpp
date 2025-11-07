#pragma once

#include <cmath>

namespace math {

// Type configuration
using Real = double;  // Changer en float si besoin de plus de performance

// Constants numériques
constexpr Real PI = 3.14159265358979323846;
constexpr Real INV_PI = 0.31830988618379067154;
constexpr Real TWO_PI = 6.28318530717958647692;

// Epsilons et seuils numériques
constexpr Real EPSILON = 1e-6;           // Seuil général pour comparaisons
constexpr Real RAY_MIN_T = 1e-3;        // Distance minimum pour intersections
constexpr Real NORMAL_EPSILON = 1e-7;    // Seuil pour normalisation
constexpr Real SHADOW_BIAS = 1e-4;       // Bias pour shadow rays

// Utils de conversion
constexpr Real DEG_TO_RAD = PI / 180.0;
constexpr Real RAD_TO_DEG = 180.0 / PI;

inline Real randomReal() {
    return static_cast<Real>(rand()) / static_cast<Real>(RAND_MAX);
}

inline Real randomReal(Real min, Real max) {
    return min + (max - min) * randomReal();
}

} // namespace math
