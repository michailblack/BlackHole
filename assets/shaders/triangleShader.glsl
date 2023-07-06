#type vertex
#version 460 core
layout (location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjectionMatrix;

void main()
{
    gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}

#type fragment
#version 460 core
layout (location = 0) out vec4 FragColor;        

void main()
{
    FragColor = vec4(0.2, 0.3, 0.8, 1.0);
}