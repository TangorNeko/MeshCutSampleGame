#pragma once
#include "tkFile/TkmFile.h"

namespace Util
{
	using IndexBuffer = TkmFile::SIndexbuffer16;

	class MeshDivider
	{
	public:
		/**
		 * @brief 分割するメッシュの初期化
		 * @param[in] mesh 分割するメッシュ
		*/
		void Init(TkmFile::SMesh* mesh)
		{
			m_divideMesh = mesh;
		}

		/**
		 * @brief メッシュを分断しているかのチェック
		 * @param cutNormal 切断面上の一点からの法線の向き
		 * @param cutPoint 切断面上の一点の座標
		 * @return 
		*/
		bool DivideCheck(const Vector3& cutNormal, const Vector3& cutPoint);

		/**
		 * @brief メッシュの分割
		 * @param cutNormal 切断面上の一点からの法線の向き
		 * @param cutPoint 切断面上の一点の座標
		 * @return 
		*/
		std::pair<TkmFile::SMesh, TkmFile::SMesh> Divide(const Vector3& cutNormal, const Vector3& cutPoint);

	private:
		TkmFile::SMesh* m_divideMesh = nullptr;//分割するメッシュ
		IndexBuffer m_frontIndexBuffer;		//表側のメッシュのインデックスバッファ
		IndexBuffer m_backIndexBuffer;		//裏側のメッシュのインデックスバッファ
	};
}

