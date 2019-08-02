// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2019
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.1 (2019/05/01)

uniform PVWMatrix
{
    mat4 pvwMatrix;
};

const vec4 color[4] =
{
    vec4(0.0f, 0.0f, 0.5f, 1.0f),
    vec4(0.5f, 0.0f, 0.0f, 1.0f),
    vec4(0.0f, 0.5f, 0.5f, 1.0f),
    vec4(0.5f, 0.5f, 0.0f, 1.0f)
};

layout(location = 0) in vec4 modelPositionColorIndex;
layout(location = 0) out vec4 vertexColor;

void main()
{
    vec4 modelPosition = vec4(modelPositionColorIndex.xyz, 1.0f);
#if GTE_USE_MAT_VEC
    gl_Position = pvwMatrix * modelPosition;
#else
    gl_Position = modelPosition * pvwMatrix;
#endif
    uint i = uint(modelPositionColorIndex.w);
    vertexColor = color[i];
}
