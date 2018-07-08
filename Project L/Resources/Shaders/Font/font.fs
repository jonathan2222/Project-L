#version 330

in vec2 texcoord;

uniform sampler2D tex;
uniform vec4 color;

out vec4 finalColor;

void main()
{
	finalColor = vec4(1.0, 1.0, 1.0, texture(tex, texcoord).r) * color;
}