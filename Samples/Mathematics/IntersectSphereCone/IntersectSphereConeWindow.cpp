// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2019
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.22.2 (2019/06/22)

#include "IntersectSphereConeWindow.h"
#include <LowLevel/GteLogReporter.h>
#include <Graphics/GteMeshFactory.h>

int main(int, char const*[])
{
#if defined(_DEBUG)
    LogReporter reporter(
        "LogReport.txt",
        Logger::Listener::LISTEN_FOR_ALL,
        Logger::Listener::LISTEN_FOR_ALL,
        Logger::Listener::LISTEN_FOR_ALL,
        Logger::Listener::LISTEN_FOR_ALL);
#endif

    Window::Parameters parameters(L"IntersectSphereConeWindow", 0, 0, 512, 512);
    auto window = TheWindowSystem.Create<IntersectSphereConeWindow>(parameters);
    TheWindowSystem.MessagePump(window, TheWindowSystem.DEFAULT_ACTION);
    TheWindowSystem.Destroy<IntersectSphereConeWindow>(window);
    return 0;
}

IntersectSphereConeWindow::IntersectSphereConeWindow(Parameters& parameters)
    :
    Window3(parameters),
    mAlpha(1.0f)
{
    mSphere.center = { 1.0f, 2.0f, 3.0f };
    mSphere.radius = 1.0f;

    mCone.ray.origin = { 0.0f, 0.0f, 0.0f };
    mCone.ray.direction = { 0.0f, 0.0f, 1.0f };
    mCone.SetAngle(0.25f);
    mCone.MakeConeFrustum(4.0f, 16.0f);

    mNoCullState = std::make_shared<RasterizerState>();
    mNoCullState->cullMode = RasterizerState::CULL_NONE;
    mEngine->SetRasterizerState(mNoCullState);

    mNoCullWireState = std::make_shared<RasterizerState>();
    mNoCullWireState->cullMode = RasterizerState::CULL_NONE;
    mNoCullWireState->fillMode = RasterizerState::FILL_WIREFRAME;

    mBlendState = std::make_shared<BlendState>();
    mBlendState->target[0].enable = true;
    mBlendState->target[0].srcColor = BlendState::BM_SRC_ALPHA;
    mBlendState->target[0].dstColor = BlendState::BM_INV_SRC_ALPHA;
    mBlendState->target[0].srcAlpha = BlendState::BM_SRC_ALPHA;
    mBlendState->target[0].dstAlpha = BlendState::BM_INV_SRC_ALPHA;
    mEngine->SetBlendState(mBlendState);

    CreateScene();
    InitializeCamera(60.0f, GetAspectRatio(), 1.0f, 5000.0f, 0.1f, 0.01f,
        { 0.0f, 0.0f, -24.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });

    TestIntersection();
}

void IntersectSphereConeWindow::OnIdle()
{
    mTimer.Measure();

    if (mCameraRig.Move())
    {
        mPVWMatrices.Update();
    }

    mEngine->ClearBuffers();
    mEngine->Draw(mConeMesh);
    mEngine->Draw(mDiskMinMesh);
    mEngine->Draw(mDiskMaxMesh);
    mEngine->Draw(mSphereMesh);

    mEngine->Draw(8, mYSize - 8, { 0.0f, 0.0f, 0.0f, 1.0f }, mTimer.GetFPS());
    mEngine->DisplayColorBuffer(0);

    mTimer.UpdateFrameCount();
}

