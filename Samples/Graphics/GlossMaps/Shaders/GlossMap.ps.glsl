// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2019
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.25.0 (2019/04/16)

layout(location = 0) in vec3 emsAmbDifColor;
layout(location = 1) in vec3 spcColor;
layout(location = 2) in vec2 vertexTCoord;

layout(location = 0) out vec4 pixelColor;

uniform sampler2D baseSampler;

void main()
{
    vec4 baseColor = texture(baseSampler, vertexTCoord);
    pixelColor.rgb = baseColor.rgb * emsAmbDifColor + baseColor.a * spcColor;
    pixelColor.a = 1.0f;
}
