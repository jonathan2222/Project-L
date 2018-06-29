#version 330
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;

// Instanced
layout(location = 2) in vec2 translation;
layout(location = 3) in vec2 minUv;

out vec2 fragUv;
out vec2 fragMinUv;

uniform mat3 transform;
uniform float uvScale;

void main()
{	
	fragUv = uv*uvScale;
	fragMinUv = minUv;
	gl_Position = vec4(translation+(transform*vec3(position, 1.0)).xy, 0.0, 1.0);
}