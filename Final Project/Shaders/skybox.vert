#version 330 core

layout(location = 0) in vec3 aPos;

//vec2 = 2d
//vec3 = cubemaps
out vec3 texCoord;

uniform mat4 projection;
uniform mat4 view;

void main() {
	vec4 pos = projection * view * vec4(aPos, 1.0);

	//write directly view / NDC
	gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);

	texCoord = aPos;
}