#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 texCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f); //its backwards, remember y'dumbo!
	TexCoord = vec2(texCoord.x, 1.0f - texCoord.y);
}