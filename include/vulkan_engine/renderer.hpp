#pragma once

#include <string>
#include <vector>

struct VkInstance_T;
using VkInstance = VkInstance_T *;

struct VkDebugUtilsMessengerEXT_T;
using VkDebugUtilsMessengerEXT = VkDebugUtilsMessengerEXT_T *;

class Renderer {
public:
  struct Config {
    std::string applicationName = "No name";
    std::vector<const char *> glfwExtensions;
  };

  explicit Renderer(Config config);
  ~Renderer();

  Renderer(const Renderer &) = delete;
  Renderer &operator=(const Renderer &) = delete;

  Renderer(Renderer &&) = delete;
  Renderer &operator=(Renderer &&) = delete;

private:
  VkInstance instance_ = nullptr;
  VkDebugUtilsMessengerEXT debugMessenger_ = nullptr;
  Config config_;

  void setupDebugMessenger();
  bool checkValidationLayerSupport();
};
