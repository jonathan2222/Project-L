#version 330
in vec2 fragUv;
in vec2 fragScale;

uniform sampler2D tex;
uniform vec3 tint;

out vec4 finalColor;

void main()
{
	vec2 scale = fragScale / min(fragScale.x, fragScale.y);
	vec4 texColor = texture(tex, fragUv*scale);
	texColor.rgb *= tint;
	finalColor = texColor;
}
