// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2019
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.23.0 (2019/03/20)

#include "GpuGaussianBlur2Window.h"

static bool gsUseDirichlet;

int main(int numArguments, char const* arguments[])
{
#if defined(_DEBUG)
    LogReporter reporter(
        "LogReport.txt",
        Logger::Listener::LISTEN_FOR_ALL,
        Logger::Listener::LISTEN_FOR_ALL,
        Logger::Listener::LISTEN_FOR_ALL,
        Logger::Listener::LISTEN_FOR_ALL);
#endif

    Command command(numArguments, arguments);
    gsUseDirichlet = (command.GetBoolean("d") > 0 ? true : false);

    // The window size is that of the Head_U16_X256_Y256.binary image.
    Window::Parameters parameters(L"GpuGaussianBlur2Window", 0, 0, 256, 256);
    auto window = TheWindowSystem.Create<GpuGaussianBlur2Window>(parameters);
    TheWindowSystem.MessagePump(window, TheWindowSystem.DEFAULT_ACTION);
    TheWindowSystem.Destroy<GpuGaussianBlur2Window>(window);
    return 0;
}

GpuGaussianBlur2Window::GpuGaussianBlur2Window(Parameters& parameters)
    :
    Window3(parameters),
    mNumXThreads(8),
    mNumYThreads(8),
    mNumXGroups(mXSize / mNumXThreads),
    mNumYGroups(mYSize / mNumYThreads)
{
    if (!SetEnvironment() || !CreateImages() || !CreateShaders())
    {
        parameters.created = false;
        return;
    }

    // Create an overlay that covers the entire window.  The blurred image
    // is drawn by the overlay effect.
    mOverlay = std::make_shared<OverlayEffect>(mProgramFactory, mXSize,
        mYSize, mXSize, mYSize, SamplerState::MIN_P_MAG_P_MIP_P,
        SamplerState::CLAMP, SamplerState::CLAMP, false);
    mOverlay->SetTexture(mImage[0]);
}

void GpuGaussianBlur2Window::OnIdle()
{
    mTimer.Measure();

    mEngine->Execute(mGaussianBlurProgram, mNumXGroups, mNumYGroups, 1);
    if (gsUseDirichlet)
    {
        mEngine->Execute(mBoundaryDirichletProgram, mNumXGroups, mNumYGroups, 1);
    }
    else
    {
        mEngine->Execute(mBoundaryNeumannProgram, mNumXGroups, mNumYGroups, 1);
    }

    mEngine->Draw(mOverlay);

    mEngine->Draw(8, mYSize - 8, { 1.0f, 1.0f, 0.0f, 1.0f }, mTimer.GetFPS());
    mEngine->DisplayColorBuffer(0);

    mTimer.UpdateFrameCount();
}

bool GpuGaussianBlur2Window::SetEnvironment()
{
    std::string path = GetGTEPath();
    if (path == "")
    {
        return false;
    }

    mEnvironment.Insert(path + "/Samples/Imagics/GpuGaussianBlur2/Shaders/");
    mEnvironment.Insert(path + "/Samples/Data/");
    std::vector<std::string> inputs =
    {
        "Head_U16_X256_Y256.binary",
#if defined(GTE_DEV_OPENGL)
        "BoundaryDirichlet.glsl",
        "BoundaryNeumann.glsl",
        "GaussianBlur.glsl"
#else
        "BoundaryDirichlet.hlsl",
        "BoundaryNeumann.hlsl",
        "GaussianBlur.hlsl"
#endif
    };

    for (auto const& input : inputs)
    {
        if (mEnvironment.GetPath(input) == "")
        {
            LogError("Cannot find file " + input);
            return false;
        }
    }

    return true;
}

