#version 330
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;

// Instanced
layout(location = 2) in vec2 translation;
layout(location = 3) in vec2 minUv;
layout(location = 4) in vec2 minUv2;
layout(location = 5) in vec2 minUvMask;

out vec2 fragUv;
flat out vec2 fragMinUv;
flat out vec2 fragMinUv2;
flat out vec2 fragMinUvMask;

uniform mat4 camera;
uniform mat3 transform;

void main()
{	
	fragUv = uv;
	fragMinUv = minUv;
	fragMinUv2 = minUv2;
	fragMinUvMask = minUvMask;
	gl_Position = camera*vec4(translation+(transform*vec3(position, 1.0)).xy, -1.0, 1.0);
}