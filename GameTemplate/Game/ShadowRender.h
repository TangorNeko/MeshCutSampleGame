#pragma once
//#include "../../MiniEngine/RenderTarget.h"

namespace Graphics
{
	class ShadowRender
	{
	public:
		/**
		 * @brief ������
		*/
		void Init();

		/**
		 * @brief �V���h�E�}�b�v�̕`��
		 * @param rc 
		*/
		void Draw(RenderContext rc);
	private:
		RenderTarget m_shadowMap;
	};
}

