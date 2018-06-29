#version 330

in vec2 fragUv;

out vec4 finalColor;

uniform sampler2D tex;
uniform vec3 tint;

void main()
{
	vec4 texColor = texture(tex, fragUv);
	finalColor = vec4(tint*texColor.rgb, texColor.a);
}