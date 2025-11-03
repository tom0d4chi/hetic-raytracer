add_library(rayscene
  ${CMAKE_CURRENT_SOURCE_DIR}/Plane.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Sphere.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Cube.cpp
)

target_link_libraries(rayscene PUBLIC raymath)

target_include_directories(rayscene PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
