add_library(raymath 
  ${CMAKE_CURRENT_SOURCE_DIR}/Color.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Vec3.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Ray.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Intersection.cpp
)

target_include_directories(raymath PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
