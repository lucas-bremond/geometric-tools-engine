// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2019
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.25.0 (2019/04/16)

struct PS_INPUT
{
    float3 emsAmbDifColor : COLOR;
    float3 spcColor : TEXCOORD0;
    float2 vertexTCoord : TEXCOORD1;
};

struct PS_OUTPUT
{
    float4 pixelColor : SV_TARGET0;
};

Texture2D<float4> baseTexture;
SamplerState baseSampler;

PS_OUTPUT PSMain(PS_INPUT input)
{
    PS_OUTPUT output;

    float4 baseColor = baseTexture.Sample(baseSampler, input.vertexTCoord);
    output.pixelColor.rgb = baseColor.rgb * input.emsAmbDifColor + baseColor.a * input.spcColor;
    output.pixelColor.a = 1.0f;

    return output;
}
