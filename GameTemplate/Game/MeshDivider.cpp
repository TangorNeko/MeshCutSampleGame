#include "stdafx.h"
#include "MeshDivider.h"
#include "TriangleDivider.h"

namespace Util
{
	std::pair<TkmFile::SMesh,TkmFile::SMesh> MeshDivider::Divide()
	{
		TriangleDivider triangleDivider;

		PlaneData plane;
		Vector3 cutNormal = { -1.0f,1.0f,1.0f };
		cutNormal.Normalize();
		plane.SetNormal(cutNormal);
		plane.SetPoint({ 0.0f,70.0f,0.0f });
		triangleDivider.Init(plane,&m_divideMesh->vertexBuffer,&m_frontIndexBuffer,&m_backIndexBuffer,&m_newVertexContainer);


		std::vector<TkmFile::SIndexbuffer16> frontIndexBufferArray;
		std::vector<TkmFile::SIndexbuffer16> backIndexBufferArray;
		//マテリアルごとに分割
		for (auto& index : m_divideMesh->indexBuffer16Array)
		{
			for (auto it = index.indices.begin();it != index.indices.end();it += 3)
			{
				TriangleData triangleData;
				triangleData.vertexIndexes[0] = *it;
				triangleData.vertexIndexes[1] = *(it+1);
				triangleData.vertexIndexes[2] = *(it+2);

				triangleDivider.Divide(triangleData);
			}

			//1マテリアル分が終わったらそれぞれのインデックスバッファの配列に追加
			frontIndexBufferArray.push_back(m_frontIndexBuffer);
			backIndexBufferArray.push_back(m_backIndexBuffer);

			//追加が終わったら追加に使用した受け取り用の可変長配列はクリアして次回に備える
			m_frontIndexBuffer.indices.clear();
			m_backIndexBuffer.indices.clear();
		}

		TkmFile::SMesh FrontNewMesh,BackNewMesh;

		FrontNewMesh.isFlatShading = m_divideMesh->isFlatShading;
		BackNewMesh.isFlatShading = m_divideMesh->isFlatShading;

		FrontNewMesh.materials = m_divideMesh->materials;
		BackNewMesh.materials = m_divideMesh->materials;

		FrontNewMesh.vertexBuffer = m_divideMesh->vertexBuffer;
		BackNewMesh.vertexBuffer = m_divideMesh->vertexBuffer;

		//IndexBuffer16Arrayのみを使っているようなのでそちらのみ格納
		FrontNewMesh.indexBuffer16Array = frontIndexBufferArray;
		BackNewMesh.indexBuffer16Array = backIndexBufferArray;

		return std::make_pair(FrontNewMesh, BackNewMesh);
	}
}
