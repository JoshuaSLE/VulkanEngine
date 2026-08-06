#include "vulkan_engine/renderer.hpp"
#include "vulkan_engine/window.hpp"
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {

  std::clog << "Triangle render staring...\n";

  Window::Config windowConfig{};
  windowConfig.width = 800;
  windowConfig.height = 600;
  windowConfig.title = "Triangle";
  windowConfig.fullscreen = false;
  windowConfig.resizable = true;
  windowConfig.monitorIndex = 0;

  Renderer::Config rendererConfig{};
  rendererConfig.applicationName = "Triangle render";

  try {
    Window window(windowConfig);

    rendererConfig.glfwExtensions = window.getRequiredExtensions();

    Renderer renderer(rendererConfig);

    window.show();

    // INFO: Main loop
    while (!window.shouldClose()) {
      window.pollEvents();

      // INFO: Pause rendering loop
      if (window.isMinimized()) {
        window.waitEvents();
        continue;
      }

      // INFO: Handle Window resized
      if (window.wasResized()) {
        window.resetResizedFlag();
      }

      // INFO: Render frame
    }
  } catch (const std::runtime_error &e) {
    std::cerr << "Caught an exception: " << e.what() << "\n";
  }

  std::clog << "Triangle render ending...\n";

  return EXIT_SUCCESS;
}
