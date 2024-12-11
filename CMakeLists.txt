cmake_minimum_required(VERSION 3.10)
project(2DPhysicsEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIRS})

include_directories(header)
file(GLOB_RECURSE SOURCES "source/*.cpp")
file(GLOB_RECURSE HEADERS "header/*.h")

add_executable(2DPhysicsEngine ${SOURCES} ${HEADERS})

target_link_libraries(2DPhysicsEngine ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})
