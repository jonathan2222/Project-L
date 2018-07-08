#version 330
in vec2 fragUv;
flat in vec2 fragMinUv;
flat in vec2 fragMinUv2;
flat in vec2 fragMinUvLeft;
flat in vec2 fragMinUvRight;
flat in vec2 fragMinUvUp;
flat in vec2 fragMinUvDown;
flat in vec2 fragMinUvMask;
flat in vec4 fragMaskSide;  	// (left, right, up, down)
flat in uint fragRandomBits;  	// The first 28 bits make up the border. Begining at TL -> TR -> BR -> BL
flat in uint fragCorners; 		// 1: TL, 2: TR, 4: BR, 8: BL 

out vec4 finalColor;

uniform sampler2D tex;
uniform int tileSize;

float isBitSet(in uint mask, in uint bit)
{
	return float((mask >> bit) & 1u);
}

vec4 getColorFromBits(in vec4 baseColor, in vec4 baseColor2, in vec4 mask, in vec4 colorLeft, in vec4 colorRight, in vec4 colorUp, in vec4 colorDown, in uvec2 p)
{
	vec4 borderMask = vec4(1.0, 1.0, 1.0, 1.0);
	float isLeft = (1.0 - step(1.0, float(p.x))) * step(1.0, float(p.y)); // Can't be left and top
	float isRight = step(float(tileSize-1), float(p.x)) * (1.0 - step(float(tileSize-1), float(p.y))); // Can't be right and bottom
	float isTop = (1.0 - step(1.0, float(p.y))) * (1.0 - step(float(tileSize-1), float(p.x))); // Can't be top and right
	float isBottom = step(float(tileSize-1), p.y) * step(1.0, float(p.x)); // Can't be bottom and left

	uint bitPos = 0u;
	// Get the bit position depending on position in tile. If not on border, return 0.
	bitPos = uint(isTop)*p.x + 
			uint(isRight)*(p.y+uint(tileSize)-1u) + 
			uint(isBottom)*((uint(tileSize)-1u - p.x) + 
			uint(tileSize)*2u - 2u) + 
			uint(isLeft)*((uint(tileSize)-1u - p.y) + uint(tileSize)*3u - 3u);

	float isTLCorner = min((1.0 - step(2.0, float(bitPos))) + step(27, float(bitPos)), 1.0) * min(isLeft+isTop, 1.0) * float((fragCorners >> 0u) & 1u);
	float isTRCorner = step(6.0, float(bitPos))*(1.0 - step(9.0, float(bitPos))) * float((fragCorners >> 1u) & 1u);
	float isBRCorner = step(13.0, float(bitPos))*(1.0 - step(16.0, float(bitPos))) * float((fragCorners >> 2u) & 1u);
	float isBLCorner = step(20.0, float(bitPos))*(1.0 - step(23.0, float(bitPos))) * float((fragCorners >> 3u) & 1u);
	float isCorner = min(isTLCorner + isTRCorner + isBRCorner + isBLCorner, 1.0);
	isLeft *= fragMaskSide.x;
	isRight *= fragMaskSide.y;
	isTop *= fragMaskSide.z;
	isBottom *= fragMaskSide.w;
	float isBorder = max(isLeft, max(isRight, max(isTop, isBottom)));
	uint bits = fragRandomBits;
	borderMask.rgb *= 1.0 - isBorder*max(isBitSet(bits, bitPos), 0);

	float cornerMask = 1.0 - isCorner;
	vec4 fullMask = mask*borderMask;
	fullMask.rgb *= cornerMask;
	vec3 invMask = 1.0 - fullMask.rgb;
	vec4 color = vec4(baseColor.rgb*fullMask.rgb + baseColor2.rgb*(1.0-mask.rgb) + 
					colorLeft.rgb*invMask*isLeft +
					colorRight.rgb*invMask*isRight +
					colorUp.rgb*invMask*isTop + 
					colorDown.rgb*invMask*isBottom +
					colorLeft.rgb*(1.0-cornerMask)*isTLCorner + 
					colorRight.rgb*(1.0-cornerMask)*isTRCorner + 
					colorUp.rgb*(1.0-cornerMask)*isBRCorner + 
					colorDown.rgb*(1.0-cornerMask)*isBLCorner, fullMask.a*baseColor.a);
	return color;
}

void main()
{
	ivec2 iuv = ivec2(fragUv.x, fragUv.y);
	iuv.x = min(iuv.x, tileSize-1);
	iuv.y = min(iuv.y, tileSize-1);
	vec4  texColor = texelFetch(tex, ivec2(fragMinUv.x + iuv.x, fragMinUv.y + iuv.y), 0);
	vec4  texColor2 = texelFetch(tex, ivec2(fragMinUv2.x + iuv.x, fragMinUv2.y + iuv.y), 0);
	vec4  texColorMask = texelFetch(tex, ivec2(fragMinUvMask.x + iuv.x, fragMinUvMask.y + iuv.y), 0);
	vec4  texColorLeft = texelFetch(tex, ivec2(fragMinUvLeft.x + iuv.x, fragMinUvLeft.y + iuv.y), 0);
	vec4  texColorRight = texelFetch(tex, ivec2(fragMinUvRight.x + iuv.x, fragMinUvRight.y + iuv.y), 0);
	vec4  texColorUp = texelFetch(tex, ivec2(fragMinUvUp.x + iuv.x, fragMinUvUp.y + iuv.y), 0);
	vec4  texColorDown = texelFetch(tex, ivec2(fragMinUvDown.x + iuv.x, fragMinUvDown.y + iuv.y), 0);

	// Merge corner masks.
	finalColor = getColorFromBits(texColor, texColor2, texColorMask, texColorLeft, texColorRight, texColorUp, texColorDown, uvec2(iuv.x, iuv.y));
}