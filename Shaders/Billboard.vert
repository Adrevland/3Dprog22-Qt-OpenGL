#version 460 core
layout (location = 0) in vec3 positionIn;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 fragPos;

//dont think i need all of them
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){

	mat3 invViewRot = inverse(mat3(view));
	vec3 pos = invViewRot * positionIn;

	gl_Position = projection * view * model * vec4(pos, 1.0); 

	TexCoord = aTexCoord;
	fragPos = vec3(model * vec4(pos.xyz,1.0));
}

