# OpenGL Notes

AKA "Why I hate OpenGL and want to switch to Vulkan."

 * OpenGL seems to be completely messed up on Mesa/Intel. I can't even tell if they are setting a default texture unit. 
 * Immediately after loading a texture to OpenGL you MUST push it as the currently bound texture for any texture unit; otherwise, it is liabel to break on Mesa/Intel for whatever reason. See [Example 1](#Example 1).

## Examples 

### Example 1

This BREAKS textures badly:

```c
gltexture_load_file(resman, "test0", "assets://texture0.png");
gltexture_load_file(resman, "test1", "assets://texture1.png");
gltexture_load_file(resman, "test2", "assets://texture2.png");
```

Adding a line to set the active unit to any texture unit (usually `GL_TEXTURE0` is okay) after loading each seems to fix the issue.

```c
gltexture_load_file(resman, "test0", "assets://texture0.png");
gltexture_set_unit(resman, "test0", GL_TEXTURE0);
gltexture_load_file(resman, "test1", "assets://texture1.png");
gltexture_set_unit(resman, "test1", GL_TEXTURE0);
gltexture_load_file(resman, "test2", "assets://texture2.png");
gltexture_set_unit(resman, "test2", GL_TEXTURE0);
```
