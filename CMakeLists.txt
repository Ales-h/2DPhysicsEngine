cmake_minimum_required(VERSION 3.10)
project(2DPhysicsEngine)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(2DPhysicsEngine ${SDL2_LIBRARIES})

include_directories(header)
file(GLOB_RECURSE SOURCES "source/*.cpp")
file(GLOB_RECURSE HEADERS "header/*.h")

add_executable(2DPhysicsEngine ${SOURCES} ${HEADERS})

target_link_libraries(2DPhysicsEngine ${SDL2_LIBRARIES})
