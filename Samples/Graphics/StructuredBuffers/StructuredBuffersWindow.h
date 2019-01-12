// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2019
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.0 (2016/06/19)

#pragma once

#include <GTEngine.h>
using namespace gte;

class StructuredBuffersWindow : public Window3
{
public:
    StructuredBuffersWindow(Parameters& parameters);

    virtual void OnIdle() override;

private:
    bool SetEnvironment();
    bool CreateScene();

    std::shared_ptr<Visual> mSquare;
    std::shared_ptr<StructuredBuffer> mDrawnPixels;
    std::shared_ptr<Texture2> mDrawnPixelsTexture;
};
