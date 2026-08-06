# VulkanEngine — Development Roadmap

Scope assumptions baked into this roadmap:
- **Cross-platform from the start** (Windows/MSYS2 + Linux + macOS via MoltenVK)
- **No time estimates** — ordered by dependency, not calendar
- **SimulationApplication (fly-cam, fixed timestep) is the first working demo**; GameApplication follows once the composition model is proven
- **Volk + VMA are planned integrations**, added once the core loop and Renderer are stable — not deferred as an afterthought, but sequenced deliberately so you're not debugging a loader swap and your first triangle at the same time

---

## Target Project Structure

```
VulkanEngine/
├── include/
│   └── vulkan_engine/
│       ├── application.hpp        # Application base + Config
│       ├── game_application.hpp
│       ├── simulation_application.hpp
│       ├── window.hpp             # opaque handle, no GLFW types exposed
│       ├── renderer.hpp           # opaque handle, no Vk types exposed (pImpl)
│       ├── input.hpp
│       ├── scene.hpp
│       ├── camera.hpp
│       ├── camera_controller.hpp  # CameraController, FlyCameraController, PlayerCameraController
│       ├── timestep_policy.hpp    # TimestepPolicy, FixedTimestep, VariableTimestep
│       └── api.hpp                # VKENGINE_API export/import macros
├── src/
│   ├── application.cpp
│   ├── game_application.cpp
│   ├── simulation_application.cpp
│   ├── window.cpp                 # GLFW implementation detail
│   ├── renderer/
│   │   ├── renderer.cpp           # public-facing pImpl shell
│   │   ├── renderer_impl.hpp      # internal-only, full of Vk* types
│   │   ├── device.cpp             # instance/device/queue selection
│   │   ├── swapchain.cpp
│   │   ├── pipeline.cpp
│   │   └── shader_compiler.cpp    # shaderc usage, isolated here
│   ├── camera_controller.cpp
│   └── timestep_policy.cpp
├── examples/
│   ├── CMakeLists.txt             # add_subdirectory(triangle), add_subdirectory(sim_demo) later
│   └── triangle/
├── tests/
├── build/                         # git-ignored
└── CMakeLists.txt
```

Everything under `renderer/` except `renderer.cpp`'s public shell is intentionally invisible to consumers — this is where the pImpl boundary lives, and where volk/VMA/shaderc calls stay contained.

---

## Phase 0 — Cross-Platform Toolchain Verification

Before writing any engine code, confirm the *build itself* works identically across targets: MSYS2/Clang on Windows, and at least one of Linux (gcc/clang) or macOS. This is easy to skip and expensive to fix later — platform assumptions (path separators, case-sensitive filesystems on Linux/macOS vs. case-insensitive on Windows, differing default compiler flags) creep into a codebase silently if only one platform is ever built.

- Confirm `cmake --preset` + Ninja produce a working empty project on each target OS.
- Confirm `find_package(Vulkan REQUIRED)` succeeds on each — on macOS this means the Vulkan SDK's MoltenVK is installed and `VULKAN_SDK` is set.
- Set up a CI matrix (GitHub Actions: `windows-latest`, `ubuntu-latest`, `macos-latest`) now if you want early warning on platform drift — much cheaper than debugging cross-platform breakage after Phase 5.

## Phase 1 — Hello Triangle (No Architecture)

Hardcode everything in `main()`: instance, device, surface, swapchain, one pipeline, one triangle. Goal is proving the toolchain end-to-end, not writing reusable code — you don't know what `Application` needs until you've built something once.

**Cross-platform note:** on macOS, MoltenVK requires enabling `VK_KHR_portability_enumeration` on the instance and `VK_KHR_portability_subset` on the device — without these, instance/device creation silently behaves differently or fails validation. Confirm this now, on the simplest possible triangle, rather than discovering it once Renderer abstraction is in the way.

## Phase 2 — Extract Window + Renderer

