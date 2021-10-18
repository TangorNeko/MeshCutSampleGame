#pragma once
class TwoDTriangulate
{
	float Cross(const Vector2& a, const Vector2& b);

	float Dot(const Vector2& a, const Vector2& b);

	float LengthSq(const Vector2& vec);

	int CCW(Vector2 a, Vector2 b, Vector2 c);

	std::vector<Vector2> MakeTriangle(const Vector2& a, const Vector2& b, const Vector2& c);

	void PushIndex(const Vector2& a, const Vector2& b, const Vector2 c);

	bool TriangleContains(const std::vector<Vector2>& tri, const Vector2& p);

	bool EarQ(int i, int j, int k, const std::vector<Vector2>& P);

public:
	void SetMap(std::map<Vector2, int>* TDMap)
	{
		m_2DMap = TDMap;
	}

	void Triangulate(const std::vector<Vector2>& P);

	std::vector<int> GetBuffer()
	{
		return m_indexBuffer;
	}
private:
	std::map<Vector2, int>* m_2DMap = nullptr;
	std::vector<int> m_indexBuffer;
};

