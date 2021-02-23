# Trestle Engine

This is the main repository for Decent Games' Trestle Engine, which currently aims to implement Quick Run. Please note that it is **not yet feature complete**, and it is missing many things. The engine only works on Linux right now, though I should start a Windows port before things get too crazy.

## Building

I recommend creating an out-of-tree build folder (I just call it "EngineBuild" and put it next to the engine directory), then running:

```bash
cmake ../Engine
make -j4
```

## Dependencies

In the future, all game dependencies should be put into the `lib` directory and be built directly as part of the game. Right now only some of them are. Currently these need to be installed:

 * Standard C library and build tools
 * `vulkan-sdk` or `vulkan-devel` (though this should be removed)
 * `glfw` (either Wayland or X.org)
