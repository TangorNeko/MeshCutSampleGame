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
	private:
		ShadowRender m_shadowRender;
	};
}

