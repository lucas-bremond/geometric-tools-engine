// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2019
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.23.0 (2019/03/18)

#include "ConformalMappingWindow.h"

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

    Window::Parameters parameters(L"ConformalMappingWindow", 0, 0, 768, 768);
    auto window = TheWindowSystem.Create<ConformalMappingWindow>(parameters);
    TheWindowSystem.MessagePump(window, TheWindowSystem.DEFAULT_ACTION);
    TheWindowSystem.Destroy<ConformalMappingWindow>(window);
    return 0;
}

ConformalMappingWindow::ConformalMappingWindow(Parameters& parameters)
    :
    Window3(parameters),
    mExtreme(10.0f)
{
    if (!SetEnvironment())
    {
        parameters.created = false;
        return;
    }

    mEngine->SetClearColor({ 0.4f, 0.5f, 0.6f, 1.0f });
    mWireState = std::make_shared<RasterizerState>();
    mWireState->fillMode = RasterizerState::FILL_WIREFRAME;

    InitializeCamera(60.0f, GetAspectRatio(), 0.1f, 100.0f, 0.01f, 0.01f,
        { 0.0f, 0.0f, -6.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });

    CreateScene();

    mPVWMatrices.Update();
}

void ConformalMappingWindow::OnIdle()
{
    mTimer.Measure();

    if (mCameraRig.Move())
    {
        mPVWMatrices.Update();
    }

    mEngine->ClearBuffers();
    mEngine->Draw(mMesh);
    mEngine->Draw(mSphere);
    mEngine->Draw(8, mYSize - 8, { 0.0f, 0.0f, 0.0f, 1.0f }, mTimer.GetFPS());
    mEngine->DisplayColorBuffer(0);

    mTimer.UpdateFrameCount();
}

bool ConformalMappingWindow::OnCharPress(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
        if (mEngine->GetRasterizerState() == mWireState)
        {
            mEngine->SetDefaultRasterizerState();
        }
        else
        {
            mEngine->SetRasterizerState(mWireState);
        }
        return true;
    case 'm':
        // Rotate only the brain mesh.
        mTrackball.Set(mMeshNode);
        mTrackball.Update();
        return true;
    case 's':
        // Rotate only the sphere mesh.
        mTrackball.Set(mSphereNode);
        mTrackball.Update();
        return true;
    case 'b':
        // Rotate both the brain and sphere meshes simultaneously.
        mTrackball.Set(mScene);
        mTrackball.Update();
        return true;
    }

    return Window3::OnCharPress(key, x, y);
}

bool ConformalMappingWindow::SetEnvironment()
{
    std::string path = GetGTEPath();
    if (path == "")
    {
        return false;
    }

    mEnvironment.Insert(path + "/Samples/Data/");
    if (mEnvironment.GetPath("Brain_V4098_T8192.binary") == "")
    {
        LogError("Cannot find file Brain_V4098_T8192.binary");
        return false;
    }
    return true;
}

void ConformalMappingWindow::LoadBrain(std::vector<Vector3<float>>& positions,
    std::vector<Vector4<float>>& colors, std::vector<unsigned int>& indices)
{
    // Load the brain mesh, which has the topology of a sphere.
    unsigned int const numPositions = NUM_BRAIN_VERTICES;
    unsigned int const numTriangles = NUM_BRAIN_TRIANGLES;
    positions.resize(numPositions);
    colors.resize(numPositions);
    indices.resize(3 * numTriangles);

    std::string path = mEnvironment.GetPath("Brain_V4098_T8192.binary");
    std::ifstream input(path, std::ios::binary);
    input.read((char*)positions.data(), positions.size() * sizeof(positions[0]));
    input.read((char*)indices.data(), indices.size() * sizeof(indices[0]));
    input.close();

    // Scale the data to the cube [-10,10]^3 for numerical preconditioning
    // of the conformal mapping.
    float minValue = positions[0][0], maxValue = minValue;
    for (unsigned int i = 0; i < numPositions; ++i)
    {
        auto const& position = positions[i];
        for (int j = 0; j < 3; ++j)
        {
            if (position[j] < minValue)
            {
                minValue = position[j];
            }
            else if (position[j] > maxValue)
            {
                maxValue = position[j];
            }
        }
    }
    float halfRange = 0.5f * (maxValue - minValue);
    float mult = mExtreme / halfRange;
    for (unsigned int i = 0; i < numPositions; ++i)
    {
        auto& position = positions[i];
        for (int j = 0; j < 3; ++j)
        {
            position[j] = -mExtreme + mult * (position[j] - minValue);
        }
    }

    // Assign vertex colors according to mean curvature.
    MeshCurvature<float>mc;
    mc(positions, indices, 1e-06f);
    auto const& minCurvatures = mc.GetMinCurvatures();
    auto const& maxCurvatures = mc.GetMaxCurvatures();
    std::vector<float> meanCurvatures(numPositions);
    float minMeanCurvature = minCurvatures[0] + maxCurvatures[0];
    float maxMeanCurvature = minMeanCurvature;
    for (unsigned int i = 0; i < numPositions; ++i)
    {
        meanCurvatures[i] = minCurvatures[i] + maxCurvatures[i];
        if (meanCurvatures[i] < minMeanCurvature)
        {
            minMeanCurvature = meanCurvatures[i];
        }
        else if (meanCurvatures[i] > maxMeanCurvature)
        {
            maxMeanCurvature = meanCurvatures[i];
        }
    }

    for (unsigned int i = 0; i < numPositions; ++i)
    {
        auto& color = colors[i];
        if (meanCurvatures[i] > 0.0f)
        {
            color[0] = 0.5f * (1.0f + meanCurvatures[i] / maxMeanCurvature);
            color[1] = color[0];
            color[2] = 0.0f;
        }
        else if (meanCurvatures[i] < 0.0f)
        {
            color[0] = 0.0f;
            color[1] = 0.0f;
            color[2] = 0.5f * (1.0f - meanCurvatures[i] / minMeanCurvature);
        }
        else
        {
            color[0] = 0.0f;
            color[1] = 0.0f;
            color[2] = 0.0f;
        }
        color[3] = 1.0f;
    }
}

