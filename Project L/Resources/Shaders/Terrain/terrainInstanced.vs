#version 330
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec2 translation;

out vec2 fragUv;

uniform mat3 transform;

void main()
{	
	fragUv = uv;
	gl_Position = vec4(translation+(transform*vec3(position, 1.0)).xy, 0.0, 1.0);
}