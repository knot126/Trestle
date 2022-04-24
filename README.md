**Important**: This engine was originally created for a game I was developing (that I might still make eventually). Currently, I want to rework the engine to make it work better - and so I don't have to start on a new engine (yet). :)

# Trestle Engine

This is the main repository for Decentix's Trestle Engine, which currently aims to implement Quick Run. Please note that it is **not yet feature complete**, and it is missing many things.

## Building

### Linux

I recommend creating an out-of-tree build folder (I just call it "EngineBuild" and put it next to the engine directory), then running:

```bash
cmake ../Engine
make -j4
```

## Dependencies

In the future, all game dependencies should be put into the `lib` directory and be built directly as part of the game. Right now only some of them are. Currently these need to be installed:

 * Standard C library and build tools
 * `glfw` (either Wayland or X.org)

## Libraries

The following libraries are used in this engine, so far:

 * Decentix Core library (see `src/util`)
 * GLEW (see `lib/glew`)
 * Lua (see `lib/lua`)
 * STB (see `lib/stb`)
