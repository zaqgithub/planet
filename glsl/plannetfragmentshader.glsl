#version 460 core

in vec2 textureCoor;

out vec4 FragColor;

struct Material{
	sampler2D texture_diffuse1;
	sampler2D texture_normal1;
	sampler2D texture_specular1;
	float shiness;
};

uniform Material material;
void main(){

	FragColor = texture(material.texture_diffuse1,textureCoor);
}