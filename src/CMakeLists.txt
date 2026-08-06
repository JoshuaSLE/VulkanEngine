if(VKENGINE_BUILD_SHARED_LIBS)
  add_library(vulkan_engine SHARED
    window.cpp
    renderer/renderer.cpp
    )
else()
  add_library(vulkan_engine STATIC
    window.cpp
    renderer/renderer.cpp
    )
endif()
add_library(VulkanEngine::vulkan_engine ALIAS vulkan_engine)

if(WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_link_options(vulkan_engine INTERFACE -static)
endif()

target_include_directories(vulkan_engine
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(vulkan_engine
  PUBLIC
    glm::glm
  PRIVATE
    Vulkan::Vulkan
    VulkanMemoryAllocator
    glfw
)

set_target_properties(vulkan_engine PROPERTIES
  OUTPUT_NAME "vulkan_engine"
  POSITION_INDEPENDENT_CODE ON
)

if(VKENGINE_BUILD_SHARED_LIBS)
  target_compile_definitions(vulkan_engine PUBLIC VKENGINE_SHARED)
  target_compile_definitions(vulkan_engine PRIVATE VKENGINE_BUILD_DLL)
endif()
