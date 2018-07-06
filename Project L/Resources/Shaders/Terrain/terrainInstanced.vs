#version 330
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;

// Instanced
layout(location = 2) in vec2 translation;
layout(location = 3) in vec2 minUv;
layout(location = 4) in vec2 minUv2;
layout(location = 5) in vec2 minUvLeft;
layout(location = 6) in vec2 minUvRight;
layout(location = 7) in vec2 minUvUp;
layout(location = 8) in vec2 minUvDown;
layout(location = 9) in vec2 minUvMask;
layout(location = 10) in vec4 maskSide;
layout(location = 11) in uint randomBits;

out vec2 fragUv;
flat out vec2 fragMinUv;
flat out vec2 fragMinUv2;
flat out vec2 fragMinUvLeft;
flat out vec2 fragMinUvRight;
flat out vec2 fragMinUvUp;
flat out vec2 fragMinUvDown;
flat out vec2 fragMinUvMask;
flat out vec4 fragMaskSide;
flat out uint fragRandomBits;

uniform mat4 camera;
uniform mat3 transform;

void main()
{	
	fragUv = uv;
	fragMinUv = minUv;
	fragMinUv2 = minUv2;
	fragMinUvLeft = minUvLeft;
	fragMinUvRight = minUvRight;
	fragMinUvUp = minUvUp;
	fragMinUvDown = minUvDown;
	fragMinUvMask = minUvMask;
	fragMaskSide = maskSide;
	fragRandomBits = randomBits;
	gl_Position = camera*vec4(translation+(transform*vec3(position, 1.0)).xy, -1.0, 1.0);
}