#version 330
in vec2 position;
in vec2 uv;
in vec2 offset; // Per instance

uniform vec2 scale;

out vec2 fragUv;

void main()
{	
	fragUv = uv;	
	gl_Position = vec4(position*scale + offset, 0.0, 1.0);
}