#version 330

in vec2 fragUv;
flat in vec2 fragMinUv;
flat in vec2 fragMinUv2;
flat in vec2 fragMinUvMask;
flat in vec2 fragMinUvMask2;
flat in vec4 fragMask2Dir; // (tl, tr, bl, br)

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

	// Merge corner masks.
	vec4  mask2 = 1.0 - texelFetch(tex, ivec2(fragMinUvMask2.x + iuv.x, fragMinUvMask2.y + iuv.y), 0);
	float halfSize = tileSize/2;
	float a = step(halfSize, float(iuv.x));
	float b = step(halfSize, float(iuv.y));
	vec4  texColorMask2 = vec4(1, 1, 1, 1);
	if(a == 0.0 && b == 0.0)  // tl
	{
		texColorMask2.rgb *= mask2.rgb*fragMask2Dir.x + (1.0 - fragMask2Dir.x);
	}
	if(a == 1.0 && b == 0.0)  // tr
	{
		texColorMask2.rgb *= mask2.rgb*fragMask2Dir.y + (1.0 - fragMask2Dir.y);
	}
	if(a == 0.0 && b == 1.0)  // bl
	{
		texColorMask2.rgb *= mask2.rgb*fragMask2Dir.z + (1.0 - fragMask2Dir.z);
	}
	if(a == 1.0 && b == 1.0)  // br
	{
		texColorMask2.rgb *= mask2.rgb*fragMask2Dir.w + (1.0 - fragMask2Dir.w);
	}
	//vec4 cornor =	min(texColorMask2, mask2*(1.0 - a)*(1.0 - b)*fragMask2Dir.x + (1.0 - fragMask2Dir.x)) +  // tl
	//				min(texColorMask2, mask2*a*(1.0 - b)*fragMask2Dir.y + (1.0 - fragMask2Dir.y)) +  // tr
	//				min(texColorMask2, mask2*(1.0 - a)*b*fragMask2Dir.z + (1.0 - fragMask2Dir.z)) +  // bl
	//				min(texColorMask2, mask2*a*b*fragMask2Dir.w + (1.0 - fragMask2Dir.w)); // br
	//texColorMask2 = texColorMask2*(1.0 - corner);
	//texColorMask2 = clamp(texColorMask2, 0, 1);

	vec4 mask = texColorMask*texColorMask2;
	finalColor = vec4(texColor.rgb*mask.rgb + texColor2.rgb*(1.0 - mask.rgb), mask.a);
}