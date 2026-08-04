#include "vulkan_engine/window.hpp"
#include "GLFW/glfw3.h"
#include <iostream>
#include <stdexcept>
#include <utility>

namespace {
void glfwErrorCallback(int error, const char *description) {
  std::cerr << "GLFW Error [" << error << "] : " << description << "\n";
}
} // namespace

Window::Window(Config config) : config_(std::move(config)) {
  if (!windowCount_) {
    if (!glfwInit()) {
      throw std::runtime_error("Failed to init GLFW");
    }

    glfwSetErrorCallback(glfwErrorCallback);
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  window_ = glfwCreateWindow(config_.width, config_.height,
                             config_.title.c_str(), nullptr, nullptr);

  if (!window_) {
    throw std::runtime_error("Failed to create GLFW window");
  }

  glfwShowWindow(window_);

  ++windowCount_;
}

Window::~Window() {
  if (window_) {
    glfwDestroyWindow(window_);

    if (--windowCount_ == 0) {
      glfwTerminate();
    }
  }
}
