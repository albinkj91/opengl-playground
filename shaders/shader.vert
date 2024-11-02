#version 330
in vec4 position;
in vec4 color;
in vec4 textureCoord;

out vec4 colorOut;
out vec2 textOut;

uniform mat4 mat_mvp;

void main()
{
	gl_Position = mat_mvp * position;
	colorOut = color;
	textOut = textureCoord.xy;
}
