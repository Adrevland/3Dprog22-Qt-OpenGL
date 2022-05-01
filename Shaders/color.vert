#version 410 core
layout (location = 0) in vec4 positionIn;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 anormals;

out vec4 ourColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos; 

void main()
{
    gl_Position = projection * view * model * vec4(positionIn.xyz, 1.0);
    TexCoord = aTexCoord;
    ourColor = aColor;
}
