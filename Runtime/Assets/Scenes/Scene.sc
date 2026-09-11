# Physics test scene: floor + falling cube

newobj Person
p 0.0 3.0 0.0
r 0.0 0.0 0.0
s 0.5 0.5 0.5
vSh Assets/Shaders/LightingVertexShader.glsl
fSh Assets/Shaders/LightingFragmentShader.glsl
mesh Assets/Objects/model.obj
p_mass 50
new_comp charController

newobj Floor
p 0.0 -5.0 0.0
r 0.0 0.0 0.0
s 20.0 0.5 20.0	
vSh Assets/Shaders/LightingVertexShader.glsl
fSh Assets/Shaders/LightingFragmentShader.glsl
mesh Assets/Objects/test.obj
p_mass 0.0