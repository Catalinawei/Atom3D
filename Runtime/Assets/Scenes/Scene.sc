# Physics test scene: floor + falling cube

newobj Floor
p 0.0 -5.0 0.0
r 0.0 0.0 0.0
s 20.0 0.5 20.0
vSh Assets/Shaders/LightingVertexShader.glsl
fSh Assets/Shaders/LightingFragmentShader.glsl
mesh Assets/Objects/test.obj
p_mass 0.0

newobj Cube
p 0.0 3.0 0.0
r 30.0 45.0 145.0
s 1.0 1.0 1.0
vSh Assets/Shaders/LightingVertexShader.glsl
fSh Assets/Shaders/LightingFragmentShader.glsl
mesh Assets/Objects/test.obj
p_mass 1.0

newobj Cube2
p 0.0 8.0 0.0
r 323.0 12.0 234.0
s 1.0 1.0 1.0
vSh Assets/Shaders/LightingVertexShader.glsl
fSh Assets/Shaders/LightingFragmentShader.glsl
mesh Assets/Objects/test.obj
p_mass 0.5