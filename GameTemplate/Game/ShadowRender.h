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
	private:
		RenderTarget m_shadowMap;
	};
}

