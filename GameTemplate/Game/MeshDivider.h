#pragma once
#include "tkFile/TkmFile.h"

namespace Util
{
	using IndexBuffer = TkmFile::SIndexBuffer32;
	using NewPointMap = std::map<std::pair<Vector3, Vector3>, uint32_t>;

	class MeshDivider
	{
		/**
		 * @brief 分割するメッシュの初期化
		 * @param[in] mesh 分割するメッシュ
		*/
		void Init(TkmFile::SMesh* mesh)
		{
			m_divideMesh = mesh;
		}

		/**
		 * @brief メッシュの分割(仮)
		*/
		void Divide();

	private:
		TkmFile::SMesh* m_divideMesh = nullptr;//分割するメッシュ
		//WARNING:mapに使用するためだけにVector3同士の比較演算子を適当に定義しているので、比較の結果が正しくない可能性がある。
		//m_newVertexContainer周りでエラーが起きた際はまずそこをあたること。
		NewPointMap m_newVertexContainer;	//新しくできた頂点のインデックスを格納する連想配列
		IndexBuffer m_frontIndexBuffer;		//表側のメッシュのインデックスバッファ
		IndexBuffer m_backIndexBuffer;		//裏側のメッシュのインデックスバッファ
	};
}

