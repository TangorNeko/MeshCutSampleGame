#pragma once
//#include "../../MiniEngine/RenderTarget.h"

namespace Graphics
{
	class ShadowRender
	{
	public:
		/**
		 * @brief 初期化
		*/
		void Init();

		/**
		 * @brief シャドウマップの描画
		 * @param rc 
		*/
		void Draw(RenderContext rc);

		/**
		 * @brief シャドウマップを取得
		 * @return シャドウマップ
		*/
		Texture& GetShadowMap()
		{
			return m_shadowMap.GetRenderTargetTexture();
		}
	private:
		RenderTarget m_shadowMap;	//シャドウマップ
	};
}

