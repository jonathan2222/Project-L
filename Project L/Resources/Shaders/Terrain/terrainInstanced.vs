#version 330
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;

// Instanced
layout(location = 2) in vec2 translation;
layout(location = 3) in vec2 minUv;

out vec2 fragUv;
flat out vec2 fragMinUv;

uniform mat4 camera;
uniform mat3 transform;

void main()
{	
	fragUv = uv;
	fragMinUv = minUv;
	gl_Position = camera*vec4(translation+(transform*vec3(position, 1.0)).xy, -1.0, 1.0);
}