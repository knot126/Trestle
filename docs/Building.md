# Building Trestle

This document contains instructions for building the Trestle engine.

## Windows

**Note**: The build for windows does not work right now, and currently is only planned for ARM64-based systems.

## Linux

### Get Dependncies

Firstly, install the following dependncies if they are not installed:

  * Basic development tools
  * GLFW
  * PulseAudio

For example, on Arch-based distros of linux with a desktop, run:

```zsh
sudo pacman -Syu base-devel glfw
```

### Build the Engine

Then, in the root engine folder, run:

```zsh
python ./build.py linux
```

The build system will output a file named `trestle-linux-native.elf` in the `temp` folder.

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
