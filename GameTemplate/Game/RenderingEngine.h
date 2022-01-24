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
	private:
		ShadowRender m_shadowRender;
	};
}