bool GpuGaussianBlur2Window::CreateImages()
{
    for (int i = 0; i < 2; ++i)
    {
        mImage[i] = std::make_shared<Texture2>(DF_R32_FLOAT, mXSize, mYSize);
        mImage[i]->SetUsage(Resource::SHADER_OUTPUT);
    }

    std::string path = mEnvironment.GetPath("Head_U16_X256_Y256.binary");
    if (path == "")
    {
        return false;
    }
    std::vector<uint16_t> original(mXSize * mYSize);
    std::ifstream input(path, std::ios::binary);
    input.read((char*)original.data(), original.size() * sizeof(uint16_t));
    input.close();

    // The head image is known to store 10 bits per pixel.  Scale the
    // texture image to have values in [0,1).
    float const divisor = 1024.0f;
    auto* target = mImage[0]->Get<float>();
    for (int i = 0; i < mXSize * mYSize; ++i)
    {
        target[i] = static_cast<float>(original[i]) / divisor;
    }

    // Create the mask texture for BoundaryDirichlet and the offset
    // texture for BoundaryNeumann.
    mMaskTexture = std::make_shared<Texture2>(DF_R32_FLOAT, mXSize, mYSize);
    auto* mask = mMaskTexture->Get<float>();
    mOffsetTexture = std::make_shared<Texture2>(DF_R32G32_SINT, mXSize, mYSize);
    auto* offset = mOffsetTexture->Get<std::array<int, 2>>();
    int xSizeM1 = mXSize - 1, ySizeM1 = mYSize - 1, index;

    // Interior.
    for (int y = 1; y < ySizeM1; ++y)
    {
        for (int x = 1; x < xSizeM1; ++x)
        {
            index = x + mXSize * y;
            mask[index] = 1.0f;
            offset[index] = { 0, 0 };
        }
    }

    // Edge-interior.
    for (int x = 1; x < xSizeM1; ++x)
    {
        mask[x] = 0.0f;
        offset[x] = { 0, 1 };
        index = x + mXSize * ySizeM1;
        mask[index] = 0.0f;
        offset[index] = { 0, -1 };
    }
    for (int y = 1; y < ySizeM1; ++y)
    {
        index = mXSize * y;
        mask[index] = 0.0f;
        offset[index] = { 1, 0 };
        index += xSizeM1;
        mask[index] = 0.0f;
        offset[index] = { -1, 0 };
    }

    // Corners.
    mask[0] = 0.0f;
    offset[0] = { 1, 1 };
    mask[xSizeM1] = 0.0f;
    offset[xSizeM1] = { -1, 1 };
    index = mXSize * ySizeM1;
    mask[index] = 0.0f;
    offset[index] = { 1, -1 };
    index += xSizeM1;
    mask[index] = 0.0f;
    offset[index] = { -1, -1 };

    mWeightBuffer = std::make_shared<ConstantBuffer>(sizeof(Vector4<float>), false);
    auto& weight = *mWeightBuffer->Get<Vector4<float>>();
    weight[0] = 0.01f;  // = kappa*DeltaT/DeltaX^2
    weight[1] = 0.01f;  // = kappa*DeltaT/DeltaY^2
    weight[2] = 1.0f - 2.0f * weight[0] - 2.0f * weight[1];  // positive value
    weight[3] = 0.0f;   // unused
    return true;
}

bool GpuGaussianBlur2Window::CreateShaders()
{
    mProgramFactory->defines.Set("NUM_X_THREADS", mNumXThreads);
    mProgramFactory->defines.Set("NUM_Y_THREADS", mNumYThreads);

    std::string ext;
#if defined(GTE_DEV_OPENGL)
    ext = ".glsl";
#else
    ext = ".hlsl";
#endif

    mGaussianBlurProgram = mProgramFactory->CreateFromFile(mEnvironment.GetPath("GaussianBlur" + ext));
    if (!mGaussianBlurProgram)
    {
        return false;
    }

    mBoundaryDirichletProgram = mProgramFactory->CreateFromFile(mEnvironment.GetPath("BoundaryDirichlet" + ext));
    if (!mBoundaryDirichletProgram)
    {
        return false;
    }

    mBoundaryNeumannProgram = mProgramFactory->CreateFromFile(mEnvironment.GetPath("BoundaryNeumann" + ext));
    if (!mBoundaryNeumannProgram)
    {
        return false;
    }

    auto cshader = mGaussianBlurProgram->GetCShader();
    cshader->Set("inImage", mImage[0]);
    cshader->Set("outImage", mImage[1]);
    cshader->Set("Weight", mWeightBuffer);

    cshader = mBoundaryDirichletProgram->GetCShader();
    cshader->Set("inImage", mImage[1]);
    cshader->Set("outImage", mImage[0]);
    cshader->Set("inMask", mMaskTexture);

    cshader = mBoundaryNeumannProgram->GetCShader();
    cshader->Set("inImage", mImage[1]);
    cshader->Set("outImage", mImage[0]);
    cshader->Set("inOffset", mOffsetTexture);

    return true;
}
