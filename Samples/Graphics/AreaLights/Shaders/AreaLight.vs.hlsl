// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2019
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.25.0 (2019/04/15)

cbuffer PVWMatrix
{
    float4x4 pvwMatrix;
};

struct VS_INPUT
{
    float3 modelPosition : POSITION;
    float2 modelTCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 vertexPosition : TEXCOORD0;
    float2 vertexTCoord : TEXCOORD1;
    float4 clipPosition : SV_POSITION;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output;
    output.vertexPosition = float4(input.modelPosition, 1.0f);
    output.vertexTCoord = input.modelTCoord;
#if GTE_USE_MAT_VEC
    output.clipPosition = mul(pvwMatrix, output.vertexPosition);
#else
    output.clipPosition = mul(output.vertexPosition, pvwMatrix);
#endif
    return output;
}
