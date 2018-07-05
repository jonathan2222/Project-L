#version 330

in vec2 fragUv;
flat in vec2 fragMinUv;

out vec4 finalColor;

uniform sampler2D tex;

void main()
{
	//vec4 texColor = texture(tex, fragMinUv + fragUv);
	//vec4 texColor = textureOffset(tex, fragMinUv, ivec2(fragUv.x*8, fragUv.y*8));

	ivec2 iuv = ivec2(fragUv.x*16, fragUv.y*16);
	iuv.x = min(iuv.x, 15);
	iuv.y = min(iuv.y, 15);
	vec4  texColor = texelFetch(tex, ivec2(fragMinUv.x*1024 + iuv.x, fragMinUv.y*1024 + iuv.y), 0);
	finalColor = vec4(texColor.rgb, texColor.a);
}