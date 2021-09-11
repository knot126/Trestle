# Building Trestle

This document contains instructions for building the Trestle engine.

## Arch-based Linux

### Get Dependncies

Firstly, install the following dependncies:

  * Basic development tools
  * Cmake
  * GLFW
  * PulseAudio

For example, on Arch based distrobutions of linux with a desktop, run:

```c
sudo pacman -Syu glfw cmake
```

This is assuming that basic development tools and standard desktop things like PulseAudio are already installed, like on most arch based and Manjaro linux.

### Build the Game

Then, in a folder outside of the build directory, run:

```zsh
cmake ../Trestle
make -j4
```

After that, there will be a binary called `quickrun` and a library called `libmelon.so`.

## Appendix A: Specific Libraries Required

The following libraries are linked against or loaded and required on a Linux system:

  * `libc.so`: Standard C library
  * `libm.so`: Standard C maths
  * `libGL.so`: OpenGL graphics
  * `libdl.so`: Dynamic loading and linking library
  * `libglfw.so`: System GLFW library (input, windowing and etc.)
  * `libpthread.so`: POSIX multithreading library
  * `libpulse.so`: PulseAudio audio client library
  * `libpulse-simple.so`: PulseAudio simple API library
