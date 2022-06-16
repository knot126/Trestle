# Trestle Engine

**Trestle Engine** is a game and simulation engine. It <s>is</s> *will* be designed to support more per-game physics simulations, procedurally generated content and tighter integration between heterogeneous compute and scripting.

***Important**: Trestle is currently undergoing a transition which will take a very different approach compared to what it was and what most current game engines are. I don't really want to begin to write a new engine yet, but I am considering starting a new engine project.*

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
