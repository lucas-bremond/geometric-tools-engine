// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2019
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.23.0 (2019/03/05)

#include "IntersectTriangles2DWindow.h"

namespace gte
{
    template class TIQuery<float, Triangle2<float>, Triangle2<float>>;
}

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

    Window::Parameters parameters(L"IntersectTriangles2DWindow", 0, 0, 512, 512);
    auto window = TheWindowSystem.Create<IntersectTriangles2DWindow>(parameters);
    TheWindowSystem.MessagePump(window, TheWindowSystem.DEFAULT_ACTION);
    TheWindowSystem.Destroy<IntersectTriangles2DWindow>(window);
    return 0;
}

IntersectTriangles2DWindow::IntersectTriangles2DWindow(Parameters& parameters)
    :
    Window2(parameters),
    mActive(0),
    mHasIntersection(false),
    mDoTIQuery(true)
{
    mTriangle[0].v[0] = { 260.0f, 260.0f };
    mTriangle[0].v[1] = { 388.0f, 260.0f };
    mTriangle[0].v[2] = { 420.0f, 400.0f };

    mTriangle[1].v[0] = { 252.0f, 252.0f };
    mTriangle[1].v[1] = { 152.0f, 248.0f };
    mTriangle[1].v[2] = { 200.0f, 100.0f };

    mDoFlip = true;
    DoQuery();
}

void IntersectTriangles2DWindow::OnDisplay()
{
    uint32_t const white = 0xFFFFFFFF;
    uint32_t const redL = 0xFF0000FF;
    uint32_t const redD = 0xFF000080;
    uint32_t const blueL = 0xFFFF0000;
    uint32_t const blueD = 0xFF800000;
    uint32_t const yellowL = 0xFF00FFFF;
    uint32_t const yellowD = 0xFF0080FF;
    uint32_t const greenL = 0xFF00FF00;
    uint32_t const greenD = 0xFF008000;

    ClearScreen(white);

    if (mHasIntersection)
    {
        DrawTriangle(mTriangle[0].v, yellowL, yellowD);
        DrawTriangle(mTriangle[1].v, greenL, greenD);
        if (!mDoTIQuery && mIntersection.size() >= 3)
        {
            DrawIntersection();
        }
    }
    else
    {
        DrawTriangle(mTriangle[0].v, redL, redD);
        DrawTriangle(mTriangle[1].v, blueL, blueD);
        if (!mDoTIQuery && mIntersection.size() >= 3)
        {
            DrawIntersection();
        }
    }

    mScreenTextureNeedsUpdate = true;
    Window2::OnDisplay();
}

bool IntersectTriangles2DWindow::OnCharPress(unsigned char key, int x, int y)
{
    float const trnDelta = 1.0f;
    float const rotDelta = (float)(GTE_C_DEG_TO_RAD * 1.0);

    switch (key)
    {
    case '0':  // mTriangle[0] active for translation/rotation
        mActive = 0;
        return true;
    case '1':  // mTriangle[1] active for translation/rotation
        mActive = 1;
        return true;
    case 'x':  // translate active triangle in -x direction
        mTriangle[mActive].v[0][0] -= trnDelta;
        mTriangle[mActive].v[1][0] -= trnDelta;
        mTriangle[mActive].v[2][0] -= trnDelta;
        DoQuery();
        return true;
    case 'X':  // translate active triangle in +x direction
        mTriangle[mActive].v[0][0] += trnDelta;
        mTriangle[mActive].v[1][0] += trnDelta;
        mTriangle[mActive].v[2][0] += trnDelta;
        DoQuery();
        return true;
    case 'y':  // translate active triangle in -y direction
        mTriangle[mActive].v[0][1] -= trnDelta;
        mTriangle[mActive].v[1][1] -= trnDelta;
        mTriangle[mActive].v[2][1] -= trnDelta;
        DoQuery();
        return true;
    case 'Y':  // translate active triangle in +y direction
        mTriangle[mActive].v[0][1] += trnDelta;
        mTriangle[mActive].v[1][1] += trnDelta;
        mTriangle[mActive].v[2][1] += trnDelta;
        DoQuery();
        return true;
    case 'r':  // rotate active triangle clockwise
    {
        Vector2<float> C{ 0.0f, 0.0f };
        for (int i = 0; i < 3; ++i)
        {
            C += mTriangle[mActive].v[i];
        }
        C /= 3.0f;

        float cs = std::cos(-rotDelta), sn = std::sin(-rotDelta);
        for (int i = 0; i < 3; ++i)
        {
            Vector2<float> u = mTriangle[mActive].v[i] - C;
            mTriangle[mActive].v[i] =
            {
                C[0] + cs * u[0] - sn * u[1],
                C[1] + sn * u[0] + cs * u[1]
            };
        }
        DoQuery();
        return true;
    }
    case 'R':  // rotate active triangle counterclockwise
    {
        Vector2<float> C{ 0.0f, 0.0f };
        for (int i = 0; i < 3; ++i)
        {
            C += mTriangle[mActive].v[i];
        }
        C /= 3.0f;

        float cs = std::cos(+rotDelta), sn = std::sin(+rotDelta);
        for (int i = 0; i < 3; ++i)
        {
            Vector2<float> u = mTriangle[mActive].v[i] - C;
            mTriangle[mActive].v[i] =
            {
                C[0] + cs * u[0] - sn * u[1],
                C[1] + sn * u[0] + cs * u[1]
            };
        }
        DoQuery();
        return true;
    }
    case ' ':
        // Support debugging by allowing a DoQuery to occur for the
        // current triangle configuration.
        DoQuery();
        return true;
    case 'f':
    case 'F':
        mDoTIQuery = !mDoTIQuery;
        DoQuery();
        return true;
    }

    return Window2::OnCharPress(key, x, y);
}

