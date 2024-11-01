#version 330
in vec4 colorOut;
in vec2 textOut;

out vec4 outputColor;
uniform sampler2D sampleTex;

void main()
{
   outputColor = colorOut * texture(sampleTex, textOut);
}
