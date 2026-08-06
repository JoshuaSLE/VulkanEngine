#include "vulkan_engine/window.hpp"
#include <iostream>
#include <stdexcept>
#include <utility>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

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
  glfwWindowHint(GLFW_RESIZABLE, config_.resizable ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  GLFWmonitor *monitor = nullptr;
  if (config_.fullscreen) {
    int count;
    GLFWmonitor **monitors = glfwGetMonitors(&count);
    monitor = (config_.monitorIndex < count) ? monitors[config_.monitorIndex]
                                             : glfwGetPrimaryMonitor();
  }

  window_ = glfwCreateWindow(config_.width, config_.height,
                             config_.title.c_str(), monitor, nullptr);

  if (!window_) {
    throw std::runtime_error("Failed to create GLFW window");
  }

  glfwSetFramebufferSizeCallback(window_, framebufferResizeCallback);
  glfwSetWindowUserPointer(window_, this);

  if (!config_.fullscreen) {
    centerOnMonitor(config_.monitorIndex);
  }

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

std::vector<const char *> Window::getRequiredExtensions() {
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions =
      glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  return {glfwExtensions, glfwExtensions + glfwExtensionCount};
}

void Window::show() const { glfwShowWindow(window_); }

bool Window::shouldClose() const { return glfwWindowShouldClose(window_); }

void Window::pollEvents() const { glfwPollEvents(); }

void Window::waitEvents() const { glfwWaitEvents(); }

bool Window::isMinimized() const {
  int width = 0, height = 0;
  glfwGetFramebufferSize(window_, &width, &height);
  return width == 0 || height == 0;
}

void Window::handleMonitorDisconnect(GLFWmonitor *) {
  if (!config_.fullscreen)
    return;

  int count;
  std::ignore = glfwGetMonitors(&count);

  if (count > 0) {
    GLFWmonitor *primary = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(primary);

    glfwSetWindowMonitor(window_, primary, 0, 0, mode->width, mode->height,
                         mode->refreshRate);
  }
}

void Window::centerOnMonitor(int monitorIndex) {
  int count;
  GLFWmonitor **monitors = glfwGetMonitors(&count);
  if (count == 0)
    return;

  GLFWmonitor *target = (monitorIndex >= 0 && monitorIndex < count)
                            ? monitors[monitorIndex]
                            : glfwGetPrimaryMonitor();

  int mx, my, mw, mh;
  glfwGetMonitorWorkarea(target, &mx, &my, &mw, &mh);

  int wx, wy;
  glfwGetWindowSize(window_, &wx, &wy);

  glfwSetWindowPos(window_, mx + (mw - wx) / 2, my + (mh - wy) / 2);
}

void Window::framebufferResizeCallback(GLFWwindow *window, int, int) {
  auto self = static_cast<Window *>(glfwGetWindowUserPointer(window));

  if (self) {
    self->framebufferResized_ = true;
  }
}
