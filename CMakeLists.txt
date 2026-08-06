cmake_minimum_required(VERSION 4.0)
project(VulkanEngine
  VERSION 0.1.0
  DESCRIPTION "Custom Vulkan graphics engine library"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(VKENGINE_BUILD_SHARED_LIBS "Build vulkan_engine as a shared library" OFF)
option(VKENGINE_BUILD_EXAMPLES "Build example applications" ON)
option(VKENGINE_BUILD_TESTS "Build unit tests" ON)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(VKENGINE_IS_TOP_LEVEL ON)
else()
  set(VKENGINE_IS_TOP_LEVEL OFF)
endif()

find_package(Vulkan REQUIRED COMPONENTS shaderc_combined SPIRV-Tools)

include(FetchContent)

FetchContent_Declare(
  vma
  GIT_REPOSITORY 
    https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
  GIT_TAG        v3.4.0
  GIT_SHALLOW    TRUE
)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.3
    GIT_SHALLOW    TRUE
)

set(_prev_shared_libs ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.5.1
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(vma glm glfw)

set(BUILD_SHARED_LIBS ${_prev_shared_libs} CACHE BOOL "" FORCE)

add_subdirectory(src)

if(VKENGINE_IS_TOP_LEVEL AND VKENGINE_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if(VKENGINE_IS_TOP_LEVEL AND VKENGINE_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
