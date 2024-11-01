#version 330
in vec4 position;
in vec4 color;
in vec4 textureCoord;

out vec4 colorOut;
out vec2 textOut;

void main()
{
	gl_Position = position;
	colorOut = color;
	textOut = textureCoord.xy;
}
