#version 330

in vec2 fragUv;
flat in vec2 fragMinUv;
flat in vec2 fragMinUv2;
flat in vec2 fragMinUvMask;

out vec4 finalColor;

uniform sampler2D tex;
uniform int tileSize;

void main()
{
	ivec2 iuv = ivec2(fragUv.x, fragUv.y);
	iuv.x = min(iuv.x, tileSize-1);
	iuv.y = min(iuv.y, tileSize-1);
	vec4  texColor = texelFetch(tex, ivec2(fragMinUv.x + iuv.x, fragMinUv.y + iuv.y), 0);
	vec4  texColor2 = texelFetch(tex, ivec2(fragMinUv2.x + iuv.x, fragMinUv2.y + iuv.y), 0);
	vec4  texColorMask = texelFetch(tex, ivec2(fragMinUvMask.x + iuv.x, fragMinUvMask.y + iuv.y), 0);

	finalColor = vec4(texColor.rgb*texColorMask.rgb + texColor2.rgb*(1.0 - texColorMask.rgb), texColorMask.a);
}