bool IntersectTriangles2DWindow::OnMouseClick(int button, int state, int x, int y, unsigned int modifiers)
{
    return Window2::OnMouseClick(button, state, x, y, modifiers);
}

bool IntersectTriangles2DWindow::OnMouseMotion(int button, int x, int y, unsigned int modifiers)
{
    return Window2::OnMouseMotion(button, x, y, modifiers);
}

void IntersectTriangles2DWindow::DrawTriangle(std::array<Vector2<float>, 3> const& vertex,
    uint32_t colorL, uint32_t colorD)
{
    Vector2<float> vmin, vmax;
    ComputeExtremes(3, vertex.data(), vmin, vmax);
    int xmin = static_cast<int>(std::floor(vmin[0]));
    int ymin = static_cast<int>(std::floor(vmin[1]));
    int xmax = static_cast<int>(std::ceil(vmax[0]));
    int ymax = static_cast<int>(std::ceil(vmax[1]));

    PointInPolygon2<float> pip(3, vertex.data());
    Vector2<float> test;
    for (int y = ymin; y <= ymax; ++y)
    {
        test[1] = (float)y;
        for (int x = xmin; x <= xmax; ++x)
        {
            test[0] = (float)x;
            if (pip.ContainsConvexOrderN(test))
            {
                SetPixel(x, y, colorL);
            }
        }
    }

    int x0 = static_cast<int>(vertex[0][0]);
    int y0 = static_cast<int>(vertex[0][1]);
    int x1 = static_cast<int>(vertex[1][0]);
    int y1 = static_cast<int>(vertex[1][1]);
    int x2 = static_cast<int>(vertex[2][0]);
    int y2 = static_cast<int>(vertex[2][1]);
    DrawLine(x0, y0, x1, y1, colorD);
    DrawLine(x1, y1, x2, y2, colorD);
    DrawLine(x2, y2, x0, y0, colorD);
}

void IntersectTriangles2DWindow::DrawIntersection()
{
    uint32_t const black = 0xFF000000;
    uint32_t const gray = 0xFF7F7F7F;

    Vector2<float> vmin, vmax;
    ComputeExtremes(static_cast<int>(mIntersection.size()), mIntersection.data(),
        vmin, vmax);
    int xmin = static_cast<int>(std::floor(vmin[0]));
    int ymin = static_cast<int>(std::floor(vmin[1]));
    int xmax = static_cast<int>(std::ceil(vmax[0]));
    int ymax = static_cast<int>(std::ceil(vmax[1]));

    PointInPolygon2<float> pip(static_cast<int>(mIntersection.size()),
        mIntersection.data());
    Vector2<float> test;
    for (int y = ymin; y <= ymax; ++y)
    {
        test[1] = static_cast<float>(y);
        for (int x = xmin; x <= xmax; ++x)
        {
            test[0] = static_cast<float>(x);
            if (pip.ContainsConvexOrderN(test))
            {
                SetPixel(x, y, gray);
            }
        }
    }

    int x0, y0, x1, y1;
    x0 = static_cast<int>(mIntersection[0][0]);
    y0 = static_cast<int>(mIntersection[0][1]);
    for (size_t i = 1; i < mIntersection.size(); ++i)
    {
        x1 = static_cast<int>(mIntersection[i][0]);
        y1 = static_cast<int>(mIntersection[i][1]);
        DrawLine(x0, y0, x1, y1, black);
        x0 = x1;
        y0 = y1;
    }
    x1 = static_cast<int>(mIntersection[0][0]);
    y1 = static_cast<int>(mIntersection[0][1]);
    DrawLine(x0, y0, x1, y1, black);
}

void IntersectTriangles2DWindow::DoQuery()
{
    if (mDoTIQuery)
    {
        mHasIntersection = mTIQuery(mTriangle[0], mTriangle[1]).intersect;
    }
    else
    {
        mIntersection = mFIQuery(mTriangle[0], mTriangle[1]).intersection;
        mHasIntersection = (mIntersection.size() > 0);
    }

    OnDisplay();
}
