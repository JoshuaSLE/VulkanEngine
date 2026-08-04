#pragma once

#include <cstdint>
#include <string>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Window {
public:
  struct Config {
    uint16_t width{1920};
    uint16_t height{1080};
    std::string title = "Vulkan Graphics Engine";
    bool fullscreen = false;
  };

  explicit Window(Config config);
  ~Window();

  bool shouldClose() { return glfwWindowShouldClose(window_); };
  void pollEvents() { glfwPollEvents(); }

private:
  GLFWwindow *window_ = nullptr;
  Config config_;

  inline static uint16_t windowCount_ = 0;
};
