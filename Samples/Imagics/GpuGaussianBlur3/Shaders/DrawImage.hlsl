// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2019
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.23.0 (2019/03/21)

Texture2D<float> inImage;
Texture2D<float> inMask;
SamplerState nearestSampler;

struct PS_INPUT
{
    float2 vertexTCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 pixelColor0 : SV_TARGET0;
};

static float4 boundaryColor = { 0.0f, 1.0f, 0.0f, 1.0f };

PS_OUTPUT PSMain(PS_INPUT input)
{
    PS_OUTPUT output;
    float image = inImage.Sample(nearestSampler, input.vertexTCoord);
    float4 interiorColor = { image, image, image, 1.0f };
    float mask = inMask.Sample(nearestSampler, input.vertexTCoord);
    output.pixelColor0 = mask * interiorColor + (1.0f - mask) * boundaryColor;
    return output;
};