Pull GLFW handling into `Window`, Vulkan setup into `Renderer`, still driven from a plain `main()`. Keep `Window`'s public header free of `GLFWwindow*` — return an opaque handle or engine-defined type instead, so GLFW can stay a `PRIVATE` link dependency. Same principle for `Renderer`: start the pImpl now, even if it feels premature — retrofitting it after `Application` and examples depend on raw `Vk*` types is far more painful than starting clean.

## Phase 3 — Input

Wrap keyboard/mouse polling behind `Input`. Validate it by moving the camera you'll build next.

## Phase 4 — Camera + FlyCameraController

Since Simulation is your first target, build `FlyCameraController` before `PlayerCameraController`. Validate view/projection math and free-fly movement against a handful of hardcoded objects — this is the foundation `SimulationApplication` needs first; `PlayerCameraController` can wait until Phase 8.

## Phase 5 — Scene

Introduce a minimal container: a list of meshes + transforms, no ECS. `Renderer` should consume `Scene` data instead of hardcoded geometry from this point on.

## Phase 6 — Application + FixedTimestep

Wrap Window/Renderer/Input/Camera/Scene in the `Application` lifecycle (Init → Run → Shutdown). Implement `FixedTimestep` first, since it's what `SimulationApplication` needs — `VariableTimestep` can wait until Phase 8. This is also the point where the `Config`-based composition (timestep policy + camera controller as owned, swappable objects) gets validated for real, rather than staying a paper design.

## Phase 7 — SimulationApplication (First Working Demo)

Assemble `FixedTimestep` + `FlyCameraController` into `SimulationApplication`. This is your first true end-to-end demo through the full architecture — fixed-step update, free-cam navigation, scene rendering. Treat this as the checkpoint that validates the whole composition model before building a second app type on top of it.

## Phase 8 — GameApplication

Now add `VariableTimestep` and `PlayerCameraController`, and assemble `GameApplication`. Because the policy objects are already proven by Phase 7, this should mostly be new policy implementations plus gameplay-specific `OnUpdate`/`OnRender` logic — not a parallel lifecycle rewrite. If it *does* require touching `Application` itself, that's a signal something was too Simulation-specific in Phase 6 and needs generalizing.

## Phase 9 — Volk Integration

Swap the standard Vulkan loader for volk. Because call sites use the same `vk*` function names either way, this is mostly an initialization-sequence change (`volkInitialize()`, `volkLoadInstance()`, `volkLoadDevice()`) rather than a rewrite — confirm this holds cross-platform, since volk's dynamic-loading behavior differs slightly between Windows/Linux/macOS. Do this after Phases 7–8 are working, not before — you want a known-good baseline to diff against if the loader swap introduces a subtle bug.

## Phase 10 — VMA Integration

Replace manual `vkAllocateMemory`/`vkBindBufferMemory` calls in `Renderer` with VMA. This is naturally scoped to `renderer/device.cpp` and buffer/image creation code — confirm allocation strategy (e.g. `VMA_MEMORY_USAGE_AUTO` vs. explicit flags) works consistently across the GPU vendors you can test against, since driver behavior around memory types varies more than the Vulkan spec suggests.

## Phase 11 — Ongoing Hardening

Swapchain recreation edge cases (minimized window, alt-tab, DPI changes), shader hot-reload, pipeline abstraction generalized from whatever Phases 7–8 actually needed — driven by real usage, not upfront guessing.

---

## Other Notes

- **Enable Vulkan validation layers in debug builds from day one.** Nearly all early Vulkan confusion is silently-wrong API usage that validation catches instantly instead of a mysterious black screen or platform-specific crash.
- **Don't build a generic ECS/scene graph speculatively.** Wait until both `GameApplication` and `SimulationApplication` exist and you can see what they actually share.
- **Commit at every working milestone**, especially around Phases 9–10 — loader and allocator swaps are exactly the kind of change where you want a clean rollback point if something breaks in a way that's hard to diagnose.
- **Watch for case-sensitivity bugs** in shader/asset paths once you're testing on Linux/macOS — a path that resolves fine on Windows can 404 silently elsewhere.
- **Keep a running NOTES.md of *why*, not just *what*** — especially useful for revisiting Phase 9/10 decisions (why volk over the standard loader in your specific setup, what VMA usage flags you picked and why) after time away from the project.
