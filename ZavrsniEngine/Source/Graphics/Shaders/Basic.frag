#version 330 core

layout (location = 0) out vec4 color;

in DATA
{
	vec4 position;
	vec4 color;
	float textureSlot;
	vec2 textureCoordinates;
} fs_in;

uniform sampler2D textures[32];

void main()
{	
	if(fs_in.textureSlot >= 0.0f)
	{
	int textureSlot = int(fs_in.textureSlot);
	color  = fs_in.color * texture(textures[textureSlot], fs_in.textureCoordinates);
	}
	else 
	color = fs_in.color;
}
