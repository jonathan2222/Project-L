#version 330
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;

uniform mat4 camera;
uniform mat3 transform;

out vec2 fragUv;
out vec2 fragScale;

void main()
{	
	fragUv = uv;
	fragScale = vec2(transform[0][0], transform[1][1]);
	gl_Position = camera*vec4((transform*vec3(position, 1.0)).xy, -1.0, 1.0);
}