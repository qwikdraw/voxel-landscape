# Voxel Landscape

Procedural voxel landscape engine.  
Humidity / temperature based biome distribution and perlin field based  
terrain generation.

Rendering engine uses greedy meshing to dramatically reduce vertex counts and GPU memory usage.  
Chunk generation is multi threaded.

![Flyover gif](https://raw.githubusercontent.com/qwikdraw/voxel-landscape/master/screenshots/voxel.gif)

### Dependencies
`pkg-config`  
`glm`  
`glfw`  
`wget`  
`make`  

### Building

On macOS:
```
./deps.sh
make
```

### Team
Theo Walton  
Logan Kaser
