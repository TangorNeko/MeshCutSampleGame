#pragma once
#include "ShadowRender.h"

namespace Graphics
{
	class RenderingEngine
	{
	public:
		/**
		 * @brief ������
		*/
		void Init();

		/**
		 * @brief �`�揈��
		 * @param rc �����_�����O�R���e�L�X�g
		*/
		void Render(RenderContext& rc);

		/**
		 * @brief �r���[�|�[�g���f�t�H���g�T�C�Y�ɖ߂�
		 * @param rc �����_�����O�R���e�L�X�g
		*/
		void SetDefaultViewPort(RenderContext& rc);

		/**
		 * @brief �V���h�E�}�b�v���擾
		 * @return 
		*/
		Texture& GetShadowMap()
		{
			return m_shadowRender.GetShadowMap();
		}
	private:
		ShadowRender m_shadowRender;
	};

	extern RenderingEngine* g_renderingEngine;	//�����_�����O�G���W��
}

