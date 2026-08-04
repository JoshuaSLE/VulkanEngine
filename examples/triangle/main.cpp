#include "vulkan_engine/window.hpp"
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {

  Window::Config config{};
  config.width = 800;
  config.height = 600;
  config.title = "Triangle";

  try {
    Window window(config);

    while (!window.shouldClose()) {
      window.pollEvents();
    }
  } catch (const std::runtime_error &e) {
    std::cerr << "Caught an exception: " << e.what() << "\n";
  }

  return EXIT_SUCCESS;
}
