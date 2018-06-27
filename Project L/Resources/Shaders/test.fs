#version 330

in vec3 fragColor;

out vec4 finalColor;

void main()
{
	finalColor = vec4(min(fragColor, vec3(1.0, 1.0, 1.0)), 1.0);
}