bool IntersectSphereConeWindow::OnCharPress(unsigned char key, int x, int y)
{
    float const trnDelta = 0.1f;
    float const rotDelta = 0.01f;

    switch (key)
    {
    case 'w':
    case 'W':
        if (mEngine->GetRasterizerState() == mNoCullState)
        {
            mEngine->SetRasterizerState(mNoCullWireState);
        }
        else
        {
            mEngine->SetRasterizerState(mNoCullState);
        }
        return true;

    case ' ':
        TestIntersection();
        return true;

    case 'x':  // decrement x-center of box
        Translate(0, -trnDelta);
        return true;

    case 'X':  // increment x-center of box
        Translate(0, +trnDelta);
        return true;

    case 'y':  // decrement y-center of box
        Translate(1, -trnDelta);
        return true;

    case 'Y':  // increment y-center of box
        Translate(1, +trnDelta);
        return true;

    case 'z':  // decrement z-center of box
        Translate(2, -trnDelta);
        return true;

    case 'Z':  // increment z-center of box
        Translate(2, +trnDelta);
        return true;

    case 'a':  // incremental rotate about (1,0,0)
        Rotate(0, -rotDelta);
        return true;

    case 'A':  // incremental rotate about (1,0,0)
        Rotate(0, +rotDelta);
        return true;

    case 'b':  // incremental rotate about (0,1,0)
        Rotate(1, -rotDelta);
        return true;

    case 'B':  // incremental rotate about (0,1,0)
        Rotate(1, +rotDelta);
        return true;
    }

    return Window3::OnCharPress(key, x, y);
}

void IntersectSphereConeWindow::CreateScene()
{
    mBlueEffect = std::make_shared<ConstantColorEffect>(mProgramFactory,
        Vector4<float>({ 0.0f, 0.0f, 1.0f, mAlpha }));

    mCyanEffect = std::make_shared<ConstantColorEffect>(mProgramFactory,
        Vector4<float>({ 0.0f, 1.0f, 1.0f, mAlpha }));

    mRedEffect = std::make_shared<ConstantColorEffect>(mProgramFactory,
        Vector4<float>({ 1.0f, 0.0f, 0.0f, mAlpha }));

    for (int i = 0; i < 2; ++i)
    {
        mGreenEffect[i] = std::make_shared<ConstantColorEffect>(mProgramFactory,
            Vector4<float>({ 0.0f, 1.0f, 0.0f, mAlpha }));

        mYellowEffect[i] = std::make_shared<ConstantColorEffect>(mProgramFactory,
            Vector4<float>({ 1.0f, 1.0f, 0.0f, mAlpha }));
    }

    // Create a visual representation of the cone with heights in [4,16].
    VertexFormat vformat;
    vformat.Bind(VA_POSITION, DF_R32G32B32_FLOAT, 0);
    MeshFactory mf;
    mf.SetVertexFormat(vformat);
    unsigned int const numAxial = 16;
    unsigned int const numRadial = 16;
    mConeMesh = mf.CreateCylinderOpen(numAxial, numRadial, 1.0f, 1.0f);
    mConeMesh->localTransform.SetTranslation(mCone.ray.origin);
    auto vbuffer = mConeMesh->GetVertexBuffer();
    auto vertices = vbuffer->Get<Vector3<float>>();
    for (unsigned int row = 0, i = 0; row < numAxial; ++row)
    {
        float const height = mCone.GetMinHeight() + (mCone.GetMaxHeight() - mCone.GetMinHeight())
            * static_cast<float>(row) / static_cast<float>(numAxial - 1);
        float const radius = height * mCone.tanAngle;
        for (unsigned int col = 0; col <= numRadial; ++col, ++i)
        {
            Vector3<float>& P = vertices[i];
            float stretch = radius / std::sqrt(P[0] * P[0] + P[1] * P[1]);
            P[0] *= stretch;
            P[1] *= stretch;
            P[2] = height;
        }
    }

    mConeMesh->SetEffect(mBlueEffect);
    mPVWMatrices.Subscribe(mConeMesh->worldTransform, mBlueEffect->GetPVWMatrixConstant());

    // Create visual representations of the disk caps for the cone.
    mDiskMinMesh = mf.CreateDisk(16, 16, mCone.GetMinHeight() * mCone.tanAngle);
    mDiskMinMesh->localTransform.SetTranslation(mCone.ray.origin + mCone.GetMinHeight() * mCone.ray.direction);
    mDiskMinMesh->Update();
    mDiskMinMesh->SetEffect(mGreenEffect[0]);
    mPVWMatrices.Subscribe(mDiskMinMesh->worldTransform, mGreenEffect[0]->GetPVWMatrixConstant());

    mDiskMaxMesh = mf.CreateDisk(16, 16, mCone.GetMaxHeight() * mCone.tanAngle);
    mDiskMaxMesh->localTransform.SetTranslation(mCone.ray.origin + mCone.GetMaxHeight() * mCone.ray.direction);
    mDiskMaxMesh->Update();
    mDiskMaxMesh->SetEffect(mGreenEffect[1]);
    mPVWMatrices.Subscribe(mDiskMaxMesh->worldTransform, mGreenEffect[1]->GetPVWMatrixConstant());

    mSphereMesh = mf.CreateSphere(numAxial, numRadial, mSphere.radius);
    mSphereMesh->SetEffect(mRedEffect);
    mSphereMesh->localTransform.SetTranslation(mSphere.center);
    mPVWMatrices.Subscribe(mSphereMesh->worldTransform, mRedEffect->GetPVWMatrixConstant());

    mTrackball.Attach(mConeMesh);
    mTrackball.Attach(mDiskMinMesh);
    mTrackball.Attach(mDiskMaxMesh);
    mTrackball.Attach(mSphereMesh);
    mTrackball.Update();
}

