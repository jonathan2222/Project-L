#version 330

in vec2 fragUv;

out vec4 finalColor;

uniform sampler2D tex;

void main()
{
	vec4 texColor = texture(tex, fragUv);
	finalColor = vec4(texColor.rgb, texColor.a);
}