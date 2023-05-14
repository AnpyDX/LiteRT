# Lite RT
**Lite Ray Tracer** is a small(actually not) renderer using OpenGL.

# Features
## Rendering
- [ ] Physically Based Rendering
- [x] BVH & SAH Boost
## Formats
- [ ] Wavefont OBJ (with mtl)
- [ ] glTF
## GUI
- [ ] Built-in GUI
## Others
- [x] SPIR-V Shader

# Build
**LiteRT** is managed with ``CMake``. You can build this project with CMake Tools on Windows and Linux (macOS do not support OpenGL 4.6). 

Besides, **LiteRT** requires your computer supporting **OpenGL 4.6**!

## 1. install dependencies
### Windows
On Windows, you need to install ``CMake`` and ``glslangValidator``.
You can install them matually or using **scoop**.

- [CMake](https://cmake.org/download/)
- [glslang](https://github.com/KhronosGroup/glslang)

```bash
scoop install cmake glslangValidator
```

### Linux
Debian / Ubuntu / Mint (X11)
```bash
sudo apt install cmake xorg-dev glslc
```
If you are using Wayland or other Linux Distributions, you can find appropriate commands in [GLFW Documents](https://www.glfw.org/docs/latest/compile.html). The required dependecies of **LiteRT** are installed for GLFW.

## 2. build project
Run following commands under project root directory.
```bash
mkdir build & cd build
cmake ..
cmake --build -DCMAKE_BUILD_TYPE=Release .
```

Then get executable app under ``~/exec-dir/``.

# ThirdParty
These projects are used in here. Thanks to their efforts!
- [GLAD](https://github.com/Dav1dde/glad)
- [GLFW](https://github.com/glfw/glfw)
- [GLM](https://github.com/g-truc/glm)

# Reference
- [光线追踪渲染实战](https://blog.csdn.net/weixin_44176696/article/details/119044396)
- [LearnOpnGL](https://learnopengl.com/)
- [LearnOpenGL-CN](https://learnopengl-cn.github.io/)