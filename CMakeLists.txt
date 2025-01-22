cmake_minimum_required(VERSION 3.10)
project(2DPhysicsEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

add_library(
  imgui STATIC
  thirdparty/imgui/imgui.cpp
  thirdparty/imgui/imgui_demo.cpp
  thirdparty/imgui/imgui_draw.cpp
  thirdparty/imgui/imgui_widgets.cpp
  thirdparty/imgui/imgui_tables.cpp
  thirdparty/imgui/backends/imgui_impl_sdl2.cpp
  thirdparty/imgui/backends/imgui_impl_sdlrenderer2.cpp)

target_include_directories(imgui PUBLIC thirdparty/imgui
                                        thirdparty/imgui/backends)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(header)
include_directories(thirdparty/json)

file(GLOB_RECURSE SOURCES "source/*.cpp")
file(GLOB_RECURSE HEADERS "header/*.h")

add_executable(2DPhysicsEngine ${SOURCES} ${HEADERS})
target_link_libraries(2DPhysicsEngine ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} SDL2_image imgui)
