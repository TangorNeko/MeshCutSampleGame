#include "stdafx.h"
#include "MeshDivider.h"
#include "TriangleDivider.h"
#include "CutSurfaceMaker.h"

namespace Util
{
	bool MeshDivider::DivideCheck(const Vector3& cutNormal, const Vector3& cutPoint)
	{
		TriangleDivider triangleDivider;

		PlaneData plane;
		plane.SetNormal(cutNormal);
		plane.SetPoint(cutPoint);
		triangleDivider.Init(plane, &m_divideMesh->vertexBuffer);

		for (auto& index : m_divideMesh->indexBuffer16Array)
		{
			for (auto it = index.indices.begin();it != index.indices.end();it += 3)
			{
				TriangleData triangleData;
				triangleData.vertexIndexes[0] = *it;
				triangleData.vertexIndexes[1] = *(it + 1);
				triangleData.vertexIndexes[2] = *(it + 2);

				bool isDivide = triangleDivider.DivideCheck(triangleData);

				if (isDivide == true)
				{
					return true;
				}
			}
		}

		return false;
	}

	std::pair<TkmFile::SMesh,TkmFile::SMesh> MeshDivider::Divide(const Vector3& cutNormal, const Vector3& cutPoint)
	{
		TriangleDivider triangleDivider;

		PlaneData plane;
		plane.SetNormal(cutNormal);
		plane.SetPoint(cutPoint);

		//NOTE:切断面の作成のための仮処理
		std::set<std::pair<uint16_t, uint16_t>> cutSurfaceSegmentSet;
		triangleDivider.SetCutSeg(&cutSurfaceSegmentSet);


		triangleDivider.Init(plane,&m_divideMesh->vertexBuffer);
		triangleDivider.InitReceiver(&m_frontIndexBuffer, &m_backIndexBuffer);

		std::vector<TkmFile::SIndexbuffer16> frontIndexBufferArray;
		std::vector<TkmFile::SIndexbuffer16> backIndexBufferArray;
		//マテリアルごとに分割

		//何番目のマテリアルかを記録する
		int materialIndex = 0;

		//表のメッシュ、裏のメッシュごとに削除するマテリアル番号を記録する可変長配列
		std::list<int> frontEraseIndexes;
		std::list<int> backEraseIndexes;
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
			//1マテリアル分のインデックスバッファが1つでもある?
			if (m_frontIndexBuffer.indices.size() > 0)
			{
				//1つでもあれば追加
				frontIndexBufferArray.push_back(m_frontIndexBuffer);
			}
			else
			{
				//なければそのマテリアルは削除しなければいけないので何番目かを記録
				frontEraseIndexes.push_back(materialIndex);
			}

			//裏側のマテリアルでも同様にする
			if (m_backIndexBuffer.indices.size() > 0)
			{
				backIndexBufferArray.push_back(m_backIndexBuffer);
			}
			else
			{
				backEraseIndexes.push_back(materialIndex);
			}

			//追加が終わったら追加に使用した受け取り用の可変長配列はクリアして次回に備える
			m_frontIndexBuffer.indices.clear();
			m_backIndexBuffer.indices.clear();

			//次のマテリアルに進むのでインデックスを増やす
			materialIndex++;
		}

		//切断面の生成
		CutSurfaceMaker csm;
		csm.Init(&cutSurfaceSegmentSet, &m_divideMesh->vertexBuffer);
		csm.ConvertFromSet();
		csm.MakeLinkFromSet();
		bool needToMakeSurface = csm.CalcIn2D(cutNormal);

		Util::VertexBuffer backVertexBuffer;
		if (needToMakeSurface == true)
		{
			//切断面を生成する必要があれば生成
			csm.MakeSurface(m_divideMesh->vertexBuffer, backVertexBuffer ,&frontIndexBufferArray, &backIndexBufferArray);
		}

		TkmFile::SMesh FrontNewMesh,BackNewMesh;

		FrontNewMesh.isFlatShading = m_divideMesh->isFlatShading;
		BackNewMesh.isFlatShading = m_divideMesh->isFlatShading;

		FrontNewMesh.materials = m_divideMesh->materials;
		BackNewMesh.materials = m_divideMesh->materials;

		//表裏別々にいらないマテリアルを削除する

		//何回削除したかを格納する変数
		int deleteIndex = 0;

		for (auto frontEraseIndex : frontEraseIndexes)
		{
			//削除する場所の位置から削除した回数を引いた数字の場所に削除するマテリアルがある
			FrontNewMesh.materials.erase((FrontNewMesh.materials.begin() + (frontEraseIndex - deleteIndex)));
			
			//削除したので回数を増やす
			deleteIndex++;
		}

		//裏面でも同様にする
		//削除した回数はリセットする
		deleteIndex = 0;
		for (auto backEraseIndex : backEraseIndexes)
		{
			BackNewMesh.materials.erase((BackNewMesh.materials.begin() + (backEraseIndex - deleteIndex)));
			deleteIndex++;
		}

		//表側の頂点バッファを格納
		FrontNewMesh.vertexBuffer = m_divideMesh->vertexBuffer;

		//切断面生成の必要がある時は裏表で頂点バッファが別になるが、
		//そうでない場合は同じ頂点バッファを使用するため表と裏で同じ頂点バッファを使用する
		if (needToMakeSurface == true)
		{
			BackNewMesh.vertexBuffer = backVertexBuffer;
		}
		else
		{
			BackNewMesh.vertexBuffer = m_divideMesh->vertexBuffer;
		}

		//IndexBuffer16Arrayのみを使っているようなのでそちらのみ格納
		FrontNewMesh.indexBuffer16Array = frontIndexBufferArray;
		BackNewMesh.indexBuffer16Array = backIndexBufferArray;

		return std::make_pair(FrontNewMesh, BackNewMesh);
	}
}
