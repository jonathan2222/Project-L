#version 330

in vec2 fragUv;

out vec4 finalColor;

uniform sampler2D myTexture;
uniform vec3 tint;

void main()
{
	vec4 texColor = texture(myTexture, fragUv);
	finalColor = vec4(tint*texColor.rgb, 1.0);
}