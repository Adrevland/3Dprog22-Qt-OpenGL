#version 410 core
out vec4 FragColor;

in vec4 ourColor;

void main()
{
    FragColor = vec4(ourColor.xyz,1.0);

}
