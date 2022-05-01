#version 460 core
layout (location = 0) in vec3 positionIn;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 anormals;

out vec4 fragColor;
out vec2 TexCoord;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    gl_Position = projection * view * model * vec4(positionIn.xyz, 1.0);
    TexCoord = aTexCoord;
    normal = mat3(transpose(inverse(model)))*anormals;
    fragColor = aColor;
    fragPos = vec3(model * vec4(positionIn.xyz,1.0));
}
