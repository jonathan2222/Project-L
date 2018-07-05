#version 330

in vec2 fragUv;
flat in vec2 fragMinUv;

out vec4 finalColor;

uniform sampler2D tex;
uniform int tileSize;

void main()
{
	ivec2 iuv = ivec2(fragUv.x, fragUv.y);
	iuv.x = min(iuv.x, tileSize-1);
	iuv.y = min(iuv.y, tileSize-1);
	vec4  texColor = texelFetch(tex, ivec2(fragMinUv.x + iuv.x, fragMinUv.y + iuv.y), 0);
	finalColor = vec4(texColor.rgb, texColor.a);
}