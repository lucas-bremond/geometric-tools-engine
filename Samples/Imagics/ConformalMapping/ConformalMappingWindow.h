// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2019
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.23.0 (2019/03/18)

#pragma once

#include <GTEngine.h>
using namespace gte;

class ConformalMappingWindow : public Window3
{
public:
    ConformalMappingWindow(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(unsigned char key, int x, int y) override;

private:
    // These are known for the file Brain_V4098_T8192.binary.
    enum
    {
        NUM_BRAIN_VERTICES = 4098,
        NUM_BRAIN_TRIANGLES = 8192,
    };

    bool SetEnvironment();

    // Load the brain data set, scale it for numerical stability of the
    // conformal mapping and generate colors based on mean curvatures at the
    // vertices.
    void LoadBrain(std::vector<Vector3<float>>& positions,
        std::vector<Vector4<float>>& colors, std::vector<unsigned int>& indices);

    void CreateScene();

    struct Vertex
    {
        Vector3<float> position;
        Vector4<float> color;
    };

    std::shared_ptr<RasterizerState> mWireState;
    std::shared_ptr<Node> mScene;
    std::shared_ptr<Node> mMeshNode;
    std::shared_ptr<Node> mSphereNode;
    std::shared_ptr<Visual> mMesh;
    std::shared_ptr<Visual> mSphere;
    float mExtreme;
};
