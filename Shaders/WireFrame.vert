#version 410 core
layout (location = 0) in vec4 positionIn;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 ourColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(positionIn.xyz, 1.0);
    TexCoord = aTexCoord;
    ourColor = aColor;
}
