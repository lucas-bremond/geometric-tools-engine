// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2019
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.0 (2016/06/19)

uniform sampler2D mySampler;

layout(location = 0) in vec2 vertexTCoord;
layout(location = 0) out vec4 pixelColor;

void main()
{
    vec4 color = texture(mySampler, vertexTCoord);
    if (color.z > 0.0f)
    {
        pixelColor = vec4(0.0f, color.z, 0.0f, 1.0f);
    }
    else
    {
        pixelColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    }
};
