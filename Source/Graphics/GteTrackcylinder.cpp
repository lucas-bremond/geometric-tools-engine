// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2019
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.15.2 (2019/03/18)

#include <GTEnginePCH.h>
#include <Mathematics/GteMath.h>
#include <Graphics/GteTrackcylinder.h>
using namespace gte;

Trackcylinder::Trackcylinder()
    :
    TrackObject(),
    mInitialYaw(0.0f),
    mYaw(0.0f),
    mInitialPitch(0.0f),
    mPitch(0.0f)
{
    mRoot = std::make_shared<Node>();
}

Trackcylinder::Trackcylinder(int xSize, int ySize, std::shared_ptr<Camera> const& camera)
    :
    TrackObject(xSize, ySize, camera),
    mInitialYaw(0.0f),
    mYaw(0.0f),
    mInitialPitch(0.0f),
    mPitch(0.0f)
{
    Set(xSize, ySize, camera);
    mRoot = std::make_shared<Node>();
}

void Trackcylinder::Reset()
{
    mInitialYaw = 0.0f;
    mInitialPitch = 0.0f;
    mYaw = 0.0f;
    mPitch = 0.0f;
    mRoot->localTransform.MakeIdentity();
    mRoot->Update();
}

void Trackcylinder::OnSetInitialPoint()
{
    mInitialYaw = mYaw;
    mInitialPitch = mPitch;
}

void Trackcylinder::OnSetFinalPoint()
{
    float const pi = (float)GTE_C_PI;
    float const halfPi = (float)GTE_C_HALF_PI;
    float dx = mX1 - mX0;
    float dy = mY1 - mY0;
    float angle = dx * pi;
    mYaw = mInitialYaw + angle;
    angle = -dy * pi;
    mPitch = mInitialPitch + angle;
    mPitch = gte::clamp(mPitch, -halfPi, halfPi);

    // The angle order depends on camera {D=0, U=1, R=2}.
#if defined(GTE_USE_MAT_VEC)
    AxisAngle<4, float> yawAxisAngle(Vector4<float>::Unit(2), mYaw);
    Matrix4x4<float> yawRotate = Rotation<4, float>(yawAxisAngle);
    AxisAngle<4, float> pitchAxisAngle(Vector4<float>::Unit(1), mPitch);
    Matrix4x4<float> pitchRotate = Rotation<4, float>(pitchAxisAngle);
    Matrix4x4<float> rotate = pitchRotate * yawRotate;
#else
    AxisAngle<4, float> yawAxisAngle(Vector4<float>::Unit(2), -mYaw);
    Matrix4x4<float> yawRotate = Rotation<4, float>(yawAxisAngle);
    AxisAngle<4, float> pitchAxisAngle(Vector4<float>::Unit(1), -mPitch);
    Matrix4x4<float> pitchRotate = Rotation<4, float>(pitchAxisAngle);
    Matrix4x4<float> rotate = yawRotate * pitchRotate;
#endif

    NormalizeAndUpdateRoot(rotate);
}
