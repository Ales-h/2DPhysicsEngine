cmake_minimum_required(VERSION 3.19.2)
project(2DPhysicsEngine)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SDL3_IMAGE_STATIC ON)
set(SDL3_USE_STATIC_LIBS ON)
set(SDLIMAGE_CODEC_AVIF OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_VENDORED  OFF  CACHE BOOL "" FORCE)

set(SDL_SHARED  OFF CACHE BOOL "Disable building SDL3 shared" FORCE)
set(SDL_STATIC   ON CACHE BOOL "Enable building SDL3 static" FORCE)


file(GLOB_RECURSE MY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

add_executable(2DPhysicsEngine ${MY_SOURCES})

target_include_directories(2DPhysicsEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/header)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_subdirectory(thirdparty/SDL3)
add_subdirectory(thirdparty/SDL_image)
add_subdirectory(thirdparty/imgui)
include_directories(thirdparty/json)

target_link_libraries(imgui PUBLIC SDL3::SDL3-static)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3-static SDL3_image::SDL3_image-static imgui)


if(EXISTS "${PROJECT_SOURCE_DIR}/scenes")
  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/scenes"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/scenes"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/scenes"
    COMMENT "Copying resources → $<TARGET_FILE_DIR:${PROJECT_NAME}>/scenes")
endif()

if(EXISTS "${PROJECT_SOURCE_DIR}/assets")
  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMENT "Copying resources → $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets")
endif()