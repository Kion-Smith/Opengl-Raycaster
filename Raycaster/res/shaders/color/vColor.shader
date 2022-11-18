#version 330 core
layout(location = 0) in vec3 pos;

uniform mat4 mvp;
uniform vec3 userColor;

out vec4 vertexColor;

void main()
{
	gl_Position = mvp * vec4(pos, 1.0f);
	vertexColor = vec4(userColor, 1.0f);
}