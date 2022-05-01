#version 460 core
out vec4 FragColor;

in vec4 fragColor;
in vec3 fragPos;
in vec2 TexCoord;
in vec3 normal;

//shadows
uniform sampler2DArray shadowmaps;
uniform vec3 lightDir;
uniform float farPlane;
uniform mat4 view;


layout (std140, binding = 0) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[4];
};
uniform float cascadePlaneDistances[4];
uniform int cascadeCount;

uniform sampler2D texture1;
uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform int ViewMode=0;

float ShadowCalculation(vec3 fragposWorldSpace)
{
// select cascade layer
    vec4 fragPosViewSpace = view * vec4(fragposWorldSpace, 1.0);
    float depthValue = abs(fragPosViewSpace.z);

    int layer = -1;
    for (int i = 0; i < cascadeCount; ++i)
    {
        if (depthValue < cascadePlaneDistances[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = cascadeCount;
    }

    vec4 fragPosLightSpace = lightSpaceMatrices[layer] * vec4(fragposWorldSpace, 1.0);
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (currentDepth > 1.0)
    {
        return 0.0;
    }
    // calculate bias (based on depth map resolution and slope)
    vec3 norm = normalize(normal);
    float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);
    const float biasModifier = 5.f;
    //bias = 0.00005;
    if (layer == cascadeCount)
    {
        bias *= 1 / (farPlane * biasModifier);
    }
    else
    {
        bias *= 1 / (cascadePlaneDistances[layer] * biasModifier);
    }

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(shadowmaps, 0));
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowmaps, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r;
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
        
    return shadow;
}



void main()
{
    //253.0/255.0, 184.0/255.0,19.0/255.0 // orange
    
    //vec3 dir = normalize(vec3(0.0,0.3,0.3));
    
    //light
    float specularStrength = 0.3;
    float ambientStrength = 0.2;
    float shininess = 0.5;
    float lightStrength = 1.3;
    vec3 lightColor = normalize(vec3(0.72,0.7,0.8));
    
    vec3 color = texture(texture1, TexCoord).rgb;
   
   //normalise
    vec3 norm = normalize(normal);

     //ambient
    vec3 ambient = ambientStrength * fragColor.rgb;

    //diffuse
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff * lightColor;

    
    //specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;  

    //shadow
    float shadow = ShadowCalculation(fragPos);
    
    //vec3 specular = lightColor * spec;

   
    //calculatet light
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * lightStrength;

    //vec3 result = (ambient + diffuse + specular ) * fragColor.xyz;


    if(ViewMode == 0){
        FragColor = vec4(lighting*color,1.0);
    }
    else if(ViewMode == 1){
        FragColor = vec4(normal,1.0);
    }
    else if(ViewMode == 2){
        FragColor = vec4(lighting,fragColor.w);
    }
    //FragColor = vec4(normal,1.0);
    //FragColor = vec4(1.0,1.0,1.0,1.0); // white texture
    //FragColor = ourColor; // only color
}
