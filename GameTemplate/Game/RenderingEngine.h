#pragma once
#include "ShadowRender.h"

namespace Graphics
{
	class RenderingEngine
	{
	public:
		/**
		 * @brief 初期化
		*/
		void Init();

		/**
		 * @brief 描画処理
		 * @param rc レンダリングコンテキスト
		*/
		void Render(RenderContext& rc);

		/**
		 * @brief ビューポートをデフォルトサイズに戻す
		 * @param rc レンダリングコンテキスト
		*/
		void SetDefaultViewPort(RenderContext& rc);

		/**
		 * @brief シャドウマップを取得
		 * @return 
		*/
		Texture& GetShadowMap()
		{
			return m_shadowRender.GetShadowMap();
		}
	private:
		ShadowRender m_shadowRender;
	};

	extern RenderingEngine* g_renderingEngine;	//レンダリングエンジン
}

