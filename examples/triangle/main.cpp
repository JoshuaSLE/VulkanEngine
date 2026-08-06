#include "vulkan_engine/window.hpp"
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {

  std::clog << "Triangle render staring...\n";

  Window::Config config{};
  config.width = 800;
  config.height = 600;
  config.title = "Triangle";
  config.fullscreen = false;
  config.resizable = true;
  config.monitorIndex = 0;

  try {
    Window window(config);
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
