#version 410 core
out vec4 FragColor;

in vec4 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoord) * vec4(ourColor.xyz,1.0);
    //FragColor = vec4(1.0,1.0,1.0,1.0); // white texture
    //FragColor = ourColor; // only color
}
