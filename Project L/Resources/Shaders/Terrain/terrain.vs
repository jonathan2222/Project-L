#version 330
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;

uniform mat3 matrix;

out vec2 fragUv;

void main()
{	
	fragUv = uv;
	gl_Position = vec4((matrix*vec3(position, 1.0)).xy, 0.0, 1.0);
}