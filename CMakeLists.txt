cmake_minimum_required(VERSION 3.10.0)
project(hetic-raytracer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(hetic-raytracer main.cpp)

target_include_directories(hetic-raytracer PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/src/raymath"
                           "${PROJECT_SOURCE_DIR}/src/rayimage"
                           "${PROJECT_SOURCE_DIR}/src/rayscene"
                           "${PROJECT_SOURCE_DIR}/src/raytimer"
                           "${PROJECT_SOURCE_DIR}/src/rayshader"
                           )

add_subdirectory(./src/raymath)
add_subdirectory(./src/rayimage)
add_subdirectory(./src/rayscene)
add_subdirectory(./src/raytimer)
add_subdirectory(./src/rayshader)
add_subdirectory(./src/lodepng)
add_subdirectory(./src/nlohmann)


find_package(Threads REQUIRED)

target_link_libraries(hetic-raytracer PUBLIC 
                      raymath
                      rayimage
                      rayscene
                      raytimer
                      rayshader
                      lodepng
                      nlohmann
                      Threads::Threads
                      )
