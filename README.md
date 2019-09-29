# Volume Renderer

Another volume data set renderer.


## Features

- [ ] Volume data set loading
  - [x] RAW
  - [ ] PVM
- [ ] Texture based techniques
  - [ ] 2D textures: Model aligned planes
  - [x] 3D textures: Viewport aligned polygons
- [ ] Ray casting
  - [ ] CPU
  - [ ] GPU
- [ ] Built-in transfer function GUI editor


## Controls
Camera:
- W, S: Move the camera forward or backward
- A, D: Move the camera to the right or left (strafe)
- Space, C: Move the camera up or down
- Shift: Boost the camera speed
- P: Toggle proyection (perspective/orthogonal)

Mouse:
- Left button:
  - Volume: Rotate
  - Channel: Set value
  - Transfer function: Select node
- Right button:
  - Volume: Translate
  - Transfer function: Remove current node
- Wheel: Zoom

Settings:
- I: Toggle the GUI
- F5: Reload the volume from disk
- F6: Reload the GLSL program from disk


# Resources

Since the volume data sets are too big, they aren't included in the repository,
but they can be found in the next links:
  - https://www.cg.tuwien.ac.at/research/vis/datasets/
  - https://graphics.stanford.edu/data/voldata/
  - http://schorsch.efi.fh-nuernberg.de/data/volume/

Anyway, the [`carp.dat`] file in raw format _(256x256x512, 16 bits, ~64MB)_ was
included into the [`volume`] folder and is loaded by default.


# Dependencies

- [GLFW]\: Library for OpenGL
- [Glad]\: OpenGL extension loader library
- [GLM]\: OpenGL mathematics


[//]: # "Links references"
[`carp.dat`]: volume/carp.dat
[`volume`]: volume

[GLFW]: https://www.glfw.org/
[Glad]: https://github.com/Dav1dde/glad
[GLM]: http://glm.g-truc.net/