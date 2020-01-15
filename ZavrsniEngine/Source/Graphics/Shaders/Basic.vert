#version 330 core

layout (location = 0) in vec4 position;

uniform mat4 projection_matrix;
uniform mat4 vm_matrix = mat4(1.0);
uniform mat4 ml_matrix = mat4(1.0);

out DATA
{
	vec4 position;
	vec4 color;
} vs_out;

void main()
{
	gl_Position = position;
	vs_out.position = ml_matrix * position;
	vs_out.color = vec4(1.0,0.0,0.0,1.0);
}