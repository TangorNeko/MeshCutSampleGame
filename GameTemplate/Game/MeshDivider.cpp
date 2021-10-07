#include "stdafx.h"
#include "MeshDivider.h"
#include "TriangleDivider.h"

namespace Util
{
	void MeshDivider::Divide()
	{
		TriangleDivider triangleDivider;

		PlaneData plane;
		plane.SetNormal(Vector3::Up);
		plane.SetPoint(Vector3::Zero);
		triangleDivider.Init(plane,&m_divideMesh->vertexBuffer,&m_frontIndexBuffer,&m_backIndexBuffer,&m_newVertexContainer);

		//ƒ}ƒeƒŠƒAƒ‹‚²‚Æ‚É•ªŠ„
		for (auto& index : m_divideMesh->indexBuffer32Array)
		{
			for (auto it = index.indices.begin();it != index.indices.end();it += 3)
			{
				TriangleData triangleData;
				triangleData.vertexIndexes[0] = *it;
				triangleData.vertexIndexes[1] = *(it+1);
				triangleData.vertexIndexes[2] = *(it+2);

				triangleDivider.Divide(triangleData);
			}
		}

	}
}
