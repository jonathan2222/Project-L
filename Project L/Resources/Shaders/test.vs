#version 330
in vec2 position;
in vec3 color;

uniform vec2 offset;
uniform vec2 scale;
uniform vec3 tint;

out vec3 fragColor;

void main()
{	
	fragColor = color * tint;	
	gl_Position = vec4(position*scale + offset, 0.0, 1.0);
}