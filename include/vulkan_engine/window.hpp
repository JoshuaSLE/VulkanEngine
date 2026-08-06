#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct GLFWwindow;
struct GLFWmonitor;

struct WindowExtent {
  uint32_t width;
  uint32_t height;
};

class Window {
public:
  struct Config {
    uint16_t width{1920};
    uint16_t height{1080};
    std::string title = "Vulkan Engine";
    bool fullscreen = false;
    bool resizable = true;
    int monitorIndex = 0;
  };

  explicit Window(Config config);
  ~Window();

  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

  Window(Window &&) noexcept = default;
  Window &operator=(Window &&) noexcept = default;

  GLFWwindow *getNativeWindow() const { return window_; }
  static std::vector<const char *> getRequiredExtensions();

  void show() const;
  bool shouldClose() const;
  void pollEvents() const;
  void waitEvents() const;

  bool isMinimized() const;
  bool wasResized() const { return framebufferResized_; }
  void resetResizedFlag() { framebufferResized_ = false; }

private:
  GLFWwindow *window_ = nullptr;
  Config config_;
  bool framebufferResized_ = false;

  inline static uint16_t windowCount_ = 0;

  void centerOnMonitor(int monitorIndex);
  static void framebufferResizeCallback(GLFWwindow *window, int width,
                                        int height);

  void handleMonitorDisconnect(GLFWmonitor *monitor);
};
