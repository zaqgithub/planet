#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 anormal;
layout (location = 2) in vec2 aTexCoords;
layout (std140,binding =0) uniform matrix{
	mat4 view;
	mat4 project;
};
out vec2 textureCoor;

uniform mat4 model;

void main(){
	gl_Position=project*view*model*vec4(aPos,1.0f);
	textureCoor =aTexCoords;

}
