# VulkanEngine

A custom C++ graphics engine built on Vulkan, written from scratch as a learning project — and as a foundation for future graphics applications.

## Features

- [ ] Cross-platform window management (GLFW), with resize handling and fullscreen support
- [ ] Vulkan instance / device selection
- [ ] Swapchain creation and recreation on resize
- [ ] Graphics pipeline
- [ ] Shader compilation (GLSL → SPIR-V)
- [ ] Basic renderer (triangle example)

## Prerequisites

- **[MSYS2](https://www.msys2.org/) (`CLANG64` environment)** — C++23 compiler toolchain (LLVM Clang)
- **[CMake](https://cmake.org/) ≥ 4.0** — required by this project's minimum version
- **[Vulkan SDK](https://vulkan.lunarg.com/sdk/home)** — during install, select the Volk, Shader components

### Required System Packages (MSYS2)

Install the build toolchain via `pacman` inside your MSYS2 `CLANG64` environment:

```bash
pacman -S --needed \
  mingw-w64-clang-x86_64-cmake \
  mingw-w64-clang-x86_64-ninja \
  mingw-w64-clang-x86_64-clang \
  mingw-w64-clang-x86_64-clang-tools-extra
```

Vulkan Memory Allocator, GLM, GLFW, and Catch2 are pulled automatically via CMake `FetchContent` — no separate install needed for those.

## Building

This project uses [CMake Presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html) for configuration. Build output goes to `build/debug/` or `build/release/`.

```bash
# Debug build
cmake --preset debug
cmake --build --preset debug

# Release build
cmake --preset release
cmake --build --preset release

# Run tests
ctest --preset debug
```

## Project Structure

```
VulkanEngine/
├── include/    # Public headers (installed API surface)
├── src/        # Library implementation
├── examples/   # Example applications (e.g. triangle)
├── tests/      # Unit tests
├── build/      # Build output (debug/release), git-ignored
└── CMakeLists.txt
└── CMakePresets.json
```

## CMake Options

| Option                    | Default | Description                               |
| ------------------------- | ------- | ----------------------------------------- |
| `VKENGINE_BUILD_SHARED_LIBS  ` | `OFF`   | Build `vulkan_engine` as a shared library |
| `VKENGINE_BUILD_EXAMPLES` | `ON`    | Build the example applications            |
| `VKENGINE_BUILD_TESTS`    | `ON`    | Build the unit test suite                 |

## Dependencies

| Library                                          | Source                                     | Purpose                        |
| ------------------------------------------------ | ------------------------------------------ | ------------------------------ |
| **[Vulkan](https://www.vulkan.org/)**            | System (Vulkan SDK / MSYS2 `vulkan-devel`) | Graphics API loader & headers  |
| **[GLFW](https://www.glfw.org/)** (3.5.1)        | FetchContent                               | Windowing and input management |
| **[GLM](https://github.com/g-truc/glm)** (1.0.3) | FetchContent                               | Mathematics library            |
| **[Catch2](https://github.com/catchorg/Catch2)** | FetchContent                               | Unit testing framework         |

> Vulkan Memory Allocator is installed via the Vulkan SDK but not yet wired into the build — add it here once it's linked in `src/CMakeLists.txt`.

## License

See [LICENSE](LICENSE).
