#version 330
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;

// Instanced
layout(location = 2) in vec2 translation;
layout(location = 3) in vec2 minUv;
layout(location = 4) in vec2 minUv2;
layout(location = 5) in vec2 minUvMask;
layout(location = 6) in vec2 minUvMask2;
layout(location = 7) in vec4 mask2Dir;

out vec2 fragUv;
flat out vec2 fragMinUv;
flat out vec2 fragMinUv2;
flat out vec2 fragMinUvMask;
flat out vec2 fragMinUvMask2;
flat out vec4 fragMask2Dir;

uniform mat4 camera;
uniform mat3 transform;

void main()
{	
	fragUv = uv;
	fragMinUv = minUv;
	fragMinUv2 = minUv2;
	fragMinUvMask = minUvMask;
	fragMinUvMask2 = minUvMask2;
	fragMask2Dir = mask2Dir;
	gl_Position = camera*vec4(translation+(transform*vec3(position, 1.0)).xy, -1.0, 1.0);
}