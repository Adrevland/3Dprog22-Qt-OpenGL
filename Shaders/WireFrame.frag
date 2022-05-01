#version 410 core
out vec4 FragColor;

in vec4 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
    FragColor = vec4(1.0,1.0,0.0,1.0); // Solid color 
    //FragColor = vec4(1.0,1.0,1.0,1.0); // white texture
    //FragColor = ourColor; // only color
}
