// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2019
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.25.0 (2019/04/17)

struct PS_INPUT
{
    float2 vertexBaseTCoord : TEXCOORD0;
    float2 vertexDetailTCoord : TEXCOORD1;
    float4 vertexFogInfo : TEXCOORD2;
};

struct PS_OUTPUT
{
    float4 pixelColor : SV_TARGET0;
};

Texture2D<float4> baseTexture;
Texture2D<float4> detailTexture;
SamplerState baseSampler;
SamplerState detailSampler;

PS_OUTPUT PSMain(PS_INPUT input)
{
    PS_OUTPUT output;

    // Sample the texture images and multiply the results.
    float3 baseColor = baseTexture.Sample(baseSampler, input.vertexBaseTCoord).xyz;
    float3 detailColor = detailTexture.Sample(detailSampler, input.vertexDetailTCoord).xyz;
    float3 product = baseColor * detailColor;

    // Combine the base*detail color with the fog color.
    output.pixelColor.rgb = lerp(input.vertexFogInfo.rgb, product, input.vertexFogInfo.w);
    output.pixelColor.a = 1.0f;
    return output;
}
