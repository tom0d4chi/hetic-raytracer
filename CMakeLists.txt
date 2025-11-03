cmake_minimum_required(VERSION 3.10.0)
project(hetic-raytracer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(hetic-raytracer
    main.cpp
    src/raytimer/Timer.cpp    
)

target_include_directories(hetic-raytracer PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/src/raymath"
    "${PROJECT_SOURCE_DIR}/src/rayimage"
    "${PROJECT_SOURCE_DIR}/src/raytimer"
)

add_subdirectory(./src/raymath)
add_subdirectory(./src/rayimage)
add_subdirectory(./src/math)
add_subdirectory(./src/lodepng)

find_package(Threads REQUIRED)

target_link_libraries(hetic-raytracer PUBLIC
    raymath
    rayimage
    lodepng
    Threads::Threads         
   
)

add_executable(render_sphere tools/render_sphere.cpp)
target_include_directories(render_sphere PRIVATE
    "${PROJECT_SOURCE_DIR}/src/render/include"
    "${PROJECT_SOURCE_DIR}/src/lodepng"
)
target_link_libraries(render_sphere PRIVATE
    math
    lodepng
)
