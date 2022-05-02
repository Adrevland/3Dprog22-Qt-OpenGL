#version 460 core
out vec4 FragColor;

in vec4 fragColor;
in vec3 fragPos;
in vec2 TexCoord;
in vec3 normal;

uniform sampler2D texture1;
uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform int ViewMode=0;

//uniform vec3 lightDir;

void main()
{
    //253.0/255.0, 184.0/255.0,19.0/255.0 // orange
    //light
    float specularStrength = 0.2;
    float ambientStrength = 0.4;
    float lightStrength = 1.0;

    vec3 lightColor = vec3(0.72,0.7,0.8);

    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);  
    //vec3 dir = normalize(lightDir - fragPos);  
    vec3 dir = normalize(lightDir);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(dir, norm);  

    float spec = pow(max(dot(viewDir, -reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  

    float diff = max(dot(norm, dir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse + specular) * fragColor.xyz;
    result = result * lightStrength;

    if(ViewMode == 0){
        FragColor = texture(texture1, TexCoord) * vec4(result,fragColor.w);
    }
    else if(ViewMode == 1){
        FragColor = vec4(normal,1.0);
        //FragColor = vec4(dir,1.0);
    }
    else if(ViewMode == 2){
        FragColor = vec4(result,fragColor.w);
    }
    //FragColor = vec4(normal,1.0);
    //FragColor = vec4(1.0,1.0,1.0,1.0); // white texture
    //FragColor = ourColor; // only color
}