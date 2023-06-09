#version 330 core

layout(location = 0) in vec3 aPos; //posi/location 0 vec 3 3 coordinates 

uniform mat4 transform;

void main() {
	gl_Position = vec4(aPos, 1.0);
	//gl_Position = transform * vec4(aPos, 1.0);
}