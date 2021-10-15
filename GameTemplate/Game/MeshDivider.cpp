#include "stdafx.h"
#include "MeshDivider.h"
#include "TriangleDivider.h"
#include "CutSurfaceMaker.h"

namespace Util
{
	std::pair<TkmFile::SMesh,TkmFile::SMesh> MeshDivider::Divide(const Vector3& cutNormal, const Vector3& cutPoint)
	{
		TriangleDivider triangleDivider;

		PlaneData plane;
		plane.SetNormal(cutNormal);
		plane.SetPoint(cutPoint);

		//NOTE:切断面の作成のための仮処理
		std::set<std::pair<uint16_t, uint16_t>> cutSurfaceSegmentSet;
		triangleDivider.SetCutSeg(&cutSurfaceSegmentSet);


		triangleDivider.Init(plane,&m_divideMesh->vertexBuffer,&m_frontIndexBuffer,&m_backIndexBuffer);


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
			//TODO:1マテリアル分すべてが分割の結果無くなった時の処理
			frontIndexBufferArray.push_back(m_frontIndexBuffer);
			backIndexBufferArray.push_back(m_backIndexBuffer);

			//追加が終わったら追加に使用した受け取り用の可変長配列はクリアして次回に備える
			m_frontIndexBuffer.indices.clear();
			m_backIndexBuffer.indices.clear();
		}

		//TODO:切断面の生成
		CutSurfaceMaker csm;
		csm.Init(&cutSurfaceSegmentSet, &m_divideMesh->vertexBuffer);
		csm.ConvertFromSet();
		csm.MakeLinkFromSet();

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