void ConformalMappingWindow::CreateScene()
{
    // Load and preprocess the brain data set.
    std::vector<Vector3<float>> positions;
    std::vector<Vector4<float>> colors;
    std::vector<unsigned int> indices;
    LoadBrain(positions, colors, indices);

    // Create the brain mesh.
    VertexFormat vformat;
    vformat.Bind(VA_POSITION, DF_R32G32B32_FLOAT, 0);
    vformat.Bind(VA_COLOR, DF_R32G32B32A32_FLOAT, 0);
    auto vbuffer = std::make_shared<VertexBuffer>(vformat, NUM_BRAIN_VERTICES);
    auto vertices = vbuffer->Get<Vertex>();
    for (int i = 0; i < NUM_BRAIN_VERTICES; ++i)
    {
        vertices[i].position = positions[i];
        vertices[i].color = colors[i];
    }

    auto ibuffer = std::make_shared<IndexBuffer>(IP_TRIMESH, NUM_BRAIN_TRIANGLES, sizeof(unsigned int));
    std::memcpy(ibuffer->GetData(), indices.data(), ibuffer->GetNumBytes());

    auto effect = std::make_shared<VertexColorEffect>(mProgramFactory);
    mMesh = std::make_shared<Visual>(vbuffer, ibuffer, effect);
    mMesh->UpdateModelBound();
    mPVWMatrices.Subscribe(mMesh->worldTransform, effect->GetPVWMatrixConstant());

    // Select the first triangle as the puncture triangle and use red
    // vertex colors for it.
    int punctureTriangle = 100;
    Vector4<float> red{ 1.0f, 0.0f, 0.0f, 1.0f };
    vertices[indices[3 * punctureTriangle + 0]].color = red;
    vertices[indices[3 * punctureTriangle + 1]].color = red;
    vertices[indices[3 * punctureTriangle + 2]].color = red;

    // Conformally map the mesh to a sphere.
    ConformalMapGenus0<float> cm;
    cm(NUM_BRAIN_VERTICES, positions.data(), NUM_BRAIN_TRIANGLES,
         ibuffer->Get<int>(), punctureTriangle);
    auto const& sphereCoordinates = cm.GetSphereCoordinates();

    vbuffer = std::make_shared<VertexBuffer>(vformat, NUM_BRAIN_VERTICES);
    vertices = vbuffer->Get<Vertex>();
    for (int i = 0; i < NUM_BRAIN_VERTICES; ++i)
    {
        vertices[i].position = sphereCoordinates[i];
        vertices[i].color = colors[i];
    }
    vertices[indices[3 * punctureTriangle + 0]].color = red;
    vertices[indices[3 * punctureTriangle + 1]].color = red;
    vertices[indices[3 * punctureTriangle + 2]].color = red;

    effect = std::make_shared<VertexColorEffect>(mProgramFactory);
    mSphere = std::make_shared<Visual>(vbuffer, ibuffer, effect);
    mSphere->UpdateModelBound();
    mPVWMatrices.Subscribe(mSphere->worldTransform, effect->GetPVWMatrixConstant());

    // Create a subtree for the mesh.  This allows for the trackball to
    // manipulate only the mesh.
    mScene = std::make_shared<Node>();
    mMeshNode = std::make_shared<Node>();
    mMeshNode->localTransform.SetTranslation(2.0f, 0.0f, 0.0f);
    mMeshNode->localTransform.SetUniformScale(1.0f / mExtreme);
    auto meshParent = std::make_shared<Node>();
    meshParent->localTransform.SetTranslation(-mMesh->modelBound.GetCenter());

    // Create a subtree for the sphere.  This allows for the trackball to
    // manipulate only the sphere.
    mSphereNode = std::make_shared<Node>();
    mSphereNode->localTransform.SetTranslation(-2.0f, 0.0f, 0.0f);
    auto sphereParent = std::make_shared<Node>();
    sphereParent->localTransform.SetTranslation(-mSphere->modelBound.GetCenter());

    // Create the scene graph.  The trackball manipulates the entire scene
    // graph initially.
    mScene->AttachChild(mMeshNode);
    mScene->AttachChild(mSphereNode);
    mMeshNode->AttachChild(meshParent);
    meshParent->AttachChild(mMesh);
    mSphereNode->AttachChild(sphereParent);
    sphereParent->AttachChild(mSphere);

    mTrackball.Set(mScene);
    mTrackball.Update();
}