void IntersectSphereConeWindow::Translate(int direction, float delta)
{
    mCone.ray.origin[direction] += delta;
    mConeMesh->localTransform.SetTranslation(mCone.ray.origin);
    mDiskMinMesh->localTransform.SetTranslation(mCone.ray.origin + mCone.GetMinHeight() * mCone.ray.direction);
    mDiskMaxMesh->localTransform.SetTranslation(mCone.ray.origin + mCone.GetMaxHeight() * mCone.ray.direction);
    mTrackball.Update();
    TestIntersection();
}

void IntersectSphereConeWindow::Rotate(int direction, float delta)
{
    Quaternion<float> incr = Rotation<3, float>(
        AxisAngle<3, float>(Vector3<float>::Unit(direction), delta));

    Quaternion<float> q;
    mConeMesh->localTransform.GetRotation(q);
    Quaternion<float> qnext = incr * q;

    mConeMesh->localTransform.SetRotation(qnext);
    mDiskMinMesh->localTransform.SetRotation(qnext);
    mDiskMaxMesh->localTransform.SetRotation(qnext);

    Matrix4x4<float> rot = mConeMesh->localTransform.GetRotation();
    mCone.ray.direction = HProject(rot.GetCol(2));

    mDiskMinMesh->localTransform.SetTranslation(mCone.ray.origin + mCone.GetMinHeight() * mCone.ray.direction);
    mDiskMaxMesh->localTransform.SetTranslation(mCone.ray.origin + mCone.GetMaxHeight() * mCone.ray.direction);

    mTrackball.Update();
    TestIntersection();
}

void IntersectSphereConeWindow::TestIntersection()
{
    mPVWMatrices.Unsubscribe(mConeMesh->worldTransform);
    mPVWMatrices.Unsubscribe(mDiskMinMesh->worldTransform);
    mPVWMatrices.Unsubscribe(mDiskMaxMesh->worldTransform);

    if (mQuery(mSphere, mCone).intersect)
    {
        mConeMesh->SetEffect(mCyanEffect);
        mDiskMinMesh->SetEffect(mYellowEffect[0]);
        mDiskMaxMesh->SetEffect(mYellowEffect[1]);
        mPVWMatrices.Subscribe(mConeMesh->worldTransform, mCyanEffect->GetPVWMatrixConstant());
        mPVWMatrices.Subscribe(mDiskMinMesh->worldTransform, mYellowEffect[0]->GetPVWMatrixConstant());
        mPVWMatrices.Subscribe(mDiskMaxMesh->worldTransform, mYellowEffect[1]->GetPVWMatrixConstant());
    }
    else
    {
        mConeMesh->SetEffect(mBlueEffect);
        mDiskMinMesh->SetEffect(mGreenEffect[0]);
        mDiskMaxMesh->SetEffect(mGreenEffect[1]);
        mPVWMatrices.Subscribe(mConeMesh->worldTransform, mBlueEffect->GetPVWMatrixConstant());
        mPVWMatrices.Subscribe(mDiskMinMesh->worldTransform, mGreenEffect[0]->GetPVWMatrixConstant());
        mPVWMatrices.Subscribe(mDiskMaxMesh->worldTransform, mGreenEffect[1]->GetPVWMatrixConstant());
    }

    mPVWMatrices.Update();
}
