cmake_minimum_required(VERSION 3.10)

project(RayTracingInAWeekend)

set(CMAKE_CXX_STANDARD 11)

add_executable(RayTracingInAWeekend
    sphere.cpp
)

# If you're using any additional libraries or dependencies, add them here
# For example:
# find_package(OpenMP REQUIRED)
# target_link_libraries(RayTracingInAWeekend PUBLIC OpenMP::OpenMP_CXX)