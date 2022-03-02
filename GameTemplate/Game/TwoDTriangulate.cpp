#include "stdafx.h"
#include "TwoDTriangulate.h"

int TwoDTriangulate::CCW(Vector2 a, Vector2 b, Vector2 c)
{
    //b-=a,c-=a
    b.Subtract(a);
    c.Subtract(a);

    if (Cross(b, c) > 0)
    {
        return +1;
    }

    if (Cross(b, c) < 0)
    {
        return -1;
    }

    if (Dot(b, c) < 0)
    {
        return +2;
    }

    if (b.LengthSq() < c.LengthSq())
    {
        return -2;
    }

    return 0;
}

std::vector<Vector2> TwoDTriangulate::MakeTriangle(const Vector2& a, const Vector2& b, const Vector2& c)
{
    std::vector<Vector2> ret(3);
    ret[0] = a;
    ret[1] = b;
    ret[2] = c;
    return ret;
}

void TwoDTriangulate::PushIndex(const Vector2& a, const Vector2& b, const Vector2 c)
{
    m_indexBuffer.push_back(m_2DMap->at(a));
    m_indexBuffer.push_back(m_2DMap->at(b));
    m_indexBuffer.push_back(m_2DMap->at(c));
}

bool TwoDTriangulate::TriangleContains(const std::vector<Vector2>& tri, const Vector2& p)
{
    return CCW(tri[0], tri[1], p) >= 0 && CCW(tri[1], tri[2], p) >= 0 && CCW(tri[2], tri[0], p) >= 0;
}

bool TwoDTriangulate::EarQ(int i, int j, int k, const std::vector<Vector2>& P)
{
    std::vector<Vector2> tri = MakeTriangle(P[i], P[j], P[k]);
    if (CCW(tri[0], tri[1], tri[2]) <= 0)
    {
        return true;
    }

    for (int m = 0; m < P.size(); ++m)
    {
        if (m != i && m != j && m != k)
        {
            if (TriangleContains(tri, P[m]))
            {
                return false;
            }
        }
    }
    return true;
}

void TwoDTriangulate::Triangulate(const std::vector<Vector2>& P)
{
    int processNum = 0;
    m_indexBuffer.clear();

    const int n = static_cast<int>(P.size());
    std::vector<int> l, r;
    for (int i = 0; i < n; ++i)
    {
        l.push_back((i - 1 + n) % n);
        r.push_back((i + 1 + n) % n);
    }

    int i = n - 1;
    while (m_indexBuffer.size() / 3 < n - 2)
    {
        i = r[i];
        if (EarQ(l[i], i, r[i], P))
        {
            PushIndex(P[l[i]], P[i], P[r[i]]);
            l[r[i]] = l[i];
            r[l[i]] = r[i];
        }
        

        //WARNING:アルゴリズムの計算量がO(n^2)とのことだったので
        //ループ回数がn^2を超えた時、時間がかかりすぎている==ループから抜け出せないと判断して強引に脱出している
        if (processNum > n * n)
        {
            OutputDebugStringA("Triangulate processNum over n^2 \n");
            return;
        }

        processNum++;
    }
}