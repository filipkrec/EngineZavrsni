#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in float textureSlot;
layout (location = 2) in vec2 textureCoordinates;
layout (location = 3) in vec4 color;

uniform mat4 projection_matrix; // camera matrix 
uniform mat4 view_matrix = mat4(1.0); // world matrix
uniform mat4 model_matrix = mat4(1.0); // object matrix

out DATA
{
	vec4 position;
	vec4 color;
	float textureSlot;
	vec2 textureCoordinates;
} vs_out;


void main()
{
	gl_Position = position * projection_matrix * view_matrix * model_matrix; //pozicija u svijetu
	vs_out.position = model_matrix * position; //pozicija za lightning
	vs_out.color = color;
	vs_out.textureSlot = textureSlot;
	vs_out.textureCoordinates = textureCoordinates;
}