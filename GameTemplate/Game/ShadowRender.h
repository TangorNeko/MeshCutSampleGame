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

		/**
		 * @brief �V���h�E�}�b�v���擾
		 * @return �V���h�E�}�b�v
		*/
		Texture& GetShadowMap()
		{
			return m_shadowMap.GetRenderTargetTexture();
		}
	private:
		RenderTarget m_shadowMap;	//�V���h�E�}�b�v
	};
}

